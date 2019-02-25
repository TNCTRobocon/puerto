#include "settings.hpp"
#include <iostream>
namespace Boot {
using namespace std;
using namespace json11;

Setting::Setting(const json11::Json& items) {
    Deserialize(items);
}

json11::Json Setting::Serialize() const {
    std::map<string, Json> list;
    if (network) {
        list["network"] = network->Serialize();
    }
    return Json{list};
}

void Setting::Deserialize(const Json& root) {
    if (auto it = root["network"]; it.is_object()) {
        network = std::make_unique<NetWork>(it);
    } else {
        network = std::make_unique<NetWork>();
    }
    motors.clear();
    if (auto parts = root["motors"]; parts.is_object()) {
        const auto list = parts.object_items();
        for (const auto [name, value] : list) {
            if (!value.is_object()) continue;
            motors.emplace(name, std::make_shared<Motor>(value));
        }
    }
}

std::shared_ptr<Setting> Setting::Load(const std::string& path) {
    string text, error;
    // file access
    {
        ifstream reader(path);
        if (!reader.is_open()) return nullptr;
        istreambuf_iterator<char> begin(reader), end;
        text = string(begin, end);
    }
    // read as json
    Json root = Json::parse(text, error);
    if (!error.empty()) {
        cerr << error << endl;
        return nullptr;
    } else {
        return std::make_unique<Setting>(root);
    }
}

void Setting::Save(const std::string& path) {
    auto root = Serialize();
    ofstream writer(path);
    writer << root.dump();
}

NetWork::NetWork(const json11::Json& items) {
    Deserialize(items);
}

json11::Json NetWork::Serialize() const {
    return json11::Json::object{{"port", port}};
}

void NetWork::Deserialize(const json11::Json& items) {
    if (auto it = items["port"]; it.is_number()) {
        port = it.int_value();
    } else {
        port = 40000;
    }
}

Motor::Motor(const json11::Json& items) {
    Deserialize(items);
}

json11::Json Motor::Serialize() const {
    return json11::Json();
}

void Motor::Deserialize(const json11::Json& items) {
    if (auto item = items["address"]; item.is_number()) {
        address = item.int_value();
    }else{
        address = std::nullopt;
    }
}

}  // namespace Boot