#include <dfw/base_config.h>
#include <tools/string_utils.h>

#include <map>

/*
Al crear el objeto aún no podemos cargar la información de los valores por
defecto (métodos virtuales) así que no podremos intentar acceder a ningún
valor hasta haber cargado la configuración.
*/

using namespace dfw;

base_config::base_config(
	const std::string& ppath
)
try
	:json_config_file(ppath) {
}
catch(std::runtime_error& e) {
	throw std::runtime_error(std::string("error starting configuration ")+ppath+" : "+e.what());
}

void base_config::set_control_data(
	const std::string& _config_map_key,
	input_description::types _type,
	int _code,
	int _device
) {

	using jsonval=rapidjson::Value;
	jsonval jsonnode{rapidjson::kObjectType};

	int type=dfw::input_description_int_from_type(_type);
	add_to_object(jsonnode, "type", type);
	add_to_object(jsonnode, "device", _device);
	add_to_object(jsonnode, "code", _code);

	jsonval data_list{rapidjson::kArrayType};
	add_to_vector(data_list, jsonnode);

	set_vector(_config_map_key, data_list);
}
