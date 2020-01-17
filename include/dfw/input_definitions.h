#pragma once

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

}

