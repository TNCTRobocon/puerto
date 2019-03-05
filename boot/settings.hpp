#pragma once
#ifndef __BOOT_SETTING_HEADER_GUARD__
#define __BOOT_SETTING_HEADER_GUARD__

#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <json11/json11.hpp>
namespace Boot {

struct NetWork {
    int port;
    NetWork(const json11::Json& items);
    json11::Json ToJson() const;
};
json11::Json to_json(const NetWork&);

struct Motor {
    std::string type{"none"};
    std::optional<int> address;
    Motor()=default;
    Motor(const json11::Json& items);
    json11::Json ToJson()const;
};
json11::Json to_json(const Motor&);

struct Setting {
    //フィールド
    NetWork network;
    std::map<std::string, Motor> motors;
    //基本メソッド
    Setting(const json11::Json& items);
    Setting(const Setting&) = default;
    ~Setting() = default;
};
 json11::Json to_json(const Setting&) ;


}  // namespace Boot

#endif