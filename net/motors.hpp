#pragma once
#ifndef __NET_MOTORS_HEADER_GURAD__
#define __NET_MOTORS_HEADER_GUARD__
#include <util/json11.hpp>

namespace Net{

class MotorAdapter{
    
    json11::Json Apply(const json11::Json&);

};

}
#endif