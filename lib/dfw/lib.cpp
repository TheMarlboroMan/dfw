#include <dfw/lib.h>
#include <sstream>
using namespace dfw;

std::string dfw::get_lib_version() {

	std::stringstream ss;
	ss<<MAJOR_VERSION<<"."<<MINOR_VERSION<<"."<<PATCH_VERSION<<"-"<<LIB_VERSION<<"-"<<RELEASE_VERSION;
	return ss.str();
}
