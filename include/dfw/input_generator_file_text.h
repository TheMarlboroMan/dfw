#pragma once

#include "input_generator_interface.h"
#include "input_converter_interface.h"
#include <string>
#include <fstream>
#include <cstdint>

namespace dfw {

/**
*Reads input from a text file in which each tic is separated by a space. Data
*is stored as a string. Needs a converter to translate input back and forth 
*from app realm to file realm.
*/
class input_generator_file_text 
	:public input_generator_interface {

	public:

	                        input_generator_file_text(const input_converter_interface&);
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
	int                     state{0},
	                        previous_state{0};
	std::ifstream           stream;

};

}
