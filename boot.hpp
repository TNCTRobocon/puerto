#pragma once
#ifndef __BOOT_HEAER_GUARD__
#define __BOOT_HEADR_GUARD__

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

namespace Boot {

class BootFlagBase {
public:
    const std::string usage;
    BootFlagBase(const std::string& _usage = "") : usage(_usage) {}
    BootFlagBase(const BootFlagBase&) = default;
    virtual ~BootFlagBase() = default;
    virtual void Set(const std::string&) {}
    virtual std::string GetUsage() const { return usage; }
};

class BootFlagString : public BootFlagBase {
    std::string& value;

public:
    BootFlagString(std::string& _value, const std::string& _usage)
        : BootFlagBase(_usage), value(_value) {}
    virtual void Set(const std::string& inst) { value = inst; }
};

class BootFlagInt : public BootFlagBase {
    int& value;

public:
    BootFlagInt(int& _value, const std::string& _usage)
        : BootFlagBase(_usage), value(_value) {}
    virtual void Set(const std::string& inst){
        value = std::stoi(inst);
    }
};

class BootFlagBool : public BootFlagBase {
    bool& value;

public:
    BootFlagBool(bool& _value, const std::string& _usage)
        : BootFlagBase(_usage), value(_value) {}
    virtual void Set(const std::string& inst);
    void Set(bool x){
        value=x;
    }
};

class BootFlags {
    mutable std::unordered_map<std::string, BootFlagBase*> flags;

public:
    BootFlags() = default;
    BootFlags(const BootFlags&) = delete;
    virtual ~BootFlags();
    // Add
    void Add(const std::string& key,
             std::string& value,
             const std::string& usage) {
        flags.emplace(key, new BootFlagString(value, usage));
    }
    void Add(const std::string& key, bool& value, const std::string& usage) {
        flags.emplace(key, new BootFlagBool(value, usage));
    }
    void Add(const std::string& key, int& value, const std::string& usage) {
        flags.emplace(key, new BootFlagInt(value, usage));
    }
    // Parse
    void Parse(int argc, char** argv);
};

}  // namespace Boot
#endif