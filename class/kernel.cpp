#include "kernel.h"
#include <cstdlib>

using namespace dfw;

kernel::kernel(ldt::log& rlog, tools::arg_manager& carg)
	:delta_step(0.01), log_i(rlog), audiocontroller(nullptr),
	fps_counter_i(), screen_i(), input_i(sdlinput),
	audio_i(nullptr),
	arg_manager_i(carg)
{
	log_i<<"Kernel is constructed"<<std::endl;
}

void kernel::init(const kernel_config_interface& kdi, const base_config& config)
{
	log_i<<"Kernel inits video environment..."<<std::endl;
	init_video_environment(kdi.get_window_info());

	log_i<<"Kernel inits audio environment..."<<std::endl;
	init_audio_environment(config);

	//Inicializar recursos.
	log_i<<"Kernel inits resources..."<<std::endl;
	resource_loader r_loader(v_manager, a_manager);
	r_loader.generate_textures(kdi.get_texture_entries());
	r_loader.generate_surfaces(kdi.get_surface_entries(), screen_i);
	r_loader.generate_sounds(kdi.get_sound_entries());
	r_loader.generate_music(kdi.get_music_entries());
	
	//Inicializar controles.
	log_i<<"Kernel inits controls..."<<std::endl;
	input_i.configure(kdi.get_input_pairs()); 

	log_i<<"Kernel is ready..."<<std::endl;
}

//Inicialización con valores hardcodeados.
void kernel::init_video_environment(const window_info& iv)
{
	screen_i.init(iv.px_w, iv.px_h);
	screen_i.set_logical_size(iv.logical_w, iv.logical_h);
	screen_i.set_title(iv.title);
	ldv::set_cursor_visible(iv.show_cursor);
	ldv::set_vsync(iv.vsync);
}

/** All this work with pointers is expected: these objects need references to
things we can't initialize as references until we know the configuration values.
*/ 

void kernel::init_audio_environment(const base_config& config)
{
	lda::audio_controller_config cfg{config.get_audio_ratio(),
					config.get_audio_out(),
					config.get_audio_buffers(),
					config.get_audio_channels(),
					config.get_audio_volume(),
					config.get_music_volume(),
					AUDIO_S16SYS};

	audiocontroller.reset(new lda::audio_controller(cfg));
	audiocontroller->set_main_sound_volume(config.get_audio_volume());
	audiocontroller->set_main_music_volume(config.get_music_volume());

	audio_i.reset(new audio(*audiocontroller));
}
