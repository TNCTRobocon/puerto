#pragma once
#ifndef __NET_MOTORS_HEADER_GURAD__
#define __NET_MOTORS_HEADER_GUARD__
#include <util/json11.hpp>
#include "server.hpp"
namespace Net {

class MotorAdapter : public IAdapter {
public:
    MotorAdapter() = default;
    MotorAdapter(const MotorAdapter&) = default;
    virtual ~MotorAdapter() = default;
    json11::Json Apply(const json11::Json&);
};

}  // namespace Net
#endif