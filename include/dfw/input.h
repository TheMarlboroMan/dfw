#pragma once

//local
#include "input_definitions.h"

//libdansdl2
#include <ldi/sdl_input.h>

//std
#include <map>
#include <vector>

namespace dfw
{

//!Abstraction of input systems.

//!Built upon the libdansdl2 input components, which in turn is built upon the
//!SDL2 input systems. Defines a complete structure with application input
//!configuration and input interfaces. This is the class that owns the
//!libdansdl2 input instance.

class input {
	////////////////////////
	//Definiciones....

	public:

	//!Class constructor.
						input(ldi::sdl_input&);

	//!First called from the kernel, loads all inputs registered by the application.
	void 				configure(const std::vector<input_pair>&);

	//!Configures a single input_pair (as defined in input_definitions.h).
	void 				configure(input_pair);

	//!Removes the given key from the input mapping. The key will represent an
	//!uniqued mapped action assigned through "configure" that may correspond
	//!to more than one device or input code.
	void				clear(int);

	//!Returns the raw instance of the libdansdl2 input component, which can
	//!then be called for finer control.
	ldi::sdl_input&			operator()() {return sdlinput;}

	//!Shortcut to know if a given input key (key being an unique mapped action,
	//!not a device input) is down.
	bool 				is_input_down(int) const;

	//!Shortcut to know if a given input key (key being an unique mapped action,
	//!not a device input) is up.
	bool 				is_input_up(int) const;

	//!Shortcut to know if a given input key (key being an unique mapped action,
	//!not a device input) is pressed. Pressed also corresponds to "down" for
	//!one tick.
	bool 				is_input_pressed(int) const;

	//!Returns the input description object for a given key. Keys are not isolated
	//!input events or codes, but an unique application action mapped to an
	//!integer (such as escape, up, down, jump...). Will return an empty
	//!input description if the key could not be found.
	input_description	locate_first_description(int) const;

	//!Returns all input description objects for a given key. Returns an empty
	//!vectr if the key could not be found.
	std::vector<input_description>	locate_description(int) const;

	//!Creates a new input pair from the given description and input key.
	//!Remember than keys are not input events, but application actions mapped
	//!to an integer.
	input_pair			from_description(const input_description&, int);

	protected:

	struct tinput{
		int sdl_key, 		//!< Input code.
		device_index;		//!< Device index.
	};

	typedef std::multimap<int, tinput> 	t_map;	//!< Internal data storage mapping keys to inputs.

	//!Result of a cached input search.
	struct lookup_result {

		//!Type of input.
		enum class types {none, keyboard, mouse, joystick} type; //!< Type of input.
		int 		val, 		//!< Value.
					index;		//!< Index.
	};

	mutable std::map<int, std::vector<lookup_result>> lookup;	//!< Internal cache. Saves the need to do repeated lookup if the same application action (key) is assigned to various input devices and codes.
	t_map 				keyboard_map;				//!< Keyboard lookup map.
	t_map 				mouse_map;					//!< Mouse lookup map.
	t_map 				joystick_map;				//!< Joystick lookup map.

	private:

	ldi::sdl_input&	 		sdlinput;				//!< Instance of the libdansdl2 component.

	//! Internal lookup method.
	const std::vector<lookup_result>&			get_lookup(int) const;
};

}
