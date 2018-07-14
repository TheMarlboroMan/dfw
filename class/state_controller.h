#ifndef DFRAMEWORK_STATE_CONTROLLER_H
#define DFRAMEWORK_STATE_CONTROLLER_H

#include <exception>
#include <iostream>
#include <functional>

namespace dfw
{

//!Used as part of the state_driver_interface, controls the current and next 
//!controller ids, as well as controller switching operations.

//!It has been designed to remove cyclic dependencies, as the state_driver
//!and the controllers depend upon it.

class state_controller {
	public:

	//!Defines a function to validate the next state that shall be provided
	//!by the state_driver, receiving the new state id and returning true
	//!if the state exists.
	typedef	std::function<bool(int)>	tf_validation;


	//!Class constructor, gets the default state id. Does not set a
	//!default validation function, which will cause the state_driver
	//!to fail when starting, forcing the code to specify one.
						state_controller(int e)
		:current(e), next(e) {
	}

	//!Sets the new status, throws if no validation function was set or 
	//!the new state is invalid.
	state_controller&			set(int v) {
		if(!f_validate) throw std::runtime_error("state_controller validation function not set");
		if(!f_validate(v)) throw std::runtime_error("invalid state for state_controller");
		next=v;
		return *this;
	}
	
	//!Sets the current validation function.
	state_controller&			set_function(tf_validation f){
		f_validate=f;
		return *this;
	}

	//!Returns true if a change is scheduled.
	bool					is_change() const {
		return current!=next;
	}

	//!Returns true if a validation function was specified, which is
	//!absolutely neccessary for the framework to work.
	bool					is_function_specified() const {
		return (bool)f_validate;
	}

	//!Instructs the class to confirm a state change.
	state_controller&			confirm() {
		current=next;
		return *this;
	}

	//!Instructs the class to reject a state change, restoring the "next" id
	//!to the current one.
	state_controller&			cancel() {
		next=current;
		return *this;
	}

	//!Returns the id for the next state.
	int					get_next() const {return next;}

	//!Returns the id for the current state.
	int					get_current() const {return current;}

	private:

	tf_validation				f_validate;	//!< Validation function, will return true if the given id exists.
	int 					current,	//!< Id of the current state. 
						next;		//!< Id of the next state. Same as current if no change is scheduled.
};

}
#endif
