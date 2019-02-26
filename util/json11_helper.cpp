#include "json11_helper.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
namespace json11 {

std::optional<Json::object> get_object(const json11::Json& items, const std::string& name) {
    if (auto it = items[name]; it.is_object()) {
        return it.object_items();
    } else {
        return std::nullopt;
    }
}

std::optional<std::string> get_string(const json11::Json& items, const std::string& name) {
    if (auto it = items[name]; it.is_string()) {
        return it.string_value();
    } else {
        return std::nullopt;
    }
}

std::optional<double> get_double(const json11::Json& items, const std::string& name) {
    if (auto it = items[name]; it.is_number()) {
        return it.number_value();
    } else {
        return std::nullopt;
    }
}

std::optional<int> get_int(const json11::Json& items, const std::string& name) {
    if (auto it = items[name]; it.is_number()) {
        return it.int_value();
    } else {
        return std::nullopt;
    }
}

std::optional<bool> get_bool(const json11::Json& items, const std::string& name) {
    if (auto it = items[name]; it.is_bool()) {
        return it.bool_value();
    } else {
        return std::nullopt;
    }
}

util::Either<json11::Json, std::string> Load(const std::string& path) {
    using namespace std;
    string text, error;
    // file access
    {
        ifstream reader(path);
        if (!reader.is_open()) {
            stringstream ss;
            ss << "[Error]" << path << "can not open!!" << endl;
            return ss.str();
        };
        istreambuf_iterator<char> begin(reader), end;
        text = string(begin, end);
    }
    // read as json
    Json items = Json::parse(text, error);
    if (!error.empty()) {
        return error;
    } else {
        return items;
    }
}

void Save(const json11::Json& items, const std::string& path) {
    using namespace std;
    string dump = items.dump();
    ofstream writer(path);
    writer << dump;
}

}  // namespace json11