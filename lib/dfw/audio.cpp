#include <dfw/audio.h>

#include <algorithm>

using namespace dfw;

audio::audio(lda::audio_controller& a)
	:ac(a) {

}

void audio::pause_active() {

	unsigned int canales=ac.get_requested_channels();
	unsigned int i=0;

	while(i < canales) {
		auto c=ac.get_channel(i);
		if(c.is_playing()) {
			c.pause();
			paused_channels.push_back(i);
		}
		++i;
	}
}

void audio::resume() {
	ac.resume_sound();
	paused_channels.clear();
}

void audio::resume_active() {
	for(unsigned int i : paused_channels) {
		ac.get_channel(i).resume();
	}

	paused_channels.clear();
}

void audio::resume_inactive() {

	unsigned int canales=ac.get_requested_channels();
	unsigned int i=0;

	if(!paused_channels.size()) {
		ac.resume_sound();
	}
	else while(i < canales) {
		if(std::find(paused_channels.begin(),
				paused_channels.end(),
				i)==paused_channels.end()) 	{
			ac.get_channel(i).resume();
		}
		++i;
	}
}

void audio::play_sound(const lda::sound_struct& ss) {
	ac.play_sound(ss);
}

void audio::play_sound(lda::sound& s) {
	play_sound(lda::sound_struct(s));
}

void audio::play_music(const lda::music_struct& m) {
	ac.play_music(m);
}
