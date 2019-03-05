#pragma once
#ifndef __UTIL_JSON11_HELPER_HEADER_GURAD__
#define __UTIL_JSON11_HELPER_HEADER_GURAD__
#include <functional>
#include <optional>
#include <string>
#include <tuple>
#include <type_traits>
#include "either.hpp"
#include "json11.hpp"
#include <memory>
namespace json11 {

class IToJson {
public:
    virtual json11::Json toJson() const = 0;
};

// optinalでラップする関数郡
const json11::Json::object& get_dummy();
std::optional<json11::Json::object> get_object(const json11::Json& items, const std::string& name);
std::optional<std::string> get_string(const json11::Json& items, const std::string& name);
std::optional<double> get_double(const json11::Json& items, const std::string& name);
std::optional<int> get_int(const json11::Json& items, const std::string& name);
std::optional<bool> get_bool(const json11::Json& items, const std::string& name);
// ToJson
template <class T>
json11::Json to_json(const T& x) {
    return json11::Json(x);
}

template <class T>
json11::Json to_json(const std::optional<T>& x) {
    return x.has_value() ? to_json(x.value()) : json11::Json(nullptr);
}
// Map型から生成する
template <class T>
json11::Json to_json(const std::map<std::string, T> list) {
    json11::Json::object result;
    for (const auto& [name, obj] : list) {
        result[name] = to_json(obj);
    }
    return result;
}

//読み込みと保存に使う関数
util::Either<json11::Json, std::string> load(const std::string& path);
void save(const json11::Json& items, const std::string& path);

}  // namespace json11
#endif