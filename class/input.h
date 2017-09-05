#ifndef DFRAMEWORK_INPUT_H
#define DFRAMEWORK_INPUT_H

//std
#include <map>
#include <vector>

//libdansdl2
#include <input/sdl_input/sdl_input.h>

//tools
#include <class/dnot_token.h>

//local
#include "kernel_config_interface.h"

/*Abstrae los sistemas de input usando la clase Controles_SDL. Cada input del
enum de turno se puede asignar a uno o más values SDLK_xxx, según lo mismo
pueda hacerse con varias teclas, joystick o no. Esta clase es la propietaria
de la instancia de controles_SDL.*/

namespace dfw
{

class input
{
	////////////////////////
	//Definiciones....

	public:

	//Public structure to expose a configured input (for example button 3 of the 2nd joystick).
	struct input_description
	{
		enum class types {keyboard, mouse, joystick, none} type;
		int		code, device;
	};

					input(ldi::sdl_input&);
	void 				configure(const std::vector<input_pair>&);
	void 				configure(input_pair);
	void				clear(int);

	ldi::sdl_input&			operator()() {return sdlinput;}

	//Estos son shortcuts para teclas, botones de ratón y botones de joystick.
	bool 				is_input_down(int) const;
	bool 				is_input_up(int) const;
	bool 				is_input_pressed(int) const;

	//Obtiene la descripción del evento en cola.
	input_description		get_current_description() const;
	input_description		locate_description(int) const;
	input_pair			from_description(const input_description&, int);

	protected:

	struct tinput{int sdl_key, device_index;};
	typedef std::multimap<int, tinput> 	t_map;

	struct lookup_result
	{
		struct values{int val, index;};

		enum class types {none, keyboard, mouse, joystick};
		types 			type;
		std::vector<values> 	val;
					lookup_result(types tm):type(tm) {}
	};

	mutable std::map<int, lookup_result> lookup;
	t_map 				keyboard_map;
	t_map 				mouse_map;
	t_map 				joystick_map;

	private:

	ldi::sdl_input&	 		sdlinput;

	lookup_result 			get_lookup(int) const;
};

input::input_description input_description_from_config_token(const tools::dnot_token&);
input::input_description::types	input_description_int_to_type(int);
}
#endif
