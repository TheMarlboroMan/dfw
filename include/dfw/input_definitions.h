#pragma once

#include <vector>
#include <rapidjson/document.h>
#include "base_config.h"

namespace dfw
{

//!Public structure to expose a configured input (for example button 3 of the 2nd joystick).
struct input_description
{
	//!Defines the input device.
	enum class types {keyboard, mouse, joystick, none} type;	//!< Type of device.
	int		code, 		//!< Received code.
			device;		//!< Device.
};

//!An input description paired with an integer, representing an application key like "up" or "action".

//!The app_key value can be later mapped trough enums for easy reference.
struct input_pair
{
	input_description		data;		//!< Device and code.
	int						app_key;	//!< Integer representing an unique action in the application.
};


/**
 * assumes the value is an array of objects with type, device and code and
 * may contain more than one input mapping.
 */
std::vector<dfw::input_description> input_description_from_config_token(const rapidjson::Value&);

/**
 * returns a json node with type, device and code.
 */
rapidjson::Value                config_token_from_input_description(const dfw::input_description&, tools::json_config_file&);
dfw::input_description::types   input_description_type_from_int(int);
int                             input_description_int_from_type(dfw::input_description::types);
}

