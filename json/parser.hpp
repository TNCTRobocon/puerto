#pragma once
#ifndef __PARSER_HEADER_GUARD__
#define __PARSER_HEADER_GUARD__

#include <stddef.h>
#include <wchar.h>
#include <algorithm>
#include <cctype>
#include <functional>
#include <memory>
#include <string>

namespace BindJson ::Parser {

struct Region final {
    using iterator = std::wstring::const_iterator;
    iterator begin, end;
    Region(const std::wstring &text) : begin(text.cbegin()), end(text.cend()) {}
    Region(iterator _begin, iterator _end) : begin(_begin), end(_end) {}
    Region(const Region &orign) = default;
    ~Region() = default;
    //演算子
    Region operator=(const Region &orign) {
        return Region(begin = orign.begin, end = orign.end);
    }
    bool operator==(const Region &cmp) const {
        return begin == cmp.begin && end == cmp.end;
    }
    bool operator!=(const Region &cmp) const {
        return begin != cmp.begin || end != cmp.end;
    }
    bool operator!() const {
        return begin >= end;
    }
    // iterator
    wchar_t operator*() const {
        return *begin;
    }
    Region operator+(int pos) const {
        return Region(begin + pos, end);
    }
    Region operator+=(int pos) {
        return Region(begin += pos, end);
    }
    Region operator++() {
        return Region(++begin, end);
    }
    Region operator++(int) {
        return Region(begin++, end);
    }
    Region operator-(int pos) const {
        return Region(begin + pos, end);
    }
    Region operator-=(int pos) {
        return Region(begin -= pos, end);
    }
    Region operator--() {
        return Region(--begin, end);
    }  // iterator
    Region operator--(int) {
        return Region(begin--, end);
    }
    //プロパティ
    std::ptrdiff_t Distance() const {
        return std::distance(begin, end);
    }
    std::wstring ToString() const;
    //有効かどうかの確認
    bool IsActive() const {
        return begin < end;
    }
};

class ParserBase {
public:
    ParserBase() = default;
    ParserBase(const ParserBase &) = default;
    virtual ~ParserBase() = default;
    virtual Region operator()(const Region &now) const = 0;
};

using ParserPointer = std::shared_ptr<ParserBase>;

class CharactorItem : public ParserBase {
    const wchar_t value;

public:
    CharactorItem(wchar_t _value) : value(_value) {}
    CharactorItem(const CharactorItem &) = default;
    virtual ~CharactorItem() = default;
    virtual Region operator()(const Region &reg) const {
        return (reg.IsActive() && (*reg == value)) ? reg + 1 : reg;
    }
};

inline ParserPointer Item(wchar_t c) {
    return ParserPointer(std::make_shared<CharactorItem>(c));
}

class CharactorList : public ParserBase {
    const std::wstring list;

public:
    CharactorList(const std::wstring &_list) : list(_list) {}
    CharactorList(const CharactorList &) = default;
    virtual ~CharactorList() = default;
    virtual Region operator()(const Region &reg) const;
};

inline ParserPointer List(const std::wstring &list) {
    return ParserPointer(std::make_shared<CharactorList>(list));
}

class CharactorRange : public ParserBase {
    const wchar_t begin, end;

public:
    CharactorRange(wchar_t _begin, wchar_t _end) : begin(_begin), end(_end) {}
    CharactorRange(const CharactorRange &) = default;
    virtual ~CharactorRange() = default;
    virtual Region operator()(const Region &reg) const {
        return reg.IsActive() && (begin <= *reg) && (*reg <= end) ? reg + 1
                                                                  : reg;
    }
};

inline ParserPointer Range(wchar_t begin, wchar_t end) {
    return ParserPointer(std::make_shared<CharactorRange>(begin, end));
}

class ParserSkip : public ParserBase {
    const unsigned int n;  // n回
public:
    ParserSkip(unsigned int _n) : n(_n) {}
    ParserSkip(const ParserSkip &) = default;
    virtual ~ParserSkip() = default;
    virtual Region operator()(const Region &now) const {
        if (auto next = now + n; next.IsActive()) {
            return next;
        } else {
            return now;
        }
    }
};

class ParserOr : public ParserBase {
    const ParserPointer a, b;

public:
    ParserOr(ParserPointer _a, ParserPointer _b) : a(_a), b(_b) {}
    ParserOr(const ParserOr &) = default;
    virtual ~ParserOr() = default;
    virtual Region operator()(const Region &reg) const;
};
inline ParserPointer operator||(ParserPointer a, ParserPointer b) {
    return ParserPointer(std::make_shared<ParserOr>(a, b));
}

class ParserAnd : public ParserBase {
    const ParserPointer a, b;

public:
    ParserAnd(ParserPointer _a, ParserPointer _b) : a(_a), b(_b) {}
    ParserAnd(const ParserAnd &) = default;
    virtual ~ParserAnd() = default;
    virtual Region operator()(const Region &reg) const;
};

inline ParserPointer operator&&(ParserPointer a, ParserPointer b) {
    return ParserPointer(std::make_shared<ParserAnd>(a, b));
}

class ParserAny : public ParserBase {
    const ParserPointer child;
    const unsigned int n;  // n回以上
public:
    ParserAny(ParserPointer _child, unsigned int _n) : child(_child), n(_n) {}
    ParserAny(const ParserAny &) = default;
    virtual ~ParserAny() = default;
    virtual Region operator()(const Region &now) const;
};

inline ParserPointer operator+(ParserPointer a, unsigned int n) {
    return ParserPointer(std::make_shared<ParserAny>(a, n));
}

class ParserSome : public ParserBase {
    const ParserPointer child;
    const unsigned int n;  // n回
public:
    ParserSome(ParserPointer _child, unsigned int _n) : child(_child), n(_n) {}
    ParserSome(const ParserSome &) = default;
    virtual ~ParserSome() = default;
    virtual Region operator()(const Region &now) const;
};

inline ParserPointer operator*(ParserPointer a, unsigned int n) {
    return ParserPointer(std::make_shared<ParserSome>(a, n));
}

}  // namespace BindJson::Parser

#endif