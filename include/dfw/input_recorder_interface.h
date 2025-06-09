#pragma once

#include <vector>

namespace dfw {

/**
 * defines a class that can record user input to be replayed later.
 */
class input_recorder_interface {

	public:

/**
 * must return true if the recorder is active. An inactive recorder will not
 * store any data.
 */
	virtual bool    is_active() const=0;
/**
 * must set the active flag
 */
	virtual void    set_active(bool)=0;
/**
 * must record the current bunch of inputs that are retrieved from the 
 * application input. Only the inputs in "set_inputs" (expressed in app 
 * realm integers) will be requested and stored.
 */
	virtual void    record()=0;

/**
 * stores the app inputs that will be queried and stored. Not all inputs may be
 * stored.
 */
	virtual void    set_inputs(const std::vector<int>& _val) {

		inputs=_val;
	}

	protected:

	std::vector<int> inputs;
};
}
