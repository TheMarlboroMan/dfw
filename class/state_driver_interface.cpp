#include "state_driver_interface.h"

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
	while(loop(kernel));
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
	//Aquí se mide el tiempo desde el último paso por este loop...
	auto& fps_counter=kernel.get_fps_counter();
	fps_counter.init_loop_step(get_max_timestep());

	float delta_step=kernel.get_delta_step();
	auto& input_i=kernel.get_input();

	common_step(delta_step);
	common_input(input_i, delta_step);

	ci->preloop(input_i, delta_step, kernel.get_fps());

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

	//TODO: Really comment this part.

	while(fps_counter.consume_loop(delta_step))
	{
		input_i().loop();

		ci->loop(input_i, delta_step);

		if(ci->is_break_loop()) 
		{
			break;
		}

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

	fps_counter.end_loop_step();

	if(states.is_change())
	{
		//Confirmación del cambio de states.
		int current=states.get_current();
		int next=states.get_next();

		controllers[current]->slumber(input_i);
		controllers[next]->awake(input_i);

		ci=controllers[next];

		states.confirm();
	}
	else
	{
		ci->postloop(input_i, delta_step, kernel.get_fps());
		ci->draw(kernel.get_screen(), kernel.get_fps());

		kernel.get_screen().update();
	}

	return !ci->is_leave();
}

void state_driver_interface::register_message_reader(message_reader_interface& i)
{
	mri=&i;
	message_q.set_reader(i);
}
