#ifndef DFRAMEWORK_CONTROLLER_VIEW_MANAGER_H
#define DFRAMEWORK_CONTROLLER_VIEW_MANAGER_H

#include "controller_interface_drawable.h"

namespace dfw
{


//!Class that requests and retrieves controller drawing methods. Allows for
//!stacking of view calls between different controllers. This class is part of
//!the framework internals and the end user does not need to interact with it.

//!Considering that we point at controller_interface_drawable (which only give
//!access to "draw"), it is okay that the pointers are not const.
class controller_view_manager {

	public:

	//!Registers the given controller into the pool with the given index.
	void register_controller(int i, controller_interface_drawable * c) {
		pool[i]=c;
	}

	//!Adds a drawable index to the pool.
	void add(size_t c) {drawables.push_back(pool[c]);}

	//!Adds an interface to the pool.
	void add_ptr(controller_interface_drawable * c) {
		drawables.push_back(c);
	}

	//!Removes all drawables from the pool.
	void clear() {
		drawables.clear();
	}

	//!Reserves space in the drawables according to the id pool size.
	void reserve() {
		drawables.reserve(pool.size());
	}

	//!Calls draw upon each drawable, effectively stacking views.
	void draw(ldv::screen& s, int fps) {
		for(const auto& c : drawables) {
			c->draw(s, fps);
		}
	}

	private:

	std::map<int, controller_interface_drawable *>	pool;		//!< Internal data map.
	std::vector<controller_interface_drawable *>	drawables;	//!< Vector of drawables: will be called in insertion order.
};

}

#endif
