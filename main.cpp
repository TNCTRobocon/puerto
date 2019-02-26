#include "main.hpp"
#include <iostream>
#include <thread>
#include <zmq.hpp>
#include "boot/flags.hpp"
#include "net/motors.hpp"
#include "net/server.hpp"
#include "boot/json11_helper.hpp"
using namespace std;
int main(int argc, char** argv) {
    Application app(argc, argv);
    auto& setting = app.setting;
    Net::Server server(setting->network.port);
    server.Add("motors", make_unique<Net::MotorAdapter>());
    server.Start();

    {
        string dummy;
        cout << "run until pressing any key" << endl;
        cin >> dummy;
    }
    server.Stop();
    return 0;
}

Application::Application(int argc, char** argv) {
    Boot::Flags flag;
    std::string setting_path = "setting.json";
    flag.Add("config", setting_path, "設定ファイルの場所");
    flag.Parse(argc, argv);
    auto it = Boot::load(setting_path);
    if (!it) {
        std::cerr << it.GetLeft() << std::endl;
        exit(0);
    }
    setting = std::make_shared<Boot::Setting>(it.GetRight());
}
