#pragma once
#ifndef __SETTING_HEADER_GUARD__
#define __SETTING_HEADER_GUARD__

#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <util/json11.hpp>
namespace Boot {

struct NetWork {
    int port;
    NetWork(const json11::Json& items);
    json11::Json ToJson() const;
};

struct Motor {
    std::string type;
    std::optional<int> address;
    Motor(const json11::Json& items = json11::Json());
    json11::Json ToJson()const;
};


using MotorPointer = std::shared_ptr<Motor>;

struct Setting {
    //フィールド
    NetWork network;
    std::map<std::string, MotorPointer> motors;
    //基本メソッド
    Setting(const json11::Json& items);
    Setting(const Setting&) = default;
    ~Setting() = default;
    json11::Json ToJson() const;
};



}  // namespace Boot

#endif