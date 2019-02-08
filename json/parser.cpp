#include "parser.hpp"
#include <algorithm>
#include <optional>
using namespace std;
namespace BindJson ::Parser {

wstring Region::ToString() const {
    wstring result;
    copy(begin, end, back_inserter(result));
    return result;
}

optional<Region> CharactorList::Fetch(const Region &now) const {
    if (wchar_t cmp = *now;
        now.IsActive() && any_of(list.begin(), list.end(),
                                 [cmp](wchar_t c) { return cmp == c; })) {
        return now + 1;
    } else {
        return nullopt;
    }
}

optional<Region> ParserOr::Fetch(const Region &now) const {
    if (!now) {
        return nullopt;
    } else if (auto next =a->Fetch(now); next.has_value()) {
        return next;
    } else if (auto next =b->Fetch(now); next.has_value()) {
        return next;
    } else {
        return nullopt;
    }
}

optional<Region> ParserAnd::Fetch(const Region &last) const {
    if (!last) {
        return nullopt;
    }
    if (auto now = a->Fetch(last); now.has_value()) {
        if (auto next = b->Fetch(now.value()); next.has_value()) {
            return next;
        }
    };
    return nullopt;
}

optional<Region> ParserOver::Fetch(const Region &last) const {
    if (!last) {
        return last;
    }
    optional<Region> now=last,next=nullopt;
    // N回試行
    for (unsigned int count = 0; count < n; count++) {
        next = child->Fetch(now.value());
        if (!next.has_value() ) {
            return nullopt;
        }
        now = next;
    }
    while (true) {
        next = child->Fetch(now.value());
        if ( !next.has_value()) {
            return now;
        }
        now = next;
    }
}

optional<Region> ParserLoop::Fetch(const Region &last) const {
    if (!last) {
        return nullopt;
    }
    optional<Region> now = last, next = nullopt;
    // N回試行
    for (unsigned int count = 0; count < n; count++) {
        next = child->Fetch(now.value());
        if (!next.has_value()) {
            return nullopt;
        }
        now = next;
    }
    return now.value();
}

optional<Region> ParserOption::Fetch(const Region &now) const {
    if (!now.IsActive())return nullopt;
    if (auto next =child->Fetch(now);next.has_value()){
        return next.value();
    }else{
        return now; 
    }
}

ParserPointer ParserInteger::parser;




}  // namespace BindJson::Parser