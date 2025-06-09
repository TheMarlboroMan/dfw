#include <dfw/input_recorder_file_text.h>

#include <cstdint>

using namespace dfw;

input_recorder_file_text::input_recorder_file_text(
	const input& _input,
	const input_converter_interface& _converter
)
	:input_recorder_interface(),
	in{_input},
	converter{_converter}
{}


void input_recorder_file_text::set_active(
	bool _val
) {

	active=_val;
}

void input_recorder_file_text::open_file(
	const std::string& _filename
) {

	stream.open(_filename.c_str(), std::ifstream::trunc);
}

bool input_recorder_file_text::is_active() const {

	return active;
}

void input_recorder_file_text::record() {

	int value=0;

	for(auto val : this->inputs) {

		if(in.is_input_pressed(val)) {

			value |= converter.to_flag(val);
		}
	}

	stream<<value<<" ";
}

