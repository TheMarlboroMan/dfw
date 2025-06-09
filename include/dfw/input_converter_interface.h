#pragma once

namespace dfw {

/**
 * defines how to convert from app realm to generator/recorder 
 * realm, that is, how to convert the numbers an application uses
 * to convert inputs to the flags that are used by input recorders
 * and replayers.
 */
class input_converter_interface {

	public:

	virtual int to_flag(int) const=0;
	virtual int to_int(int) const=0;
};
}
