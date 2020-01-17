#pragma once

#include <ldv/screen.h>

namespace dfw
{

//!Interface for controllers that can be drawn. Its existence is justified only
//!as a part of the framework.
class controller_interface_drawable
{
	public:
	//!Prototype for a controller's draw method.
	virtual void 			draw(ldv::screen&, int fps)=0;
};

}
