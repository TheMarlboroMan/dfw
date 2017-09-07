#ifndef DFRAMEWORK_CONTROLLER_INTERFACE_H
#define DFRAMEWORK_CONTROLLER_INTERFACE_H

#include "state_controller.h"
#include "controller_view_manager.h"
#include "input.h"
#include "controller_interface_drawable.h"

/**
* Interface para un controlador. Los controladores deben ser registrados en
* el director de estados para que los cambios de estado sean posibles y 
* pueda inyectarse la cola de eventos.
*/

namespace dfw
{

class controller_interface:
	//Every single one is drawable. The interface is just there to allow 
	//the controller_view_manager to keep focused on its job.
	public controller_interface_drawable

{
	public:

	controller_interface()
		:states(nullptr), leave(false), break_loop(false) 
	{}

	virtual ~controller_interface() {}

	bool 				is_leave() const {return leave;}
	bool 				is_break_loop() const {return break_loop;}

	void 				set_leave(bool v) {leave=v;}
	void 				set_break_loop(bool v) {break_loop=v;}

	void				inject_state_controller(state_controller& c) {states=&c;}

	virtual void			request_draw(controller_view_manager& cvm) {cvm.add_ptr(this);}

	void				set_state(int v) 
	{
		if(states==nullptr) throw std::runtime_error("state_controller was not injected");
		states->set(v);
	}

	virtual void 			preloop(input&, float delta, int fps)=0;
	virtual void 			loop(input&, float delta)=0;
	virtual void 			postloop(input&, float delta, int fps)=0;
	virtual void 			draw(ldv::screen&, int fps)=0;
	virtual void 			awake(input&)=0;
	virtual void 			slumber(input&)=0;
	virtual bool			can_leave_state() const=0;

	private:

	state_controller *		states;
	bool 				leave,
					break_loop;
};

}
#endif
