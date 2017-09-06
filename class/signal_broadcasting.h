#ifndef DFRAMEWORK_MESSAGE_BROADCASTING_H
#define DFRAMEWORK_MESSAGE_BROADCASTING_H

#include <iostream>
#include <string>
#include <stdexcept>

#include <class/dnot_token.h>

namespace dfw
{

//Base class for signals.
struct broadcast_signal
{
	virtual int 			get_type()const=0;
};

//Base class for receivers...
class signal_receiver
{
	public:

	//Perhaps provide default functionality.
	virtual void			receive_signal(const broadcast_signal&)
	{
		throw std::runtime_error("invoked default message_receiver::receive");
	}
};

//Final dispatcher class.
class signal_dispatcher
{
	public:

	void				dispatch_signal(const broadcast_signal& msg) {for(auto& r : receivers) r->receive_signal(msg);}
	void				register_receiver(dfw::signal_receiver& r) {receivers.push_back(&r);}

	private:

	std::vector<signal_receiver *>	receivers;

};

//Broadcaster class, for use with composition...
class signal_broadcaster
{
	public:

					signal_broadcaster(signal_dispatcher& sd):dispatcher{&sd}{}

	void				send_signal(const broadcast_signal& msg) 
	{
		if(dispatcher==nullptr) throw std::runtime_error("controller is not a dispatcher");
		dispatcher->dispatch_signal(msg);
	}

	private:

	signal_dispatcher *		dispatcher;
};
}

#endif
