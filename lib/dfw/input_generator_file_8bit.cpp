#include <dfw/input_generator_file_8bit.h>

using namespace dfw;


input_generator_file_8bit::input_generator_file_8bit(
	const input_converter_interface& _converter
)
	:converter{_converter}
{ }

void input_generator_file_8bit::open_file(
	const std::string& _filename
) {

	stream.open(_filename.c_str(), std::ifstream::binary);
}

void input_generator_file_8bit::tic() {

	if(!active) {

		return;
	}

	previous_state=state;
	stream.read(&state, 1);
}

void input_generator_file_8bit::set_active(
	bool _val
) {

	active=_val;
}

bool input_generator_file_8bit::is_active() const {

	return active;
}

bool input_generator_file_8bit::is_input_down(
	int _val
) const {
	
	int input_flag=converter.to_flag(_val);
	return !(previous_state & input_flag) && (state & input_flag);
}

bool input_generator_file_8bit::is_input_up(
	int _val
) const {

	int input_flag=converter.to_flag(_val);
	return !(state & input_flag) && (previous_state & input_flag);
}

bool input_generator_file_8bit::is_input_pressed(
	int _val
) const {

	int input_flag=converter.to_flag(_val);
	return state & input_flag;
}
