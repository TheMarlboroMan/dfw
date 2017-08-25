#ifndef DFRAMEWORK_CONTROLLER_VIEW_MANAGER_H
#define DFRAMEWORK_CONTROLLER_VIEW_MANAGER_H

class controller_interface; //Forward declaration. We don't really need to know anything about the controllers.

struct draw_controller_interface

	void register_controller(int i, const controller_interface * c) {pool[i]=c;}
	void add(size_t c) {draw.push_back(pool[c]);}
	void add(const controller_interface * c) {draw.push_back(c);}
	void clear() {draw.clear();}
	void reserve() {draw.reserve(pool.size());}

	std::map<int, controller_interface *>		pool;
	std::vector<const controller_interface *>	draw;
};

#endif
