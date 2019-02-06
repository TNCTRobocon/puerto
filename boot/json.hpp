#pragma once
#ifndef __JSON_HEADER_GUARD__
#define __JSON_HEADER_GUARD__
#include <string>
#include <unordered_map>

namespace Boot {

class TreeBase {
public:
    TreeBase() = default;
    TreeBase(const TreeBase&) = default;
    virtual ~TreeBase() = default;
    virtual std::string Serialize() const = 0;
    virtual void Deserialize(const std::string&) = 0;
};

class TreeString : public TreeBase {
    std::string& value;
public:
    TreeString(std::string& _value) : value(_value) {}
    TreeString(const TreeString&) = default;
    virtual ~TreeString()=default;
    virtual std::string Serialize() const;
    virtual void Deserialize(const std::string&);
};

class TreeBool:public TreeBase{
    bool& value;

public:
    TreeBool(bool& _value) : value(_value) {}
    TreeBool(const TreeBool&) = default;
    virtual ~TreeBool()=default;
    virtual std::string Serialize() const;
    virtual void Deserialize(const std::string&);
};

class TreeInt:public TreeBase{
    int& value;

public:
    TreeInt(int& _value) : value(_value) {}
    TreeInt(const TreeInt&) = default;
    virtual ~TreeInt()=default;
    virtual std::string Serialize() const;
    virtual void Deserialize(const std::string&);
};

class TreeReal:public TreeBase{
    double& value;
public:
    TreeReal(double& _value) : value(_value) {}
    TreeReal(const TreeReal&) = default;
    virtual ~TreeReal()=default;
    virtual std::string Serialize() const;
    virtual void Deserialize(const std::string&);
};




class TreeObject : public TreeBase {};

}  // namespace Boot

#endif