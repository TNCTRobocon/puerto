#pragma once
#ifndef __UTIL_JSON11_HELPER_HEADER_GURAD__
#define __UTIL_JSON11_HELPER_HEADER_GURAD__
#include <optional>
#include "json11.hpp"

namespace json11 {


//optinalでラップする関数郡
std::optional<Json> get_object(const json11::Json& items, const std::string& name);
std::optional<std::string> get_string(const json11::Json& items, const std::string& name);
std::optional<double> get_double(const json11::Json& items, const std::string& name);
std::optional<int> get_int(const json11::Json& items, const std::string& name);
std::optional<bool> get_bool(const json11::Json& items, const std::string& name);

}
#endif