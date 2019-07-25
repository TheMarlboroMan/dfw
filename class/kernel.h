#ifndef DFRAMEWORK_KERNEL_H
#define DFRAMEWORK_KERNEL_H

#include <string>
#include <memory>

#include <src/log.h>

#include <input/sdl_input/sdl_input.h>

#include <class/chrono.h>
#include <class/arg_manager.h>
#include <class/fps_counter.h>

#include "base_config.h"
#include "input.h"
#include "audio.h"
#include "resource_loader.h"
#include "window_info.h"
#include "audio_info.h"

namespace dfw
{

//!The kernel is the heart of the application, owner of all resources and 
//!input-output interfaces.

//!The kernel does not own the state_driver, but is injected into it in order
//!to make all resources available to controllers. The kernel should be setup
//!in the main function by constructing it and then injecting it into the
//!state driver.
class kernel {
	public:

	//!Constructs the kernel. All parameters are mandatory.
				kernel(tools::log&, tools::arg_manager&);

	//!Returns the kernel log. Applications may want to use a different log.
	tools::log&		get_log() {return log_i;}

	//!Returns the input framework class. Read "input" documentation.
	input&			get_input() {return input_i;}

	//!Returns the audio framework class. Read "audio" documentation.
	audio&			get_audio() {return *audio_i;}

	//!Returns the screen. So far we can only manage one.
	ldv::screen&	 	get_screen() {return *screen_i.get();}

	//!Returns the video resource manager.
	ldv::resource_manager&	get_video_resource_manager() {return v_manager;}

	//!Returns the audio resource manager.
	lda::resource_manager&	get_audio_resource_manager() {return a_manager;}

	//!Returns an argument manager object, useful to direct parameter data
	//!to controllers.
	tools::arg_manager& 	get_arg_manager() {return arg_manager_i;}

	//!Returns the fps_counter tool.
	tools::fps_counter&	get_fps_counter() {return fps_counter_i;}

	//!Returns the controller chrono. See member documentation.
	tools::chrono&		get_controller_chrono() {return controller_chrono;}
	
	//!Returns a delta step value. See member documentation.
	tools::fps_counter::tdelta	get_delta_step() const {
		return delta_step;
	}

	//!Sets a new delta step value. See member documentation.
	void 				set_delta_step(tools::fps_counter::tdelta v) {
		delta_step=v;
	}

	//!Starts the screen with the given window info values. Also sets 
	//!cursor and vsync information. Must be called from the state driver.s
	void 			init_video_system(const window_info&);

	void 			init_audio_system(const audio_info&);

	void			init_input_system(const std::vector<dfw::input_pair>&);

	///////////////////
	// Propiedades

	private:

	tools::fps_counter::tdelta			delta_step;	//!< The amount of time to be consumed per call to a controller loop. By default set at 0.01f.
	
	tools::log&					log_i;
	std::unique_ptr<lda::audio_controller>		audiocontroller;
	ldi::sdl_input	 				sdlinput;
	ldv::resource_manager				v_manager;
	lda::resource_manager				a_manager;
	tools::fps_counter	 			fps_counter_i;
	std::unique_ptr<ldv::screen>	screen_i;
	tools::chrono					controller_chrono; //!< Provides a running time of the controller loop.

	input						input_i;
	std::unique_ptr<audio>				audio_i;

	tools::arg_manager& 				arg_manager_i;
};

}
#endif
