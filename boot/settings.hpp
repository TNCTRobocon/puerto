#pragma once
#ifndef __BOOT_SETTING_HEADER_GUARD__
#define __BOOT_SETTING_HEADER_GUARD__

#include <fstream>
#include <json11/json11.hpp>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
namespace Boot {

struct NetWork {
    std::string host;
    int port;
    NetWork(const json11::Json& items);
    json11::Json toJson() const;
};

struct Motor {
    std::string type{"none"};
    std::optional<int> address;
    Motor() = default;
    Motor(const json11::Json& items);
    json11::Json toJson() const;
};

struct Setting {
    //フィールド
    NetWork network;
    std::map<std::string, Motor> motors;
    //基本メソッド
    Setting(const json11::Json& items);
    Setting(const Setting&) = default;
    ~Setting() = default;
    json11::Json toJson()const;
};

}  // namespace Boot

#endif