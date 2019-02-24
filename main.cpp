#include <iostream>
#include <thread>
#include <zmq.hpp>
#include "boot/flags.hpp"
#include "boot/settings.hpp"
#include "net/server.hpp"
static bool setup(int argc, char** argv);

int main(int argc, char** argv) {
    setup(argc, argv);

    //Net::Server server(5555);

    return 0;
}

bool setup(int argc, char** argv) {
    using namespace Boot;
    using namespace std;
    Flags flag;
    string setting_path = "setting.json";
    flag.Add("config", setting_path, "設定ファイルの場所");
    flag.Parse(argc, argv);
    settings = CreateSetting(setting_path);
    return true;
}
