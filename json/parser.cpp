#include "parser.hpp"
#include <algorithm>
using namespace std;
namespace BindJson ::Parser {

wstring Region::ToString() const {
    wstring result;
    copy(begin, end, back_inserter(result));
    return result;
}

Region CharactorList::operator()(const Region &now) const {
    if (wchar_t cmp = *now;
        now.IsActive() && any_of(list.begin(), list.end(),
                                 [cmp](wchar_t c) { return cmp == c; })) {
        return now + 1;
    } else {
        return now;
    }
}

Region ParserOr::operator()(const Region &now) const {
    if (!now) {
        return now;
    } else if (auto next = (*a)(now); now != next) {
        return next;
    } else if (auto next = (*b)(now); now != next) {
        return next;
    } else {
        return now;
    }
}

Region ParserAnd::operator()(const Region &now) const {
    if (!now) {
        return now;
    }
    if (auto next = (*a)(now); next != now) {
        if (auto next2 = (*b)(next); next2 != next) {
            return next2;
        }
    }
    return now;
}

Region ParserAny::operator()(const Region &last) const {
    if (!last) {
        return last;
    }
    auto now = last, next = last;
    // N回試行
    for (unsigned int count = 0; count < n; count++) {
        next = (*child)(now);
        if (now == next) {
            return last;
        }
        now = next;
    }
    while (true) {
        next = (*child)(now);
        if (now == next) {
            return now;
        }
        now = next;
    }
}

Region ParserSome::operator()(const Region &last) const {
    if (!last) {
        return last;
    }
    auto now = last, next = last;
    // N回試行
    for (unsigned int count = 0; count < n; count++) {
        next = (*child)(now);
        if (now == next) {
            return last;
        }
        now = next;
    }
    return now;
}

}  // namespace BindJson::Parser