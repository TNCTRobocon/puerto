#pragma once
#ifndef __NET_SERVICE_HAEDER_GUARD__
#define __NET_SERVICE_HAEDER_GUARD__
#include <json11/json11.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include "server.hpp"
namespace Net {

class IAdapter {
public:
    virtual json11::Json Excute(const json11::Json&) = 0;
};

class Service {
    Server server;
    std::unordered_map<std::string, std::unique_ptr<IAdapter>> table;

public:
    Service(const std::string& host, int port);

private:
    std::string Transfer(const std::string&);
};

class MotorAdapter : public IAdapter {
public:
    virtual json11::Json Excute(const json11::Json&) ;
};

}  // namespace Net

#endif