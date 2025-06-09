#include "input_recorder_interface.h"
#include "input_converter_interface.h"
#include "input.h"

#include <string>
#include <fstream>

namespace dfw {

/**
 * Stores a maximum of 8 bits of input per frame. Data is stored char by char
 * in a file in binary format. In a test application we got a little upwards
 * than a minute of data with 5858 samples (5.8k / min).
 */
class input_recorder_file_8bit:
	public input_recorder_interface {

	public:

	                        input_recorder_file_8bit(const input&, const input_converter_interface&);
	                        ~input_recorder_file_8bit();

	void                    set_active(bool);
	void                    open_file(const std::string&);
	bool                    is_active() const;
	void                    record();

/**
 * after N samples the stream will be flushed and data will be stored to 
 * disk.
 */
	void                    set_flush_size(int);

	private:

	const input&            in;
	const input_converter_interface& converter;
	bool                    active;
	std::ofstream           stream;
	int                     flush_size{512},
	                        flush_count{0};
};
}
