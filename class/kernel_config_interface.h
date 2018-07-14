#ifndef DFRAMEWORK_KERNEL_CONFIG_INTERFACE
#define DFRAMEWORK_KERNEL_CONFIG_INTERFACE

#include <vector>
#include <string>
#include <stdexcept>

#include "input_definitions.h"

namespace dfw
{

//!Structure with window information.

//!Set by the kernel, used in information exchange with other framework pieces.
struct window_info {
	int 		px_w, 		//!Physical window width.
			px_h;		//!Physical window height.
	int 		logical_w, 	//!Logical width (not a 1:1 relationship: might be up or downscaled)
			logical_h;	//!Logical height (not a 1:1 relationship: might be up or downscaled)
	std::string 	title;		//!Window title (when the system supports it... that should be always).
	bool 		show_cursor,	//!Indicates whether the mouse cursor will be shown.
			vsync;		//!Indicates if VSync will be active. VSync can play with the framerate.
};

//!Interface to be implemented by the final application, indicated where to get
//!The neccesary information for.
class kernel_config_interface {
	public:

	//!Must return a vector of strings representing each of the sound entries
	//!for the resource manager to load sounds.
	virtual std::vector<std::string> 	get_sound_entries() const=0;

	//!Must return a vector of strings representing each of the sound entries
	//!for the resource manager to load music data.
	virtual std::vector<std::string> 	get_music_entries() const=0;

	//!Must return a vector of strings representing each of the sound entries
	//!for the resource manager to load textures.
	virtual std::vector<std::string> 	get_texture_entries() const=0;

	//!Must return a vector of strings representing each of the sound entries
	//!for the resource manager to load surfaces.
	virtual std::vector<std::string> 	get_surface_entries() const=0;

	//!Must return a window_info structure.
	virtual window_info			get_window_info() const=0;

	//!Must return a vector of input_pair, indicating the input methods and
	//!bindings in the application.
	virtual std::vector<input_pair>		get_input_pairs() const=0;
};

}

#endif
