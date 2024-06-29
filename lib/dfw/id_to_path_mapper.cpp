#include <dfw/id_to_path_mapper.h>
#include <fstream>
#include <stdexcept>
#include <sstream>

using namespace dfw;

id_to_path_mapper::id_to_path_mapper(
	const std::string& _filename
) {

	std::ifstream file(_filename);

	if(!file.is_open()) {

		std::stringstream ss;
		ss<<"could not open id_to_path_mapper file "<<_filename;
		throw std::runtime_error(ss.str());
	}

	int id{0};
	std::string value;

	while(true) {

		file>>id>>value;

		if(file.eof()) {

			break;
		}

		if(file.fail()) {

			std::stringstream ss;
			ss<<"bad contents for id_to_path_mapper_file "<<_filename;
			throw std::runtime_error(ss.str());
		}

		data.insert({id, value});
	}
}

bool id_to_path_mapper::has(
	int _id
) const {

	return data.count(_id);
}

const std::string& id_to_path_mapper::get(
	int _id
) const {

	return data.at(_id);
}
