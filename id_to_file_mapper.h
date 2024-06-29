#pragma once

#include <string>
#include <map>
namespace d2d { namespace storage {

/**
 * many resources can be defined in a file that maps a numeric identifier
 * to a file path. This class just streamlines this process.
 */
class id_to_path_mapper {

	public:

	                        id_to_path_mapper(const std::string&);

/**
 * returns true if a path is stored in the given id.
 */
	bool                    has(int) const;
/**
 * returns the partial path stored in the given id. Partial means that it
 * will be as it appears in the file, undecorated by any environment types.
 */
	const std::string&      partial(int) const;
/**
 * returns the path for the given id, directly usable for the application.
 */
	const std::string       get(int) const;
};

	private:

	std::map<int, std::string>  data;
}}
