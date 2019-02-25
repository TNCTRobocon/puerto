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

class ISetting {
public:
    ISetting() = default;
    ISetting(const ISetting&) = default;
    virtual ~ISetting() = default;
    virtual json11::Json Serialize() const = 0;
    virtual void Deserialize(const json11::Json&) = 0;
};

class NetWork;
class Motor;
using MotorPointer = std::shared_ptr<Motor>;

struct Setting : public ISetting {
    //フィールド
    std::unique_ptr<NetWork> network{nullptr};
    std::unordered_map<std::string, MotorPointer> motors;
    //基本メソッド
    Setting(const json11::Json& items = json11::Json());
    Setting(const Setting&) = default;
    virtual ~Setting() = default;
    virtual json11::Json Serialize() const;
    virtual void Deserialize(const json11::Json&);
    static std::shared_ptr<Setting> Load(const std::string& path);
    void Save(const std::string& path);
};

struct NetWork : public ISetting {
    int port{40000};
    NetWork(const json11::Json& items = json11::Json());
    NetWork(const NetWork&) = default;
    virtual ~NetWork() = default;
    virtual json11::Json Serialize() const;
    virtual void Deserialize(const json11::Json&);
};

struct Motor : public ISetting {
    Motor(const json11::Json& items = json11::Json());
    Motor(const Motor&) = default;
    virtual ~Motor() = default;
    virtual json11::Json Serialize() const;
    virtual void Deserialize(const json11::Json&);
};

}  // namespace Boot

#endif