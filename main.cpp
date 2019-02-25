#include "main.hpp"
#include <iostream>
#include <thread>
#include <zmq.hpp>
#include "boot/flags.hpp"
#include "net/server.hpp"
#include "net/motors.hpp"
using namespace std;
int main(int argc, char** argv) {
    Application app(argc, argv);
    auto& setting = app.setting;
    Net::Server server(setting->network->port);
    server.Add("motors",make_unique<Net::MotorAdapter>());
    server.Start();

    {
        string dummy;
        cout << "run until pressing any key" << endl;
        cin >> dummy;
    }
    return 0;
}

Application::Application(int argc, char** argv) {
    Boot::Flags flag;
    std::string setting_path = "setting.json";
    flag.Add("config", setting_path, "設定ファイルの場所");
    flag.Parse(argc, argv);
    setting = std::make_unique<Boot::Setting>(setting_path);
}
