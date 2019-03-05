#include "service.hpp"
#include <functional>
#include <json11/json11.hpp>
//#include <utils/json11_helper.hpp>
namespace Net {
Service::Service(const std::string& host, int port)
    : server(std::bind(&Service::Transfer, this, std::placeholders::_1), host, port) {}

std::string Transfer(const std::string& requestText) {
    std::string error;
    auto requestJson = json11::Json::parse(requestText,error);
    if (error.empty()){
        return "";
    }
    auto responseJson=requestJson;//TODO 正しくパースする
    return responseJson.dump();
    
}
}  // namespace Net