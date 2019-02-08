#pragma once
#ifndef __PARSER_HEADER_GUARD__
#define __PARSER_HEADER_GUARD__

#include <stddef.h>
#include <wchar.h>
#include <algorithm>
#include <cctype>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include "lagy.hpp"
namespace BindJson ::Parser {

struct Region final {
    using iterator = std::wstring::const_iterator;
    iterator begin, end;
    Region(const std::wstring& text) : begin(text.cbegin()), end(text.cend()) {}
    Region(iterator _begin, iterator _end) : begin(_begin), end(_end) {}
    Region(const Region& orign) : begin(orign.begin), end(orign.end) {}
    ~Region() = default;
    //演算子
    Region operator=(const Region& orign) {
        return Region(begin = orign.begin, end = orign.end);
    }
    bool operator==(const Region& cmp) const {
        return begin == cmp.begin && end == cmp.end;
    }
    bool operator!=(const Region& cmp) const {
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
    //読み進んだ文を返す
    Region Difference(const Region& changed) const {
        return Region(begin, min(changed.begin, end));
    }
};

class ParserBase {
public:
    ParserBase() = default;
    ParserBase(const ParserBase&) = default;
    virtual ~ParserBase() = default;
    virtual std::optional<Region> Fetch(const Region& now) const = 0;//先読み用
    virtual std::optional<Region> Load(const Region& now) const {//実際に読み込む
        return Fetch(now);
    }
};

using ParserPointer = std::shared_ptr<ParserBase>;

class CharactorItem : public ParserBase {
    const wchar_t value;

public:
    CharactorItem(wchar_t _value) : value(_value) {}
    CharactorItem(const CharactorItem&) = default;
    virtual ~CharactorItem() = default;
    virtual std::optional<Region> Fetch(const Region& reg) const {
        if ((reg.IsActive() && (*reg == value))) {
            return reg + 1;
        } else {
            return std::nullopt;
        }
    }
};

static inline ParserPointer Item(wchar_t c) {
    return ParserPointer(std::make_shared<CharactorItem>(c));
}

class CharactorList : public ParserBase {
    const std::wstring list;

public:
    CharactorList(const std::wstring& _list) : list(_list) {}
    CharactorList(const CharactorList&) = default;
    virtual ~CharactorList() = default;
    virtual std::optional<Region> Fetch(const Region& reg) const;
};

static inline ParserPointer List(const std::wstring& list) {
    return ParserPointer(std::make_shared<CharactorList>(list));
}
static inline ParserPointer Sign() {
    return ParserPointer(std::make_shared<CharactorList>(L"+-"));
}

class CharactorRange : public ParserBase {
    const wchar_t begin, end;

public:
    CharactorRange(wchar_t _begin, wchar_t _end) : begin(_begin), end(_end) {}
    CharactorRange(const CharactorRange&) = default;
    virtual ~CharactorRange() = default;
    virtual std::optional<Region> Fetch(const Region& reg) const {
        if (reg.IsActive() && (begin <= *reg) && (*reg <= end)) {
            return reg + 1;
        } else {
            return std::nullopt;
        }
    }
};

static inline ParserPointer Range(wchar_t begin, wchar_t end) {
    return ParserPointer(std::make_shared<CharactorRange>(begin, end));
}

class ParserFunction : public ParserBase {
    std::function<bool(wchar_t)> condition;

public:
    ParserFunction(const std::function<bool(wchar_t)>& _condition)
        : condition(_condition) {}
    ParserFunction(const ParserFunction&) = default;
    virtual ~ParserFunction() = default;
    virtual std::optional<Region> Fetch(const Region& now) const {
        return (now.IsActive() && condition(*now)) ? now + 1 : now;
    }
};

class ParserSkip : public ParserBase {
    const unsigned int n;  // n回
public:
    ParserSkip(unsigned int _n) : n(_n) {}
    ParserSkip(const ParserSkip&) = default;
    virtual ~ParserSkip() = default;
    virtual std::optional<Region> Fetch(const Region& now) const {
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
    ParserOr(const ParserOr&) = default;
    virtual ~ParserOr() = default;
    virtual std::optional<Region> Fetch(const Region& reg) const;
};

static inline ParserPointer operator+(ParserPointer a, ParserPointer b) {
    return ParserPointer(std::make_shared<ParserOr>(a, b));
}

class ParserAnd : public ParserBase {
    const ParserPointer a, b;

public:
    ParserAnd(ParserPointer _a, ParserPointer _b) : a(_a), b(_b) {}
    ParserAnd(const ParserAnd&) = default;
    virtual ~ParserAnd() = default;
    virtual std::optional<Region> Fetch(const Region& reg) const;
};

static inline ParserPointer operator*(ParserPointer a, ParserPointer b) {
    return ParserPointer(std::make_shared<ParserAnd>(a, b));
}

class ParserOver : public ParserBase {
    const ParserPointer child;
    const unsigned int n;  // n回以上
public:
    ParserOver(ParserPointer _child, unsigned int _n) : child(_child), n(_n) {}
    ParserOver(const ParserOver&) = default;
    virtual ~ParserOver() = default;
    virtual std::optional<Region> Fetch(const Region& now) const;
};

static inline ParserPointer Over(ParserPointer a, unsigned int n) {
    return ParserPointer(std::make_shared<ParserOver>(a, n));
}

static inline ParserPointer Some(ParserPointer a) {
    return ParserPointer(std::make_shared<ParserOver>(a, 1));
}

class ParserLoop : public ParserBase {
    const ParserPointer child;
    const unsigned int n;  // n回
public:
    ParserLoop(ParserPointer _child, unsigned int _n) : child(_child), n(_n) {}
    ParserLoop(const ParserLoop&) = default;
    virtual ~ParserLoop() = default;
    virtual std::optional<Region> Fetch(const Region& now) const;
};

static inline ParserPointer operator^(ParserPointer a, unsigned int n) {
    return ParserPointer(std::make_shared<ParserLoop>(a, n));
}

// 0,1回
class ParserOption : public ParserBase {
    const ParserPointer child;

public:
    ParserOption(ParserPointer _child) : child(_child) {}
    ParserOption(const ParserOption&) = default;
    virtual ~ParserOption() = default;
    virtual std::optional<Region> Fetch(const Region& now) const;
};

static inline ParserPointer Option(ParserPointer p) {
    return ParserPointer(std::make_shared<ParserOption>(p));
}




class ParserInteger : public ParserBase {
    static ParserPointer parser;
    int& bind;

public:
    ParserInteger(int& _bind);
    ParserInteger(const ParserInteger&)=default;
    virtual ~ParserInteger()=default;
    virtual std::optional<Region> Fetch(const Region& now) const;
    virtual std::optional<Region> Load(const Region& now) const;
};

}  // namespace BindJson::Parser

#endif