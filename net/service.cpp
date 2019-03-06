#include "service.hpp"
#include <functional>
#include <iostream>
#include <json11/json11.hpp>
//#include <utils/json11_helper.hpp>
namespace Net {
Service::Service(const std::string& host, int port)
    : server(std::bind(&Service::Transfer, this, std::placeholders::_1), host, port) {
    table["motors"] = std::make_unique<MotorAdapter>();
}

std::string Service::Transfer(const std::string& requestText) {
    std::string error;
    auto requestJson = json11::Json::parse(requestText, error);
    if (!error.empty()) {
        std::cout<<error<<std::endl;
        json11::Json responseJson = json11::Json::object{{"error", "parse"}};
        return responseJson.dump();
    }

    json11::Json::object responseObjects;
    for (auto [key, object] : requestJson.object_items()) {
        if (auto it = table.find(key); it != table.end()) {
            responseObjects[key] = it->second->Excute(object);
        }
    }
    json11::Json responseJson{responseObjects};
    return responseJson.dump();
}

json11::Json MotorAdapter::Excute(const json11::Json& request) {
    for (const auto& [label, single] : request.object_items()) {
        //命名規則"#*
        if (label[0] != '#') {
            continue;
        }
        std::string name = label.substr(1);
        //ダミー実装
        if (auto duty = single["duty"]; duty.is_number()) {
            std::cout << "duty"
                      << "[" << name << "]" << duty.number_value() << std::endl;
        }
    }

    return json11::Json::object{};
}

}  // namespace Net