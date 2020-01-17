#pragma once

#include <string>
namespace dfw {

//!Structure with window information.

//!Set by the kernel, used in information exchange with other framework pieces.
struct window_info {
	int 			px_w, 			//!< Physical window width.
					px_h;			//!< Physical window height.
	int 			logical_w, 		//!< Logical width (not a 1:1 relationship: might be up or downscaled)
					logical_h;		//!< Logical height (not a 1:1 relationship: might be up or downscaled)
	std::string 	title;			//!< Window title (when the system supports it... that should be always).
	bool 			show_cursor,	//!< Indicates whether the mouse cursor will be shown.
				vsync;				//!< Indicates if VSync will be active. VSync can play with the framerate.
};


}
