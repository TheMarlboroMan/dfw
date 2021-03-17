#include <dfw/input.h>

using namespace dfw;

input::input(ldi::sdl_input& i)
	:sdlinput(i) {

}

const std::vector<input::lookup_result>& input::get_lookup(int i) const {

	//Insert something if not defined, so as not to crash.
	if(!lookup.count(i)) {

		lookup[i]={};
	}

	return lookup.at(i);
}

bool input::is_input_down(int i) const {

	for(const auto& rl : get_lookup(i)) {

		switch(rl.type) {
			case lookup_result::types::keyboard:
				if(sdlinput.is_key_down(rl.val)) {
					return true;
				}
			break;
			case lookup_result::types::mouse:
				if(sdlinput.is_mouse_button_down(rl.val)) {
					return true;
				}
			break;
			case lookup_result::types::joystick:
				if(sdlinput.is_joystick_button_down(rl.index, rl.val)) {
					return true;
				}
			break;
			case lookup_result::types::none: break;
		}
	}

	return false;
}

bool input::is_input_up(int i) const {

	for(const auto& rl : get_lookup(i)) {

		switch(rl.type) {
			case lookup_result::types::keyboard:
				if(sdlinput.is_key_up(rl.val)) {
					return true;
				}
			break;
			case lookup_result::types::mouse:
				if(sdlinput.is_mouse_button_up(rl.val)) {
					return true;
				}
			break;
			case lookup_result::types::joystick:
				if(sdlinput.is_joystick_button_up(rl.index, rl.val)) {
					return true;
				}
			break;
			case lookup_result::types::none: break;
		}
	}

	return false;
}

bool input::is_input_pressed(int i) const
{
	for(const auto& rl : get_lookup(i)) {

		switch(rl.type) {
			case lookup_result::types::keyboard:
				if(sdlinput.is_key_pressed(rl.val)) {
					return true;
				}
			break;
			case lookup_result::types::mouse:
				if(sdlinput.is_mouse_button_pressed(rl.val)) {
					return true;
				}
			break;
			case lookup_result::types::joystick:
				if(sdlinput.is_joystick_button_pressed(rl.index, rl.val)) {
					return true;
				}
			break;
			case lookup_result::types::none: break;
		}
	}

	return false;
}

void input::configure(const std::vector<input_pair>& v) {

	for(const auto& i : v) {

		configure(i);
	}
}

void input::configure(input_pair i) {

	if(i.data.type==input_description::types::none) {

		return;
	}

	tinput ti={i.data.code, i.data.device};
	auto par=std::make_pair(i.app_key, ti);

	if(!lookup.count(i.app_key)) {

		lookup[i.app_key]={};
	}

	auto type=lookup_result::types::none;

	switch(i.data.type) {
		case input_description::types::none: /*for completeness..  we already returned above*/ return;
		case input_description::types::keyboard:
			keyboard_map.insert(par);
			type=lookup_result::types::keyboard;
		break;
		case input_description::types::mouse:
			mouse_map.insert(par);
			type=lookup_result::types::mouse;
		break;
		case input_description::types::joystick:
			joystick_map.insert(par);
			type=lookup_result::types::joystick;
		break;
	}

	lookup.at(i.app_key).push_back({
		type,
		i.data.code,
		i.data.device
	});
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

input_description input::locate_first_description(
	int i
) const {

	const auto& lr=get_lookup(i);
	if(!lr.size()) {

		return {input_description::types::none, 0, 0};
	}

	const auto& l=lr.front();

	switch(l.type) {
		case lookup_result::types::none:
		break;

		case lookup_result::types::keyboard:
			return {input_description::types::keyboard, l.val, l.index};

		case lookup_result::types::mouse:
			return {input_description::types::mouse, l.val, l.index};

		case lookup_result::types::joystick:
			return {input_description::types::joystick, l.val, l.index};
	}

	return {input_description::types::none, 0, 0};
}

std::vector<input_description> input::locate_description(
	int i
) const {

	const auto& lr=get_lookup(i);
	if(!lr.size()) {

		return {};
	}

	std::vector<input_description> result;
	for(const auto l : lr) {

		switch(l.type) {
			case lookup_result::types::none: break;
			case lookup_result::types::keyboard:
				result.push_back({input_description::types::keyboard, l.val, l.index});
			break;
			case lookup_result::types::mouse:
				result.push_back({input_description::types::mouse, l.val, l.index});
			break;
			case lookup_result::types::joystick:
				result.push_back({input_description::types::joystick, l.val, l.index});
			break;
		}
	}

	return result;
}

input_pair input::from_description(const input_description& e, int key) {

	return input_pair{e, key};
}
