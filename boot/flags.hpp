#pragma once
#ifndef __BOOT_HEAER_GUARD__
#define __BOOT_HEADR_GUARD__

#include <memory>
#include <string>
#include <unordered_map>

namespace Boot {

class FlagBase {
public:
    const std::string usage;
    FlagBase(const std::string& _usage = "") : usage(_usage) {}
    FlagBase(const FlagBase&) = default;
    virtual ~FlagBase() = default;
    virtual void Set(const std::string&) {}
    virtual std::string GetUsage() const { return usage; }
};

class FlagString : public FlagBase {
    std::string& value;
public:
    FlagString(std::string& _value, const std::string& _usage)
        : FlagBase(_usage), value(_value) {}
    virtual void Set(const std::string& inst) { value = inst; }
};

class FlagInt : public FlagBase {
    int& value;

public:
    
FlagInt(int& _value, const std::string& _usage)
        : FlagBase(_usage), value(_value) {}
    virtual void Set(const std::string& inst){
        value = std::stoi(inst);
    }
};

class FlagBool : public FlagBase {
    bool& value;

public:
    FlagBool(bool& _value, const std::string& _usage)
        : FlagBase(_usage), value(_value) {}
    virtual void Set(const std::string& inst);
    void Set(bool x){
        value=x;
    }
};

class Flags {
    mutable std::unordered_map<std::string, FlagBase*> flags;

public:
    Flags() = default;
    Flags(const Flags&) = delete;
    virtual ~Flags();
    // Add
    void Add(const std::string& key,
             std::string& value,
             const std::string& usage) {
        flags.emplace(key, new FlagString(value, usage));
    }
    void Add(const std::string& key, bool& value, const std::string& usage) {
        flags.emplace(key, new FlagBool(value, usage));
    }
    void Add(const std::string& key, int& value, const std::string& usage) {
        flags.emplace(key, new 
    FlagInt(value, usage));
    }
    // Parse
    void Parse(int argc, char** argv);
};




}  // namespace Boot
#endif