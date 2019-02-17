#pragma once
#ifndef __MOTOR_HEADER_GUARD__
#define __MOTOR_HEADER_GURAD__

#include <memory>
#include "port.hpp"

namespace Serial {

class SimpleMotor {
    const SerialPortPointer port;
    const unsigned int address;

public:
    SimpleMotor(SerialPortPointer _port, unsigned int _address)
        : port(_port), address(_address) {}
    SimpleMotor(const SimpleMotor&) = delete;
    virtual ~SimpleMotor() = default;
    virtual bool Duty(double rate);
};

class MultiMotor {
    const SerialPortPointer port;
    const unsigned int address;
    const unsigned int id;

public:
    MultiMotor(SerialPortPointer _port, unsigned int _address, unsigned _id)
        : port(_port), address(_address), id(_id) {}
    MultiMotor(const MultiMotor&) = delete;
    virtual ~MultiMotor() = default;
    virtual bool Duty(double rate);
};

}  // namespace Serial

#endif