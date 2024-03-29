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

