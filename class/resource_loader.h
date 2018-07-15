#ifndef DFW_RESOURCE_LOADER_H
#define DFW_RESOURCE_LOADER_H


#include <stdexcept>
#include <libDan2.h>
#include <class/text_reader.h>

namespace dfw
{

//!A resource loader class. Separate from the resource managers.

/**
This class loads graphical and aural resources from index files. All resources
in the index files will be loaded, so that might be a problem for very large
projects.

Index files have a specific structure: text-only, with N values separated by
tabs. Each entry must be identified with an unique integer id, that can be
mapped in the application via enums.

For all files, a line beginning with # will be a comment. The formats are:

For graphics (textures and surfaces):

index	path	trans	rtrans	gtrans	btrans

As in

1	data/img/pic.png	1	0	0	0

Where "trans" indicates transparency and r,g,b indicate the colorkey for
transparency.

For audio

index	path

As in
1	data/audio/default.ogg

When using resource managers it is a good idea to always get references to
resources directly from them instead of storing them as pointers or references:
these may become invalidated if the managers are manipulated.
*/

class resource_loader {
	public:

	//!Class constructor, receives the graphi and audio managers.
	resource_loader(ldv::resource_manager&, lda::resource_manager&);

	//!Generates textures from the vector of strings (each one a resource line)
	//!and stores them into the manager.
	void generate_textures(const std::vector<std::string>&);

	//!Generates surfaces from the vector of strings (each one a resource line)
	//!and stores them into the manager.
	void generate_surfaces(const std::vector<std::string>&);

	//!Generates sounds from the vector of strings (each one a resource line)
	//!and stores them into the manager.
	void generate_sounds(const std::vector<std::string>&);

	//!Generates music entries from the vector of strings (each one a resource line)
	//!and stores them into the manager.
	void generate_music(const std::vector<std::string>&);

	private:

	//!Processes the vector of parameters for each music line, storing the entry.
	void process_music(const std::vector<std::string>&);

	//!Processes the vector of parameters for each sound line, storing the entry.
	void process_sound(const std::vector<std::string>&);

	//!Processes the vector of parameters for each texture line, storing the entry.
	void process_texture(const std::vector<std::string>&);

	//!Processes the vector of parameters for each surface line, storing the entry.
	void process_surface(const std::vector<std::string>&);

	//!Generic routing function, as much of the logic for each type of resource looks the same.
	void process(const std::vector<std::string>& entradas, void (resource_loader::*)(const std::vector<std::string>&));

	ldv::resource_manager&	video_m;	//!< Reference to the video manager, for textures and surfaces.
	lda::resource_manager&	audio_m;	//!< Reference to the audio manager, for sounds and music.
};

}
#endif
