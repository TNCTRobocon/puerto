#include "settings.hpp"

#include <iostream>
#include <json11/json11_helper.hpp>
namespace Boot {
using namespace std;
using namespace json11;

NetWork::NetWork(const json11::Json& items)
    : host(get_string(items, "host").value_or("*")), port(get_int(items, "port").value_or(40000)) {}

json11::Json NetWork::toJson() const {
    return Json::object{{"host", host}, {"port", port}};
}

Motor::Motor(const json11::Json& items)
    : type(get_string(items, "type").value_or("none")), address(get_int(items, "address")) {}

json11::Json Motor::toJson() const {
    return json11::Json::object{{"type", type}, {"address", json11::to_json(address)}};
}

Setting::Setting(const json11::Json& items)
    : network(get_object(items, "network").value_or(get_dummy())) {
    if (auto it = items["motors"]; it.is_object()) {
        const auto& objects = it.object_items();
        for (const auto& [name, object] : objects) {
            motors.insert_or_assign(name, Motor(object));
        }
    } else {
        // dummyを作成
        Motor mtr;
        mtr.address = 0;
        mtr.type = "demo";
        motors["demo"] = mtr;
    }
}
json11::Json Setting::toJson() const {
    return Json::object{{"network", network.toJson()}};
}

}  // namespace Boot