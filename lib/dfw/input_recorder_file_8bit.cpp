#include <dfw/input_recorder_file_8bit.h>

#include <cstdint>

using namespace dfw;

input_recorder_file_8bit::input_recorder_file_8bit(
	const input& _input,
	const input_converter_interface& _converter
)
	:input_recorder_interface(),
	in{_input},
	converter{_converter}
{}

input_recorder_file_8bit::~input_recorder_file_8bit() {

	if(stream.is_open()) {

		stream.close();
	}
}


void input_recorder_file_8bit::set_active(
	bool _val
) {

	active=_val;
}

void input_recorder_file_8bit::open_file(
	const std::string& _filename
) {

	stream.open(_filename.c_str(), std::ifstream::binary | std::ifstream::trunc);
}

bool input_recorder_file_8bit::is_active() const {

	return active;
}

void input_recorder_file_8bit::record() {

	char buffer=0;

	for(auto val : this->inputs) {

		if(in.is_input_pressed(val)) {

			buffer |= converter.to_flag(val);
		}
	}

	stream.write(&buffer, 1);
}

