#pragma once
#ifndef __SETTING_HEADER_GUARD__
#define __SETTING_HEADER_GUARD__

#include <fstream>
#include <memory>
#include <optional>
#include <string>
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

class Setting : public ISetting {
    std::string filename;
    std::unique_ptr<NetWork> network;

public:
    Setting(const std::string& _filename);
    Setting(const Setting&) = default;
    virtual ~Setting();
    void Read();
    void Write() const;
    virtual json11::Json Serialize() const;
    virtual void Deserialize(const json11::Json&);
};

extern std::unique_ptr<Setting> settings;
static inline std::unique_ptr<Setting> CreateSetting(
    const std::string& filename) {
    return std::make_unique<Setting>(filename);
}

struct NetWork : public ISetting {
    int port{40000};
    NetWork() = default;
    NetWork(const json11::Json& items);
    NetWork(const NetWork&) = default;
    virtual ~NetWork() = default;
    virtual json11::Json Serialize() const;
    virtual void Deserialize(const json11::Json&);
};

}  // namespace Boot

#endif