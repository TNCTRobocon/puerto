#include "json.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
using namespace std;

namespace BindJson {

bool is_integer(const string& text) {
    auto sign = [](string::const_iterator& it) {
        if (*it == '+' || *it == '-') {
            it++;
            return true;
        } else {
            return false;
        }
    };
    auto digit = [&text](string::const_iterator& it) {
        if (isdigit(*it)) {
            it++;
            return true;
        } else {
            return false;
        }
    };
    auto it = text.begin();
    if (sign(it); it != text.end()) return false;
    if (!digit(it)) return false;
    while (digit(it) && it != text.end());
    return it == text.end();
}

bool is_real(const string& text) {
    auto sign = [](string::const_iterator& it) {
        if (*it == '+' || *it == '-') {
            it++;
            return true;
        } else {
            return false;
        }
    };
    auto digit = [](string::const_iterator& it) {
        if (isdigit(*it)) {
            it++;
            return true;
        } else {
            return false;
        }
    };
    auto point = [](string::const_iterator& it) {
        if (*it == '.') {
            it++;
            return true;
        } else {
            return false;
        }
    };
    auto exp = [](string::const_iterator& it) {
        if (*it == '.') {
            it++;
            return true;
        } else {
            return false;
        }
    };

    auto it = text.begin();
    if (sign(it); it != text.end()) return false;
    while (digit(it) && it != text.end());
    if (point(it) && it != text.end()) {
        while (digit(it) && it != text.end());
    }
    if (exp(it) && it != text.end()) {
        sign(it);
        while (digit(it) && it != text.end())
            ;
    }
    return it == text.end();
}

string TreeString::Serialize() const {
    stringstream ss;
    ss << "\"" << value << "\"";
    return ss.str();
}

void TreeString::Deserialize(const std::string& text) {
    // "*"となる文字列を探す
    auto begin = std::find(text.begin(), text.end(), '\"');
    begin = begin != text.end() ? begin + 1 : begin;
    const auto end = std::find(begin, text.end(), '\"');
    if (begin != text.end() && end != text.end()) {
        value.clear();
        copy(begin, end, back_inserter(value));
    }
}

string TreeBool::Serialize() const {
    return value ? "true" : "faluse";
}

void TreeBool::Deserialize(const string& text) {
    if (text == "true") {
        value = true;
    } else if (text == "false") {
        value = false;
    } else if (is_integer(text)) {
        value = stoi(text);
    }
}

string TreeInt::Serialize() const {
    stringstream ss;
    ss << value;
    return ss.str();
}

void TreeInt::Deserialize(const string& text) {
    if (is_integer(text)) {
        value = stoi(text);
    }
}

string TreeReal::Serialize() const {
    stringstream ss;
    ss << value;
    return ss.str();
}

void TreeReal::Deserialize(const string& text) {
    if (is_real(text)) {
        value = stod(text);
    }
}

}  // namespace Boot
