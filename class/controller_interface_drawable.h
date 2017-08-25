#ifndef DFRAMEWORK_CONTROLLER_INTERFACE_DRAWABLE_H
#define DFRAMEWORK_CONTROLLER_INTERFACE_DRAWABLE_H

#include <video/screen/screen.h>

class controller_interface_drawable
{
	public:

	virtual void 			draw(ldv::screen&, int fps)=0;
};

#endif
