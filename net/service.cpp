#include "service.hpp"
#include <functional>
#include <json11/json11.hpp>
//#include <utils/json11_helper.hpp>
namespace Net {
Service::Service(const std::string& host, int port)
    : server(std::bind(&Service::Transfer, this, std::placeholders::_1), host, port) {}

std::string Transfer(const std::string& text) {

return "";//TODO



}
}  // namespace Net