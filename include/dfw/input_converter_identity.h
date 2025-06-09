#pragma once

#include "input_converter_interface.h"

namespace dfw {

/**
 * Identity converter, for the apps that are smart enough to 
 * store their inputs as flags.
 */
class input_converter_identity 
	: public input_converter_interface {

	public

	int     to_flag(int _val) const {return _val;}
	int     to_int(int _val) const {return _val;}
};
}
