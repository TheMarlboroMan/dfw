#ifndef DFRAMEWORK_INPUT_DEFINITIONS_H
#define DFRAMEWORK_INPUT_DEFINITIONS_H

namespace dfw
{

//Public structure to expose a configured input (for example button 3 of the 2nd joystick).
struct input_description
{
	enum class types {keyboard, mouse, joystick, none} type;
	int		code, device;
};

//An input description paired with an integer, representing an application key like "up" or "action".
struct input_pair
{
	input_description		data;
	int				app_key;
};

}

#endif
