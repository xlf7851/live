#pragma once
#include <json.h>

namespace json_utl
{
	int json_to_int(Json::Value& js, int default = 0);
	std::string json_to_string(Json::Value& js, LPCSTR default = "");
}