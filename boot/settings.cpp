#include "settings.hpp"
#include <iostream>
#include "util/json11_helper.hpp"
namespace Boot {
using namespace std;
using namespace json11;

NetWork::NetWork(const json11::Json& items) : port(get_int(items, "port").value_or(40000)) {}

Json NetWork::ToJson() const {
    return json11::Json::object{{"port", port}};
}

Motor::Motor(const json11::Json& items)
    : type(json11::get_string(items, "type").value_or("none")), address(json11::get_int(items, "address")) {}

Json Motor::ToJson() const {
    return json11::Json::object{{"type", type},{"address",to_json(address)}};
}

Setting::Setting(const json11::Json& items) : network(get_object(items, "network").value_or(get_dummy())) {
    if (auto parts = items["motors"]; parts.is_object()) {
        const auto list = parts.object_items();
        for (const auto [name, value] : list) {
            if (!value.is_object()) continue;
            motors.emplace(name, std::make_shared<Motor>(value));
        }
    }
}

Json Setting::ToJson() const {
    return Json::object{{"network", network.ToJson()}};
}


}  // namespace Boot