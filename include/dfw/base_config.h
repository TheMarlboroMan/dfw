#pragma once

#include <tools/json_config_file.h>

#include <string>

namespace dfw
{


//!Base application config.

//!Based on "dnot_config_file", reads, stores and retrieve application
//!configuration specifics from a file. For maximum usability and legibility
//!must be extended by each project with the necessary setters and getters.
//!For those methods that need to be provided by the application class,
//!paths must respect the "dnot_config_file" conventions, like "config:meta:v".
//!Is is assumed that all applications will provide at least the configuration
//!options present on this class.

class base_config:
	public tools::json_config_file
{
	public:
	//!Class constructor. Receives the path to the config file. Will throw
	//!if the file does not exist.
							base_config(const std::string& ruta);

	//!Returns the file version number. Can be used if an application has a
	//!long lifespan and new configuration values are added over time, so
	//!checks can be performed.
	int 					get_file_version() const		{return token_from_path(get_key_file_version()).GetInt();}

	//!Returns the double buffering configuration value.
	bool 					get_screen_double_buffer() const 	{return token_from_path(get_key_screen_double_buffer()).GetBool();}

	//!Returns the vsync configuration value.
	bool 					get_screen_vsync() const 	{return token_from_path(get_key_screen_vsync()).GetBool();}

	//!Returns the audio volume configuration value.
	int 					get_audio_volume() const		{return token_from_path(get_key_sound_volume()).GetInt();}

	//!Returns the music volume configuration value.
	int 					get_music_volume() const		{return token_from_path(get_key_music_volume()).GetInt();}

	//!Returns the audio ratio configuration value, as expressed by SDL audio.
	int 					get_audio_ratio() const		{return token_from_path(get_key_audio_ratio()).GetInt();}

	//!Returns the audio out configuration value (usually 2, left and right).
	int 					get_audio_out() const		{return token_from_path(get_key_audio_out()).GetInt();}

	//!Returns the audio buffer configuration value. 1024 is a sane default.
	int 					get_audio_buffers() const		{return token_from_path(get_key_audio_buffers()).GetInt();}

	//!Returns the number of audio channels.
	int 					get_audio_channels() const		{return token_from_path(get_key_audio_channels()).GetInt();}

	//!Sets the double buffering value.
	void 					set_screen_double_buffer(bool v)	{set(get_key_screen_double_buffer(), v);}

	//!Sets the double vsync value.
	void 					set_screen_vsync(bool v)	{set(get_key_screen_vsync(), v);}

	//!Sets the audio volume value.
	void 					set_audio_volume(int v)		{set(get_key_sound_volume(), v);}

	//!Sets the music volume value.
	void 					set_music_volume(int v)		{set(get_key_music_volume(), v);}

	//!Sets the audio ratio value.
	void 					set_audio_ratio(int v)		{set(get_key_audio_ratio(), v);}

	//!Sets the audio outputs value.
	void 					set_audio_out(int v)		{set(get_key_audio_out(), v);}

	//!Sets the audio buffer size value.
	void 					set_audio_buffers(int v)		{set(get_key_audio_buffers(), v);}

	//!Sets the audio channels value.
	void 					set_audio_channels(int v)		{set(get_key_audio_channels(), v);}

	protected:

	//!Must be supplied by the application returning the current version string.
	virtual std::string 				generate_file_version() const=0;

	//!Must be supplied by the application with the path to the file version value.
	virtual std::string 				get_key_file_version() const=0;

	//!Must be supplied by the application with the path to the double buffering value.
	virtual std::string 				get_key_screen_double_buffer() const=0;

	//!Must be supplied by the application with the path to the screen vsync value.
	virtual std::string 				get_key_screen_vsync() const=0;

	//!Must be supplied by the application with the path to the sound volume value.
	virtual std::string 				get_key_sound_volume() const=0;

	//!Must be supplied by the application with the path to the music volume value.
	virtual std::string 				get_key_music_volume() const=0;

	//!Must be supplied by the application with the path to the audio ratio value.
	virtual std::string 				get_key_audio_ratio() const=0;

	//!Must be supplied by the application with the path to the audio buffers value.
	virtual std::string 				get_key_audio_buffers() const=0;

	//!Must be supplied by the application with the path to the audio out value.
	virtual std::string 				get_key_audio_out() const=0;

	//!Must be supplied by the application with the path to the audio channels value.
	virtual std::string 				get_key_audio_channels() const=0;
};
}
