#include "parser.hpp"
using namespace std;
namespace BindJson {

Iterator ParserAny::operator()(Iterator begin, Iterator end) const {
    for (auto now = begin;;) {
        if (now == end) {
            return now;
        } else if (auto next = (*child)(now, end); next != now) {
            now = next;
        } else {
            return now;
        }
    }
}

Iterator ParserSome::operator()(Iterator begin, Iterator end) const {
    // n回回す
    auto now = begin;
    for (unsigned int cnt = 0; cnt < n; cnt++) {
        if (now == end) {
            return begin;
        } else if (auto next = (*child)(now, end); next != now) {
            now = next;
        } else {
            return begin;
        }
    }
    //回せるだけ回す
    for (;;) {
        if (now == end) {
            return now;
        } else if (auto next = (*child)(now, end); next != now) {
            now = next;
        } else {
            return now;
        }
    }
}

Iterator ParserMul::operator()(Iterator begin, Iterator end) const {
    // n回回す
    auto now = begin;
    for (unsigned int cnt = 0; cnt < n; cnt++) {
        if (now == end) {
            return begin;
        } else if (auto next = (*child)(now, end); next != now) {
            now = next;
        } else {
            return begin;
        }
    }
    return now;
}

ParserPointer operator|(ParserPointer a, ParserPointer b) {
    return make_unique<ParserOr>(move(a), move(b));
}

ParserPointer operator&(ParserPointer a, ParserPointer b) {
    return make_unique<ParserAnd>(move(a), move(b));
}

ParserPointer operator+(ParserPointer a, unsigned int n) {
    return make_unique<ParserSome>(move(a), n);
}

ParserPointer operator*(ParserPointer a, unsigned int n) {
    return make_unique<ParserMul>(move(a), n);
}

}  // namespace BindJson