#include "kernel.h"
#include <cstdlib>

using namespace dfw;

kernel::kernel(tools::log& rlog, tools::arg_manager& carg)
	:delta_step(0.01f), log_i(rlog), audiocontroller(nullptr),
	fps_counter_i(), screen_i(), input_i(sdlinput),
	audio_i(nullptr),
	arg_manager_i(carg) {

	log_i<<"Kernel is constructed."<<std::endl;
}


void kernel::init_video_system(const window_info& iv) {
	screen_i.init(iv.px_w, iv.px_h);
	screen_i.set_logical_size(iv.logical_w, iv.logical_h);
	screen_i.set_title(iv.title);
	ldv::set_cursor_visible(iv.show_cursor);
	ldv::set_vsync(iv.vsync);
}

/** All this work with pointers is expected: these objects need references to
things we can't initialize as references until we know the configuration values.
*/ 

void kernel::init_audio_system(const audio_info& _ai)
{
	lda::audio_controller_config cfg{_ai.ratio,
					_ai.out, 
					_ai.buffers,
					_ai.channels,
					_ai.sound_volume,
					_ai.music_volume,
					AUDIO_S16SYS};

	audiocontroller.reset(new lda::audio_controller(cfg));
	audiocontroller->set_main_sound_volume(_ai.sound_volume);
	audiocontroller->set_main_music_volume(_ai.music_volume);

	audio_i.reset(new audio(*audiocontroller));
}

void kernel::init_input_system(const std::vector<dfw::input_pair>& _iv) {

	input_i.configure(_iv); 
}
