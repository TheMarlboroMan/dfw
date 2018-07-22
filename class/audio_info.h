#ifndef DFRAMEWORK_AUDIO_INFO
#define DFRAMEWORK_AUDIO_INFO

namespace dfw {

//!Structure with audio information.

struct audio_info {

	int		ratio,
			out,
			buffers,
			channels,
			sound_volume,
			music_volume;
};


}

#endif


