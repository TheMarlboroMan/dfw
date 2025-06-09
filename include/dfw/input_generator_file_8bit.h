#pragma once

#include "input_generator_interface.h"
#include "input_converter_interface.h"
#include <string>
#include <fstream>
#include <cstdint>

namespace dfw {

/**
*Reads input from a binary file, bit by bit, with a maximum of 8 inputs per
*tic. Needs a converter to translate input back and forth from app realm to
*file realm.
*/
class input_generator_file_8bit 
	:public input_generator_interface {

	public:

	                        input_generator_file_8bit(const input_converter_interface&);
	void                    open_file(const std::string&);
	void                    set_active(bool);

	void                    tic();
	bool                    is_active() const;
	bool                    is_input_down(int) const;
	bool                    is_input_up(int) const;
	bool                    is_input_pressed(int) const;

	private:

	const input_converter_interface& converter;

	bool                    active;
	int8_t                  state{0},
	                        previous_state{0};
	std::ifstream           stream;

};

}
