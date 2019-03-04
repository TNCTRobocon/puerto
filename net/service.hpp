#pragma once
#ifndef __NET_SERVICE_HAEDER_GUARD__
#define __NET_SERVICE_HAEDER_GUARD__
#include <string>
#include "server.hpp"
namespace Net {
class Service {
    Server server;

public:
    Service(const std::string& host, int port);
private:
    std::string Transfer(const std::string&);
};

}  // namespace Net

#endif