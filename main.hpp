#pragma once
#ifndef __MAIN_HEADER_GUARD__
#define __MAIN_HEADER_GUARD__

#include <boot/settings.hpp>
#include <memory>

struct Application final {
    std::unique_ptr<Boot::Setting> setting{nullptr};

    Application(int argc, char** argv);
};

#endif