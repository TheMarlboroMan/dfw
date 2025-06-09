#pragma once

namespace dfw {

/**
 * defines a class that can reply to the status of application inputs, later
 * to be implemented as, for example, inputs to be read from a file instead
 * of from the user devices.
 */
class input_generator_interface {

	public:

/**
 * must retrieve information from wherever it may be.
 */
	virtual void tic()=0;
/**
 * must return true if active. An inactive generator will be skipped when
 * the input phase comes up.
 */
	virtual bool is_active() const=0;

/**
 * must set the active flag
 */
	virtual void set_active(bool)=0;
/**
 * must return true if the app input is down.
 */
	virtual bool is_input_down(int) const=0;
/**
 * must return true if the app input is up.
 */
	virtual bool is_input_up(int) const=0;
/**
 * must return true if the app input is pressed.
 */
	virtual bool is_input_pressed(int) const=0;
};
}

