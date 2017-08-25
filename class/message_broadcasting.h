#ifndef DFRAMEWORK_MESSAGE_BROADCASTING_H
#define DFRAMEWORK_MESSAGE_BROADCASTING_H

#include <iostream>
#include <string>
#include <stdexcept>

#include <class/dnot_token.h>

namespace dfw
{

//This is a weak link since it relies on a dnot token to convey infomation
//but since these messages between controllers are not critical at all, I
//think it is okay.

struct broadcast_message
{
	int 				type;
	tools::dnot_token		tok;
};

class message_receiver
{
	public:

	//Perhaps provide default functionality.
	virtual void			receive(const broadcast_message& msg)
	{
		std::cout<<"default receiver got type "<<msg.type<<std::endl;
	}

	//Indicates whether the controller can be a receiver.
	virtual bool			is_receiver() const {return false;}
};

class message_dispatcher
{
	public:

	void				dispatch(const broadcast_message& msg) {for(auto& r : receivers) r->receive(msg);}
	void				register_receiver(message_receiver& r) {receivers.push_back(&r);}

	private:

	std::vector<message_receiver *>	receivers;

};

class message_broadcaster
{
	public:

	void				broadcast(const broadcast_message& msg) 
	{
		if(dispatcher==nullptr) throw std::runtime_error("controller is not a dispatcher");
		dispatcher->dispatch(msg);
	}
	void				inject_message_dispatcher(message_dispatcher& d) {dispatcher=&d;}

	//Indicates whether the controller can be a broadcaster.
	virtual bool			is_broadcaster() const {return false;}

	private:

	message_dispatcher *		dispatcher=nullptr;
};
}

#endif
