#include "main.hpp"
#include <iostream>
#include <thread>
#include <zmq.hpp>
#include "boot/flags.hpp"
#include "boot/json11_helper.hpp"
#include "net/motors.hpp"
#include "net/server.hpp"
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
    cout << "a" << endl;
    server.Stop();
    return 0;
}

Application::Application(int argc, char** argv) {
    Boot::Flags flag;
    flag.Add("config", setting_path, "設定ファイルの場所");
    flag.Parse(argc, argv);
    auto it = Boot::load(setting_path);
    if (!it) {
        std::cerr << it.GetLeft() << std::endl;
        std::cerr << "[Info] load default setting"<<std::endl;
        it.Reset(Boot::get_dummy());
    }
    setting = std::make_shared<Boot::Setting>(it.GetRight());
}

Application::~Application() {
    Boot::save(Boot::to_json(*setting), setting_path);
}
