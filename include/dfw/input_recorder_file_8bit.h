#include "input_recorder_interface.h"
#include "input_converter_interface.h"
#include "input.h"

#include <string>
#include <fstream>

namespace dfw {

class input_recorder_file_8bit:
	public input_recorder_interface {

	public:

	                        input_recorder_file_8bit(const input&, const input_converter_interface&);
	                        ~input_recorder_file_8bit();

	void                    set_active(bool);
	void                    open_file(const std::string&);

	bool                    is_active() const;
	void                    record();

	private:

	const input&            in;
	const input_converter_interface& converter;
	bool                    active;
	std::ofstream           stream;
};
}
