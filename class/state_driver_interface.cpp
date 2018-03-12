#include "state_driver_interface.h"
#include <chrono>

//TODO: Remove...
#include <iomanip>

using namespace dfw;

state_driver_interface::state_driver_interface(int e)
	:states(e), ci(nullptr)
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
	loop(kernel);
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
	controller.inject_state_controller(states);
	cvm.register_controller(index, &controller);
}

void state_driver_interface::loop(dfw::kernel& kernel)
{
	cvm.reserve();
	auto& fps_counter=kernel.get_fps_counter();

	//Delta time is measured in floats, as it is a small and precise value for
	//each step. To accumulate large values, it is better to use a tools::chrono.
	tools::fps_counter::tdelta delta_step=kernel.get_delta_step();
	auto& input_i=kernel.get_input();
	loop_iteration_data lid(delta_step);

	do {
		common_pre_loop_input(input_i, delta_step);
		common_pre_loop_step(delta_step);

		//The elapsed time from the previous drawing is advanced in logic
		//in discrete steps. If the value is larger than delta_step, 
		//delta step will be used. delta_step is a fixed value but the 
		//fps_counter keeps the timed to be consumed. We will not enter 
		//the loop if there's not enough time to consume...

		lid.step=0;

		std::cout<<"Pre logic: "<<std::fixed<<std::setw(11)<<std::setprecision(6)<<std::setfill('0')<<(double)fps_counter.get_timestep()<<std::endl;

		fps_counter.init_loop_step(get_max_timestep());
		lid.timestep=fps_counter.get_timestep();
		lid.remaning=lid.timestep;

		std::cout<<"Pre logic timestep cut: "<<std::fixed<<std::setw(11)<<std::setprecision(6)<<std::setfill('0')<<(double)lid.timestep<<std::endl;

		while(fps_counter.consume_loop(delta_step)) {
			lid.remaning=fps_counter.get_timestep();
			std::cout<<"Running logic: "<<std::fixed<<std::setw(11)<<std::setprecision(6)<<std::setfill('0')<<(double)lid.remaning<<std::endl;

			//TODO: How about "loop_callback"? Perhaps I want to do something with it? we need another function like ci->input_loop(input_i()) that
			//defaults to loop or something so we can do ci->get_input_callback() ? input_i().loop_callback(*ci->get_input_callback()) : input_i().loop();
		
			input_i().loop();

			common_loop_input(input_i, delta_step);
			common_loop_step(delta_step);

			ci->loop(input_i, lid);
			++lid.step;

			if(ci->is_break_loop()) break;

			if(states.is_change()) {
				if(!ci->can_leave_state()) {
					states.cancel();
				}
				else {
					prepare_state(states.get_next(), states.get_current());
					break;
				}
			}
		}

		if(states.is_change()) {
			//Confirmación del cambio de states.
			controllers[states.get_current()]->slumber(input_i);
			ci=controllers[states.get_next()];
			ci->awake(input_i);
			states.confirm();
		}
		//This would be the drawing phase... Now, we must make sure it
		//takes at least "delta_step" time or else we may draw several
		//times in a row, never entering 
		else {
			std::cout<<"Drawing"<<std::endl;

			//TODO... Hmm... does this... really go here???????
			//It should go right before starting to consume it...
//			fps_counter.init_loop_step(get_max_timestep());

			auto& screen=kernel.get_screen();
			cvm.clear();
			ci->request_draw(cvm);

			//TODO: PASS MORE SHIT, LIKE HOW MUCH IT TOOK TO DRAW THE PREVIOUS????
			cvm.draw(screen, fps_counter.get_frame_count());
			screen.update();
			fps_counter.end_loop_step();

			std::cout<<"DRAWING DONE "<<std::fixed<<std::setw(11)<<std::setprecision(6)<<std::setfill('0')<<fps_counter.get_timestep()<<std::endl;

			//It might happen that drawing is SO fast that takes less than our fixed step... In that case, let us just consume time until the step is fulfilled.
			if(fps_counter.get_timestep() < delta_step) {
				fps_counter.fill_until(delta_step);
				std::cout<<"WAITED UNTIL "<<std::fixed<<std::setw(11)<<std::setprecision(6)<<std::setfill('0')<<fps_counter.get_timestep()<<std::endl;
			}
		}
	}while(!ci->is_leave());
}
