#include <dfw/kernel.h>

#include <lm/log.h>

#include <cstdlib>

using namespace dfw;

kernel::kernel(lm::logger& rlog, tools::arg_manager& carg)
	:delta_step(0.01f), log_i(rlog), audiocontroller(nullptr),
	fps_counter_i(), screen_i(nullptr), input_i(sdlinput),
	audio_i(nullptr),
	arg_manager_i(carg) {

	lm::log(rlog).info()<<"Kernel is constructed."<<std::endl;
}


void kernel::init_video_system(const window_info& iv) {

	screen_i.reset(new ldv::screen(iv.px_w, iv.px_h));
	screen_i->set_logical_size(iv.logical_w, iv.logical_h);
	screen_i->set_title(iv.title);
	ldv::set_cursor_visible(iv.show_cursor);
	ldv::set_vsync(iv.vsync);
	//TODO: What about the fullscreen thing????

	lm::log(log_i).info()<<"kernel built the video system with a "<<iv.px_w<<"x"<<iv.px_h<<" window (logical size of "<<iv.logical_w<<"x"<<iv.logical_h<<")"<<std::endl;
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

	lm::log(log_i).info()<<"kernel built the audio system with "<<_ai.buffers<<" buffers and "<<_ai.channels<<" channels"<<std::endl;
}

void kernel::init_input_system(const std::vector<dfw::input_pair>& _iv) {

	input_i.configure(_iv);

	lm::log(log_i).info()<<"kernel configured the input system."<<std::endl;
}
