#pragma once

#include "input_recorder_interface.h"
#include "input_converter_interface.h"
#include "input.h"

#include <string>
#include <fstream>

namespace dfw {

class input_recorder_file_text:
	public input_recorder_interface {

	public:

	                        input_recorder_file_text(const input&, const input_converter_interface&);

	void                    set_active(bool);
	void                    open_file(const std::string&);

	bool                    is_active() const;
	void                    record();

	private:

	const input&            in;
	const input_converter_interface& converter;
	bool                    active;
	std::ofstream           stream;
};
}
