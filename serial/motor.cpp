#include "motor.hpp"
#include <boost/format.hpp>
#include <sstream>
#include <string>
namespace Serial {

bool SimpleMotor::Duty(double rate) {
    auto request = (boost::format("duty %1%") % rate).str();
    auto response = port->CreateSession(address)->Transfer(request);
    return response.has_value() ? response.value() == "ok" : false;
}

bool MultiMotor::Duty(double rate) {
    auto request = (boost::format("#%1%duty %2%") % id % rate).str();
    auto response = port->CreateSession(address)->Transfer(request);
    return response.has_value() ? response.value() == "ok" : false;
}

}  // namespace Serial