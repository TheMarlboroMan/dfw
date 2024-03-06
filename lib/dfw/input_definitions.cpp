#include "dfw/input_definitions.h"
#include <string>

using namespace dfw;

std::vector<dfw::input_description> dfw::input_description_from_config_token(
	const rapidjson::Value& tok
) {
	if(!tok.IsArray()) {
		throw std::runtime_error("app::input_description_from_config_token, token is not array");
	}

	std::vector<std::string> values{"type", "device", "code"};
	std::vector<dfw::input_description> results{};

	for(const auto& node : tok.GetArray()) {

		if(!node.IsObject()) {

			throw std::runtime_error("app::input_description_from_config_token, expected an object in the input description");
		}

		for(const auto& val : values) {

			if(!node.HasMember(val.c_str())) {

				throw std::runtime_error("app::input_description_from_config_token, missing type-device-code");
			}
		}

		results.push_back(
			{
				input_description_type_from_int(node["type"].GetInt()),
				node["code"].GetInt(),
				node["device"].GetInt()
			}
		);
	};

	return results;
}

rapidjson::Value dfw::config_token_from_input_description(
	const dfw::input_description& _id,
	tools::json_config_file& _config
) {

	rapidjson::Value result(rapidjson::kObjectType);
	_config.add_to_object(result, "type", input_description_int_from_type(_id.type));
	_config.add_to_object(result, "device", _id.device);
	_config.add_to_object(result, "code", _id.code);

	return result;
}

dfw::input_description::types dfw::input_description_type_from_int(
	int v
) {

	switch(v) {
		case 0: return dfw::input_description::types::keyboard; break;
		case 1: return dfw::input_description::types::mouse; break;
		case 2: return dfw::input_description::types::joystick; break;
	}

	return dfw::input_description::types::none;
}

int dfw::input_description_int_from_type(
	dfw::input_description::types t
) {

	switch(t) {
		case dfw::input_description::types::keyboard: 	return 0; break;
		case dfw::input_description::types::mouse: 	return 1; break;
		case dfw::input_description::types::joystick: 	return 2; break;
		case dfw::input_description::types::none: 	return 3; break;
	}

	return 3;
}

