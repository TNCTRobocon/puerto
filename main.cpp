#include "main.hpp"
#include <iostream>
#include <thread>
#include <zmq.hpp>
#include "boot/flags.hpp"
#include "net/motors.hpp"
#include "net/server.hpp"
#include "json11/json11_helper.hpp"
using namespace std;
int main(int argc, char** argv) {
    Application app(argc, argv);
    auto& setting = app.setting;
    //Net::Server server("*",setting->network.port);
    {
        string dummy;
        std::cout << "run until pressing any key" << std::endl;
        cin >> dummy;
    }
    std::cout << "a" << endl;
    /*server.Stop();*/
    return 0;
}

Application::Application(int argc, char** argv) {
    Boot::Flags flag;
    flag.Add("config", setting_path, "設定ファイルの場所");
    flag.Parse(argc, argv);
    path = flag.GetPath();
    auto it = json11::load(setting_path);
    if (!it) {
        std::cerr << it.GetLeft() << std::endl;
        std::cerr << "[Info] load default setting" << std::endl;
        it.Reset(json11::get_dummy());
    }
    setting = std::make_shared<Boot::Setting>(it.GetRight());
}

Application::~Application() {
    json11::save(setting->toJson(), setting_path);
}
