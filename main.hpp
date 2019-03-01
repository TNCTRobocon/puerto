#pragma once
#ifndef __MAIN_HEADER_GUARD__
#define __MAIN_HEADER_GUARD__

#include <boot/settings.hpp>
#include <memory>
#include <string>
struct Application final {
    std::shared_ptr<Boot::Setting> setting{nullptr};
    std::string setting_path{ "setting.json"};
    std::string path;
    Application(int argc, char** argv);
    ~Application();
};

#endif