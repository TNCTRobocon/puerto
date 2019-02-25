#include "motors.hpp"
#include <iostream>
#include <string>
namespace Net {
using namespace json11;

Json MotorAdapter::Apply(const Json& motors) {
    std::map<std::string, Json> merge;
    //単一モーター
    auto list = motors.object_items();
    for (const auto& [label, single] : list) {
        //命名規則"#*
        if (label[0] != '#') {
            continue;
        }
        std::string name = label.substr(1);
        //ダミー実装
        if (auto duty = single["duty"]; duty.is_number()) {
            std::cout << "duty"
                      << "[" << name << "]" << duty.number_value() << std::endl;
        }

        
    }

    return Json::object{};
}

}  // namespace Net