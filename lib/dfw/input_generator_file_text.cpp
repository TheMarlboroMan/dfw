#include <dfw/input_generator_file_text.h>

using namespace dfw;


input_generator_file_text::input_generator_file_text(
	const input_converter_interface& _converter
)
	:converter{_converter}
{ }

void input_generator_file_text::open_file(
	const std::string& _filename
) {

	stream.open(_filename.c_str());
}

void input_generator_file_text::tic() {

	if(!active) {

		return;
	}

	int value=0;
	stream>>value;

	previous_state=state;
	state=value;
}

void input_generator_file_text::set_active(
	bool _val
) {

	active=_val;
}

bool input_generator_file_text::is_active() const {

	return active;
}

bool input_generator_file_text::is_input_down(
	int _val
) const {
	
	int input_flag=converter.to_flag(_val);
	return !(previous_state & input_flag) && (state & input_flag);
}

bool input_generator_file_text::is_input_up(
	int _val
) const {

	int input_flag=converter.to_flag(_val);
	return !(state & input_flag) && (previous_state & input_flag);
}

bool input_generator_file_text::is_input_pressed(
	int _val
) const {

	int input_flag=converter.to_flag(_val);
	return state & input_flag;
}
