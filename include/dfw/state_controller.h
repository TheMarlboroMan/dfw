#pragma once

#include <exception>
#include <iostream>
#include <functional>
#include <stack>

namespace dfw
{

//!Used as part of the state_driver_interface, controls the controller switching 
//!operations.

//!It has been designed to remove cyclic dependencies, as the state_driver
//!and the controllers depend upon it.

class state_controller {
	public:

	//!Defines a function to validate the next state that shall be provided
	//!by the state_driver, receiving the new state id and returning true
	//!if the state exists.
	typedef	std::function<bool(int)>    tf_validation;


	//!Class constructor, gets the default state id. Does not set a
	//!default validation function, which will cause the state_driver
	//!to fail when starting, forcing the code to specify one.
	                         state_controller(int _e)

		:state_stack{}, last_valid_state{0} {

		state_stack.push(_e);
	}

	//!Removes the current state from the stack, causing the
	//!state driver to attempt a change. If the change is cancelled,
	//!the state is pushed back again.
	state_controller&        pop() {

		if(!state_stack.size()) {

			throw std::runtime_error("cannot pop empty state stack");
		}

		last_valid_state=state_stack.top();
		last_operation=optypes::pop;
		state_stack.pop();
		return *this;
	}

	//!Pushes a new state to the stack, causing the state driver to
	//!attempt a change. If the change is cancelled, the state
	//!is popped.
	state_controller&       push(int _val) {

		validate(_val);
		last_valid_state=state_stack.top();
		last_operation=optypes::push;
		state_stack.push(_val);
		return *this;
	}

	//!Sets the new status by replacing the top of the stack.
	state_controller&        set(int _v) {

		validate(_v);
		last_valid_state=state_stack.top();
		last_operation=optypes::set;
		state_stack.pop();
		state_stack.push(_v);
		return *this;
	}
	
	//!Sets the current validation function.
	state_controller&        set_function(tf_validation f){

		f_validate=f;
		return *this;
	}

	//!Returns true if a change is scheduled.
	bool                     is_change() const {

		return optypes::none!=last_operation;
	}

	//!Returns true if a validation function was specified, which is
	//!absolutely neccessary for the framework to work.
	bool                    is_function_specified() const {

		return (bool)f_validate;
	}

	//!Instructs the class to confirm a state change.
	state_controller&       confirm() {

		last_valid_state=0;
		last_operation=optypes::none;
		return *this;
	}

	//!Instructs the class to reject a state change, restoring the previous
	//!stack top.
	state_controller&       cancel() {

		switch(last_operation) {

			case optypes::none:
				throw std::runtime_error("cancel cannot be called without a previous operation");
			
			case optypes::set:
				state_stack.pop();
				state_stack.push(last_valid_state);
			break;

			case optypes::pop:
				state_stack.push(last_valid_state);
			break;

			case optypes::push:
				state_stack.pop();
			break;
		}

		last_valid_state=0;
		return *this;
	}

	//!Returns the size of the state stack.
	std::size_t             size() const {return state_stack.size();}

	int                     get_previous() const {return last_valid_state;}

	//!Returns the id for the current state.
	int                     get_current() const {return state_stack.top();}

	private:

	enum class optypes {none, set, pop, push};

	void                    validate(int _state) {

		if(!f_validate) {

			throw std::runtime_error("state_controller validation function not set");
		}

		if(!f_validate(_state)) {

			throw std::runtime_error("invalid state for state_controller");
		}
	}

	optypes                 last_operation{optypes::none};
	tf_validation           f_validate; //!< Validation function, will return true if the given id exists.
	std::stack<int>         state_stack; //!!< States.
	int                     last_valid_state; //!< Copy of the last valid state after a destructive change.
};

}
