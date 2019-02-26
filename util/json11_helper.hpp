#pragma once
#ifndef __UTIL_JSON11_HELPER_HEADER_GURAD__
#define __UTIL_JSON11_HELPER_HEADER_GURAD__
#include <optional>
#include <string>
#include "json11.hpp"
#include <tuple>
#include "either.hpp"
namespace json11 {

// optinalでラップする関数郡
const Json::object& get_dummy();
std::optional<Json::object> get_object(const json11::Json& items, const std::string& name);
std::optional<std::string> get_string(const json11::Json& items, const std::string& name);
std::optional<double> get_double(const json11::Json& items, const std::string& name);
std::optional<int> get_int(const json11::Json& items, const std::string& name);
std::optional<bool> get_bool(const json11::Json& items, const std::string& name);
//ToJson
template <class T>Json to_json(const std::optional<T>& x){
    return x.has_value()?Json(x.value()):Json(nullptr);
}

//読み込みと保存に使う関数
util::Either<json11::Json,std::string> load(const std::string& path);
void save(const json11::Json& items, const std::string& path);

}  // namespace json11
#endif