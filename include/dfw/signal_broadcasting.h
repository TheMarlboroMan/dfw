#pragma once

#include <tools/dnot_token.h>

#include <iostream>
#include <string>
#include <stdexcept>

namespace dfw
{

//!Base class for signals

//!Specifies the need to assign an id (or type) for each kind of derived signal.

struct broadcast_signal {

	//!Must return a unique integer identifying the type of signal.
	virtual int 			get_type()const=0;
};

//!Base class for receivers...

//!No default functionality is provided: the receiver designed by the user must
//!provide basic signal handling code.
class signal_receiver {
	public:

	//!Will be called when a signal is received. Possible implementations
	//!may include casting to the specific signal type through its get_type
	//!method and acting accordingly to injected state.
	virtual void			receive_signal(const broadcast_signal&)=0;
};

//!Final dispatcher class.

//!There is no need to extend this class: it can be used as it is.
class signal_dispatcher {
	public:

	//!Routes the signal to every receiver.
	void				dispatch_signal(const broadcast_signal& msg) {
		for(auto& r : receivers) {
			r->receive_signal(msg);
		}
	}

	//TODO: Does not check if already registered!!!
	//!Registers a receiver so it can receive broadcasts sent through "dispatch_signal".
	void				register_receiver(dfw::signal_receiver& r) {
		receivers.push_back(&r);
	}

	private:

	std::vector<signal_receiver *>	receivers;	//!< Internal data storage for receivers.

};

//!Signal broadcaster class.

//!It is designed to be used with composition in controllers/classes/entities
//!that need to send messages. Must be constructed with a signal dispacher, 
//!which is turn property of the state_driver_interface.
class signal_broadcaster {
	public:

	//!Class constructor.
					signal_broadcaster(signal_dispatcher& sd)
		:dispatcher{&sd}{
	}

	//!Sends a signal, which will take effect inmediately.
	void				send_signal(const broadcast_signal& msg) 
	{
		dispatcher->dispatch_signal(msg);
	}

	private:

	signal_dispatcher *		dispatcher; //!< Pointer to the internal dispatcher class.
};
}

