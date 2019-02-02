#pragma once
#ifndef __BOOT_HEAER_GUARD__
#define __BOOT_HEADR_GUARD__
#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
struct OptionBase {
    const std::string usage{""};
    OptionBase(const std::string& _usage) : usage(_usage) {}
    OptionBase(const OptionBase&) = default;
    virtual ~OptionBase() = default;

    virtual void Parse(const std::string&) = 0;
};

struct OptionString : public OptionBase {
    std::string& value;
    OptionString(std::string& _value, const std::string& _usage)
        : OptionBase(_usage), value(_value) {}
    OptionString(const OptionString&) = default;
    virtual ~OptionString() = default;
    void Parse(const std::string& text) { value = text; }
};

struct OptionInt : public OptionBase {
    int& value;
    OptionInt(int& _value, const std::string& _usage)
        : OptionBase(_usage), value(_value) {}
    OptionInt(const OptionInt&) = default;
    virtual ~OptionInt() = default;
    void Parse(const std::string& text) { value = std::stoi(text); }
};

class OptionBool : public OptionBase {
    static inline std::string low(const std::string& text) {
        std::string l("", text.size());
        std::transform(text.begin(), text.end(), l.end(), tolower);
        return l;
    }

public:
    bool& value;
    OptionBool(bool& _value, const std::string& _usage)
        : OptionBase(_usage), value(_value) {}
    OptionBool(const OptionBool&) = default;
    virtual ~OptionBool() = default;
    void Parse(const std::string& text) {
        //文字列として認識
        std::string text_low = low(text);
        if (text == "true") {
            value = true;
        } else if (text == "false") {
            value = false;
        }
        //数字として認識
        value = std::stoi(text) != 0;
    }
};

class BootOption {
    std::unordered_map<std::string, std::unique_ptr<OptionBase>> list;
    template <class T>
    std::unique_ptr<OptionBase> option(T& value, const std::string& usage);

public:
    void Parse(int argc, char** argv);
    // string
    template <class T>
    void Registor(const std::string& _key,
                  T& _value,
                  const std::string& _usage = "") {
        list.emplace(_key, std::move(option(_value, _usage)));
    }
    template <class T>
    void Registor(const std::string& _key,
                  T& _value,
                  const T& init,
                  const std::string& usage = "") {
        _value = init;
        Registor(_key, _value, usage);
    }
};

template <class T>  std::unique_ptr<OptionBase> option(T& value, const std::string& usage){
    return nullptr;
}
/*
template <>
std::unique_ptr<OptionBase> BootOption::option<std::string>(
    std::string& _value,
    const std::string& _usage) {
    return std::make_unique<OptionString>(_value, _usage);
}

template <>
std::unique_ptr<OptionBase> BootOption::option<int>(
    int& _value,
    const std::string& _usage) {
    return std::make_unique<OptionInt>(_value, _usage);
}

template <>
std::unique_ptr<OptionBase> BootOption::option<bool>(
    bool& _value,
    const std::string& _usage) {
    return std::make_unique<OptionBool>(_value, _usage);
}*/

#endif