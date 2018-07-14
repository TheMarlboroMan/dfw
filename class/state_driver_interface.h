#ifndef DFRAMEWORK_STATE_DRIVER_INTERFACE_H
#define DFRAMEWORK_STATE_DRIVER_INTERFACE_H

#include <stdexcept>
#include <memory>
#include <vector>
#include <functional>
#include <map>

#include "state_controller.h"
#include "controller_interface.h"
#include "controller_view_manager.h"
#include "kernel.h"
#include "signal_broadcasting.h"

namespace dfw
{

//!This is the application centre: owner of all resources that are not kernel
//!bound and owner-orchestrator of all controllers.

//!Every application must have a state_driver that extends this class and,
//!unlike the kernel's, its logic is configurable by the end developer.

class state_driver_interface {
	public:

	//!Class constructor, receives an integer that represents the controller
	//!that it will first execute.
						state_driver_interface(int e);

	//!Virtual class destructor.
	virtual					~state_driver_interface() {
	}

	//!Executed just before confirming a controller change (just before the
	//!old controller slumbers and the new awakens). Can be used to
	//!exchange information between controllers, as it receives the id
	//!of the new and old controllers.
	virtual void				prepare_state(int next, int current)=0;

	//!Executeed before the iterations of a controller are called (as many
	//!as needed until screen drawing can be done). Receives the input and 
	//!delta time. It is supposed to handle common input data tasks.
	virtual void				common_pre_loop_input(dfw::input&, float delta)=0;

	//!Executed right after common_pre_loop_input, it is supposed to handle
	//!common application logic.
	virtual void				common_pre_loop_step(float delta)=0;

	//!Executed inside the controller iteration loop, before the controller's
	//!loop method runs. Serves the same purpose as common_pre_loop_input.
	virtual void				common_loop_input(dfw::input&, float delta)=0;

	//!Excutes between common_loop_input and the controller's loop method,
	//!to handle iteration specifics.
	virtual void				common_loop_step(float delta)=0;

	//!Returns the maximum timestep value (in case refreshing takes to long,
	//!which should result in slowdown).
	virtual float				get_max_timestep() const=0;

	//!Returns the signal dispatcher object.
	signal_dispatcher&			get_signal_dispatcher() {return sd;}

	//!Sort of "main" for the driver: awakes the current controller and
	//!starts the inner loop, which will run until the application stops.
	void					init(dfw::kernel&);

	//!Registers a controller with the given id. Will throw if there's 
	//!a controller with the same id.
	void					register_controller(int, controller_interface&);

	protected:
	
	std::map<int, controller_interface *>	controllers;	//!< Internal controller map.
	state_controller			states;		//!< State controller, see state_controller docs.

	private:

	//!Central application loop.
	void					loop(dfw::kernel&);

	controller_view_manager			cvm;		//!< Allows access to the drawing methods of controllers, even stacking them.
	controller_interface *			ci;		//!< Current controller.
	signal_dispatcher			sd;		//!< Signal dispatcher object, to be injected in controllers.
};


}
#endif
