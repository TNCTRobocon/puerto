#include <iostream>
#include <thread>
#include <zmq.hpp>
#include "boot/settings.hpp"
#include "boot/flags.hpp"

int main(int argc, char **argv) {
    using namespace Boot;
    using namespace std;
    Flags flag;
    string setting_path="setting.json";
    flag.Add("config",setting_path,"設定ファイルの場所");
    flag.Parse(argc,argv);
    settings=CreateSetting(setting_path);
    return 0;
}
