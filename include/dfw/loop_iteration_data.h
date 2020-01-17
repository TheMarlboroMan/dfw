#pragma once

namespace dfw {

//!Time data for controllers.

//!This structure is passed to each controller's loop method with timing information.

struct loop_iteration_data {
	float		delta,		//!< Current delta step.
			timestep,	//!< Duration of the full timestep before drawing.
			remaning;	//!< Time left to consume for drawing.
	int		step;		//!< Iteration number.

	//!Class constructor.
	loop_iteration_data(float _d)
		:delta(_d), timestep(0.f), remaning(0.f), step(0) {
	}
};

}
