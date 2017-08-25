#ifndef DFRAMEWORK_CONTROLLER_VIEW_MANAGER_H
#define DFRAMEWORK_CONTROLLER_VIEW_MANAGER_H

#include "controller_interface_drawable.h"

namespace dfw
{

//Considering that we point at controller_interface_drawable (which only give
//access to "draw"), it is okay that the pointers are not const. 

class controller_view_manager
{
	public:

	void register_controller(int i, controller_interface_drawable * c) {pool[i]=c;}
	void add(size_t c) {drawables.push_back(pool[c]);}
	void add_ptr(controller_interface_drawable * c) {drawables.push_back(c);}
	void clear() {drawables.clear();}
	void reserve() {drawables.reserve(pool.size());}
	void draw(ldv::screen& s, int fps) {for(const auto& c : drawables) c->draw(s, fps);}

	private:

	std::map<int, controller_interface_drawable *>	pool;
	std::vector<controller_interface_drawable *>	drawables;
};

}

#endif
