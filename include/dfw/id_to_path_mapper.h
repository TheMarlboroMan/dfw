#pragma once

#include <string>
#include <map>
namespace dfw {

/**
 * many resources can be defined in a file that maps a numeric identifier
 * to a file path. This class just streamlines this process.
 */
class id_to_path_mapper {

	public:

/**
 * creates a mapper from the filename. The file is expected to contain an 
 * entry per line, with the first character begin a number followed by any
 * whitespace and the value. Any other whitespace will cause an exception
 * to be thrown.
 */
	                        id_to_path_mapper(const std::string&);

/**
 * returns true if a path is stored in the given id.
 */
	bool                    has(int) const;
/**
 * returns the value for the given id, undefined if the index does not exist.
 */
	const std::string&      get(int) const;

	private:

	std::map<int, std::string>  data;
};
}
