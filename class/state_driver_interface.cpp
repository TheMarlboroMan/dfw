#include "state_driver_interface.h"
#include <chrono>

using namespace dfw;

state_driver_interface::state_driver_interface(int e, std::function<bool(int)> f)
	:states(e, f), ci(nullptr), mri(nullptr)
{

}

void state_driver_interface::init(dfw::kernel& kernel)
{
	if(!controllers.size())
	{
		throw std::runtime_error("no controllers registered.");
	}

	if(!controllers.count(states.get_current()))
	{
		throw std::runtime_error("initial controller is unregistered.");
	}

	ci=controllers[states.get_current()];
	ci->awake(kernel.get_input());

	//TODO: Good moment to really restart the fps thing...
	kernel.get_fps_counter().reset();
	kernel.get_controller_chrono().start();
	while(loop(kernel));
	kernel.get_controller_chrono().stop();
	kernel.get_log()<<"controller logic ran for "<<kernel.get_controller_chrono().get_seconds()<<" seconds"<<std::endl;
}

void state_driver_interface::register_controller(int index, controller_interface& controller)
{
	if(controllers.count(index))
	{
		throw std::runtime_error("duplicate index for controller");
	}
	
	controllers[index]=&controller;
	controller.inject_message_queue(message_q);
	controller.inject_state_controller(states);
}

bool state_driver_interface::loop(dfw::kernel& kernel)
{
	auto& fps_counter=kernel.get_fps_counter();

	float delta_step=kernel.get_delta_step();
	auto& input_i=kernel.get_input();

	common_step(delta_step);
	common_input(input_i, delta_step);

	ci->preloop(input_i, delta_step, fps_counter.get_frame_count());

	//Time elapsed since previous drawing is consumed here, hoping it will 
	//average. The idea is that the same time is devoted to running the 
	//logic and to the rendering. Since rendering takes the most, it is used 
	//to measure the number of logic cycles that will be run.
	//In short bursts is can work bu it is clearly, wrong. If it takes me 2 
	//seconds to draw a frame I will then do 2 seconds of logic, and that 
	//will quickly escalate. It would make sense in a multithreaded 
	//environment where I can consume on one thread the time I generate in 
	//the renderer, but not here.
	//On the other hand, it mostly works when the renderer can yield good
	//framerates, corresponding with short logic bursts... That's why 
	//get_max_timestep exists above.

	//Actually, delta_step is a fixed value...
	while(fps_counter.consume_loop(delta_step))
	{
		input_i().loop();
		ci->loop(input_i, delta_step);
		if(ci->is_break_loop()) break;

		if(states.is_change()) 
		{
			if(!ci->can_leave_state())
			{
				states.cancel();
			}
			else
			{
				prepare_state(states.get_next(), states.get_current());
				if(mri!=nullptr && message_q.size()) message_q.process(*mri);
				break;
			}
		}
	
		if(mri!=nullptr && message_q.size()) message_q.process(*mri);
	}

	if(states.is_change())
	{
		//Confirmación del cambio de states.
		controllers[states.get_current()]->slumber(input_i);
		ci=controllers[states.get_next()];
		ci->awake(input_i);
		states.confirm();
	}
	else
	{
		fps_counter.init_loop_step(get_max_timestep());

		ci->postloop(input_i, delta_step, fps_counter.get_frame_count());
		ci->draw(kernel.get_screen(), fps_counter.get_frame_count());

		kernel.get_screen().update();

		fps_counter.end_loop_step();
	}

	return !ci->is_leave();
}

void state_driver_interface::register_message_reader(message_reader_interface& i)
{
	mri=&i;
	message_q.set_reader(i);
}
