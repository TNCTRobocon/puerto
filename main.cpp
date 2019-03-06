#include "main.hpp"
#include <stdlib.h>
#include <boot/settings.hpp>
#include <iostream>
#include <thread>
#include <zmq.hpp>
#include "boot/flags.hpp"
#include "json11/json11_helper.hpp"
#include "net/service.hpp"
using namespace std;
int main(int argc, char** argv) {
    Application app(argc, argv);
    auto& setting = app.setting;
    Net::Service service(setting->network.host, setting->network.port);
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
    using namespace std;
    Boot::Flags flag;
    bool isInitializing{false};
    flag.Add("config", setting_path, "設定ファイルの場所を指定する");
    flag.Add("init", isInitializing, "設定ファイルを生成する");
    flag.Parse(argc, argv);
    path = flag.GetPath();

    if (isInitializing) {
        Boot::Setting defaultSettings(json11::get_dummy());
        json11::save(defaultSettings.toJson(), setting_path);
        cout << "wrote settings into "<<setting_path<<endl;
         exit(0);
    }

    cout << "load form" << path << endl;
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
