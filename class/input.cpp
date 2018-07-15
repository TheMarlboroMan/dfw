#include "input.h"

using namespace dfw;

input::input(ldi::sdl_input& i)
	:sdlinput(i) {

}

input::lookup_result input::get_lookup(int i) const {
	//Esto puede dar problemas en el futuro si queremos usar el mismo
	//input para dos cosas distintas.
	//Vamos a cachear cada input en el dispositivo que le pertenece...

	auto it=lookup.find(i);

	if(it!=lookup.end()) {
		return it->second;
	}
	else {
		lookup_result resultado(lookup_result::types::none);

		auto f=[&resultado, this, i](const t_map& mapa, lookup_result::types t) {
			resultado.type=t;
			auto itr=mapa.equal_range(i);
			for(auto r=itr.first; r!=itr.second; ++r) {
				resultado.val.push_back({r->second.sdl_key, r->second.device_index});
			}

			lookup.insert(std::make_pair(i, resultado));
		};

		//Buscamos en cada uno de los mapas si algo se corresponde con
		//la key de aplicación de entrada. Si existe se meterá
		//en el mapa de lookup el dispositivo y el valor SDL. La
		//lambda además actualiza el resultado para devolverlo.

		if(keyboard_map.count(i)) {
			f(keyboard_map, lookup_result::types::keyboard);
		}
		else if(mouse_map.count(i)) {
			f(mouse_map, lookup_result::types::mouse);
		}
		else if(joystick_map.count(i)) {
			f(joystick_map, lookup_result::types::joystick);
		}

		return resultado;
	}
}

bool input::is_input_down(int i) const {
	lookup_result rl=get_lookup(i);
	switch(rl.type) {
		case lookup_result::types::keyboard:
			for(auto val : rl.val) {
				if(sdlinput.is_key_down(val.val)) {
					return true;
				}
			}
		break;
		case lookup_result::types::mouse:
			for(auto val : rl.val) {
				if(sdlinput.is_mouse_button_down(val.val)) {
					return true;
				}
			}
		break;
		case lookup_result::types::joystick:
			for(auto val : rl.val) {
				if(sdlinput.is_joystick_button_down(val.index, val.val)) {
					return true;
				}
			}
		break;
		case lookup_result::types::none: break;
	}

	return false;
}

bool input::is_input_up(int i) const {
	lookup_result rl=get_lookup(i);
	switch(rl.type) {
		case lookup_result::types::keyboard:
			for(auto val : rl.val) {
				if(sdlinput.is_key_up(val.val)) {
					return true;
				}
			}
		break;
		case lookup_result::types::mouse:
			for(auto val : rl.val) {
				if(sdlinput.is_mouse_button_up(val.val)) {
					return true;
				}
			}
		break;
		case lookup_result::types::joystick:
			for(auto val : rl.val) {
				if(sdlinput.is_joystick_button_up(val.index, val.val)) {
					return true;
				}
			}
		break;
		case lookup_result::types::none: break;
	}

	return false;
}

bool input::is_input_pressed(int i) const
{
	lookup_result rl=get_lookup(i);
	switch(rl.type) {
		case lookup_result::types::keyboard:
			for(auto val : rl.val) {
				if(sdlinput.is_key_pressed(val.val)) {
					return true;
				}
			}
		break;
		case lookup_result::types::mouse:
			for(auto val : rl.val) {
				if(sdlinput.is_mouse_button_pressed(val.val)) {
					return true;
				}
			}
		break;
		case lookup_result::types::joystick:
			for(auto val : rl.val) {
				if(sdlinput.is_joystick_button_pressed(val.index, val.val)) {
					return true;
				}
			}
		break;
		case lookup_result::types::none: break;
	}

	return false;
}

void input::configure(const std::vector<input_pair>& v) {
	for(const auto& i : v) {
		configure(i);
	}
}

void input::configure(input_pair i) {
	tinput ti={i.data.code, i.data.device};
	auto par=std::make_pair(i.app_key, ti);

	switch(i.data.type) {
		case input_description::types::none:		break;
		case input_description::types::keyboard: 	keyboard_map.insert(par); break;
		case input_description::types::mouse:		mouse_map.insert(par); break;
		case input_description::types::joystick:	joystick_map.insert(par); break;
	}
}


//Elimina un input de los mapas y del lookup, liberándolo para poder volverlo
//a configurar.

void input::clear(int key) {
	if(keyboard_map.count(key)) {
		keyboard_map.erase(key);
	}

	if(mouse_map.count(key)) {
		mouse_map.erase(key);
	}

	if(joystick_map.count(key)) {
		joystick_map.erase(key);
	}

	if(lookup.count(key)) {
		lookup.erase(key);
	}
}

//Gets the full input description for the given application key.
input_description input::locate_description(int i) const {

	input_description res{input_description::types::none, 0, 0};

	auto l=get_lookup(i);

	switch(l.type) {
		case lookup_result::types::none:
		break;

		case lookup_result::types::keyboard:
			res.type=input_description::types::keyboard;
			res.code=l.val[0].val;

		break;

		case lookup_result::types::mouse:
			res.type=input_description::types::mouse;
			res.code=l.val[0].val;
		break;

		case lookup_result::types::joystick:
			res.type=input_description::types::joystick;
			res.device=l.val[0].index;
			res.code=l.val[0].val;
		break;
	}

	return res;
}

input_pair input::from_description(const input_description& e, int key) {

	return input_pair{e, key};
}
