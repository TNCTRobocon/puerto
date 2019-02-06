#include "json.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include "parser.hpp"
using namespace std;

namespace BindJson {

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
    } else  {
        
        value = stoi(text);
    }
}

string TreeInt::Serialize() const {
    stringstream ss;
    ss << value;
    return ss.str();
}

void TreeInt::Deserialize(const string& text) {
    
        value = stoi(text);
    
}

string TreeReal::Serialize() const {
    stringstream ss;
    ss << value;
    return ss.str();
}

void TreeReal::Deserialize(const string& text) {
    
        value = stod(text);
    
}

}  // namespace Boot
