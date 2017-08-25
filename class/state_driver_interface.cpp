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
	cvm.register_controller(&controller);
}

bool state_driver_interface::loop(dfw::kernel& kernel)
{
	cvm.reserve();
	auto& fps_counter=kernel.get_fps_counter();

	float delta_step=kernel.get_delta_step();
	auto& input_i=kernel.get_input();

	common_pre_loop_input(input_i, delta_step);
	common_pre_loop_step(delta_step);

	ci->preloop(input_i, delta_step, fps_counter.get_frame_count());

	//The elapsed time from the previous drawing is advanced in logic
	//in discrete steps. If the value is larger than delta_step, 
	//delta step will be used.

	//Actually, delta_step is a fixed value...
	while(fps_counter.consume_loop(delta_step))
	{
		input_i().loop();

		common_loop_input(input_i, delta_step);
		common_loop_step(delta_step);

		ci->loop(input_i, delta_step);
		if(mri!=nullptr && message_q.size()) message_q.process(*mri);
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
				break;
			}
		}
	}

	ci->postloop(input_i, delta_step, fps_counter.get_frame_count());

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

		auto& screen=kernel.get_screen();
		cvm.clear();
		ci->request_draw(cmv);
		for(const auto& c : cmv.draw) c->draw(screen, fps_counter.get_frame_count());
		screen.update();
		fps_counter.end_loop_step();
	}

	return !ci->is_leave();
}

void state_driver_interface::register_message_reader(message_reader_interface& i)
{
	mri=&i;
	message_q.set_reader(i);
}
