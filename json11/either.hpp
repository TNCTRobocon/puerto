#pragma once
#ifndef __UTIL_EITHER_HEADER_GURAD__
#define __UTIL_EITHER_HEADER_GUARD__
#include <memory>
#include <optional>

namespace util {

template <class R, class L>
class Either {
    std::optional<R> right{std::nullopt};
    std::optional<L> left{std::nullopt};
    Either(const R& _right, const L& _left) : right(_right), left(_left) {}

public:
    //暗黙的に生成する
    Either(const R& _right) : right(_right) {}
    Either(const L& _left) : left(_left) {}
    Either(const Either&) = default;
    Either(Either&& orign) {
        if (orign.right.has_value()) {
            right.reset(std::move(orign.right));
        }
        if (orign.left.has_value()) {
            left.reset(std::move(orign.left));
        }
    }
    //明示的に生成する
    static Either&& Right(const R& _right) {
        return Either(_right, std::nullopt);
    }
    static Either&& Left(const R& _left) {
        return Either(std::nullopt, _left);
    }

    ~Either() = default;
    operator bool() const {
        return right.has_value();
    }

    bool operator!() const {
        return left.has_value();
    }

    //アクセサ

    bool IsRight() const {
        return right.has_value();
    }

    bool IsLeft() const {
        return right.has_value();
    }

    R& GetRight() {
        return right.value();
    }

    const R& GetRight() const {
        return right.value();
    }

    L& GetLeft() {
        return left.value();
    }
    const L& GetLeft() const {
        return left.value();
    }

    void Reset(const R& _right) {
        right = _right;
        left.reset();
    }

    void Reset(const L& _left) {
        right.reset();
        left=_left;
    }
};
}  // namespace util
#endif