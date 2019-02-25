#include "json11_helper.hpp"

namespace json11 {

std::optional<Json> get_object(const json11::Json& items, const std::string& name){
        if (auto it = items[name]; it.is_object()) {
        return it.object_items();
    } else {
        return std::nullopt;
    }
}

std::optional<std::string> get_string(const json11::Json& items,
                                      const std::string& name) {
    if (auto it = items[name]; it.is_string()) {
        return it.string_value();
    } else {
        return std::nullopt;
    }
}

std::optional<double> get_double(const json11::Json& items,
                                 const std::string& name) {
    if (auto it = items[name]; it.is_number()) {
        return it.number_value();
    } else {
        return std::nullopt;
    }
}

std::optional<int> has(const json11::Json& items, const std::string& name) {
    if (auto it = items[name]; it.is_number()) {
        return it.int_value();
    } else {
        return std::nullopt;
    }
}

std::optional<bool> get_bool(const json11::Json& items,
                             const std::string& name) {
    if (auto it = items[name]; it.is_bool()) {
        return it.bool_value();
    } else {
        return std::nullopt;
    }
}
}  // namespace json11