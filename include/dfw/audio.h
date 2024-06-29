#pragma once

#include <lda/audio_controller.h>
#include <lda/music.h>
#include <lda/sound.h>

namespace dfw
{

//!This class acts as a proxy for the libdansdl2 audio component. It also
//!provides some utilities to handle paused sounds.

class audio
{
	public:

	//!Class constructor. Must be injected with the libdansdl2 audio controller.
								audio(lda::audio_controller&);

	//!Returns the internal libdansdl2 audio controller instance.
	lda::audio_controller& 		operator()() {
		return ac;
	}

	//!Proxy call to libdansdl2 audio controllers' pause_sound.
	void 			pause() {
		ac.pause_sound();
	}

	//!Pauses active channels, saving them to an internal index. This index
	//!can be used later to restore the playing of these channels. There can
	//!be only one index active, so the application must supply any additional
	//!logic.
	void 			pause_active();

	//!Resumes the playing of all paused sounds and clears the index of channels
	//!that would be restored.
	void 			resume();

	//!Resumes the playing of paused sounds, as stored by a call to
	//!"pause_active". The index of paused channels is emptied.
	void 			resume_active();

	//!Resumes the playing of all sounds except those stored in the index by
	//!a call to pause_active. The index remains intact.
	void 			resume_inactive();

	//!Proxy call to libdansdl2 audio controller's play_sound.
	void			play_sound(lda::sound&);
	//!Proxy call to libdansdl2 audio controller's play_sound.
	void			play_sound(const lda::sound_struct&);
	//!Proxy call to libdansdl2 audio controller's play_music.
	void			play_music(const lda::music_struct&);
	//!Proxy call to libdansdl2 audio controller's stop_music
	void            stop_music(int _fade=0) {ac.stop_music(_fade);}
	//!Proxy call to libdansdl2 audio controller's pause_music
	void            pause_music() {ac.pause_music();}
	//!Proxy call to libdansdl2 audio controller's resume_music
	void            resume_music() {ac.resume_music();}

	private:

	lda::audio_controller&			ac;					//!< Reference to the audio controller.
	std::vector<unsigned int> 		paused_channels;	//!< Internal index of paused channels.
};

}
