#pragma once
#ifndef __PARSER_HEADER_GUARD__
#define __PARSER_HEADER_GUARD__

#include <stddef.h>
#include <algorithm>
#include <cctype>
#include <functional>
#include <memory>
#include <string>

namespace BindJson {
using Iterator = std::string::const_iterator;
class ParserBase {
public:
    virtual Iterator operator()(Iterator now, Iterator end) const = 0;
};

using ParserPointer = std::unique_ptr<ParserBase>;

class ParserDigit : public ParserBase {
public:
    virtual Iterator operator()(Iterator it, Iterator end) const {
        return (it != end) && isdigit(*it) ? it + 1 : it;
    }
};

class ParserCharactorList : public ParserBase {
    const std::string match;
public:
    ParserCharactorList(const std::string& _match) : match(_match) {}
    virtual Iterator operator()(Iterator it, Iterator end) const {
        return (it != end) &&
                       std::any_of(match.begin(), match.end(),
                                   [it](char c) -> bool { return *it == c; })
                   ? it + 1
                   : it;
    }
};

class ParserCharactor : public ParserBase {
    const char c;

public:
    ParserCharactor(char _c) : c(_c) {}
    virtual Iterator operator()(Iterator it, Iterator end) const {
        return (it != end) && *it == c ? it + 1 : it;
    }
};

class ParserRange : public ParserBase {
    const char start,finish;
public:
    ParserRange(char _start,char _finish) : start(_start),finish(_finish){}
    virtual Iterator operator()(Iterator it, Iterator end) const {
        return (it != end) && (start<=*it)&&(*it<finish)? it + 1 : it;
    }
};

// 0回以上
class ParserAny : public ParserBase {
    const std::unique_ptr<ParserBase> child;

public:
    ParserAny(std::unique_ptr<ParserBase> _child) : child(std::move(_child)) {}
    virtual Iterator operator()(Iterator begin, Iterator end) const;
};

// n回以上
class ParserSome : public ParserBase {
    const ParserPointer child;
    const unsigned int n;

public:
    ParserSome(ParserPointer _child, unsigned int _n = 1)
        : child(std::move(_child)), n(_n) {}
    virtual Iterator operator()(Iterator begin, Iterator end) const;
};

// n回
class ParserMul : public ParserBase {
    const ParserPointer child;
    const unsigned int n;

public:
    ParserMul(ParserPointer _child, unsigned int _n = 1)
        : child(std::move(_child)), n(_n) {}
    virtual Iterator operator()(Iterator begin, Iterator end) const;
};

class ParserOr : public ParserBase {
    ParserPointer a, b;

public:
    ParserOr(ParserPointer _a, ParserPointer _b)
        : a(std::move(_a)), b(std::move(_b)) {}
    virtual Iterator operator()(Iterator begin, Iterator end) const {
        if (auto now = (*a)(begin, end); now != begin) {
            return now;
        } else if (auto now = (*b)(begin, end); now != begin) {
            return now;
        } else {
            return begin;
        }
    }
};

class ParserAnd : public ParserBase {
    ParserPointer a, b;

public:
    ParserAnd(ParserPointer _a, ParserPointer _b)
        : a(std::move(_a)), b(std::move(_b)) {}
    virtual Iterator operator()(Iterator begin, Iterator end) const {
        if (auto now = (*a)(begin, end); now != begin) {
            if (auto next = (*b)(now, end); next != now) {
                return next;
            }
        }
        return begin;
    }
};

ParserPointer operator|(ParserPointer a, ParserPointer b);
ParserPointer operator&(ParserPointer a, ParserPointer b);
ParserPointer operator+(ParserPointer a, unsigned int n);//n回以上
ParserPointer operator*(ParserPointer a, unsigned int n);//n回


}  // namespace BindJson

#endif