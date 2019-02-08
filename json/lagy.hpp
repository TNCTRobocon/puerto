#pragma once
#ifndef __LAGY_HEADER_GUARD__
#define __LAGY_HEADER_GUARD__

namespace BindJson {
//遅延生成
template <class T>
class Lagy {
    const std::function<T()> init;
    mutable std::optional<T> instance;

    T& Get() {
        if (!instance.has_value()) {
            instance.reset(init());
        }
        return instance.value();
    }

public:
    Lagy(const std::function<T()>& _init) : init(_init) {}
    Lagy(const Lagy&) = default;
    virtual ~Lagy() = default;



    T& operator*(){
        return Get();
    }

    const T& operator*()const {
        return Get();
    }


};
}  // namespace BindJson

#endif