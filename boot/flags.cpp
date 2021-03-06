#include "flags.hpp"
#include <iostream>
#include <istream>
#include <sstream>
#include <stdlib.h>
using namespace std;
namespace Boot {

void FlagBool::Set(const std::string& inst) {
    if (inst == "true") {
        value = true;
    } else if (inst == "false") {
        value = false;
    } else if (inst.empty()){
        value=true;
    }else{
        value =stoi(inst);
    }
}

Flags::~Flags() {
    for (auto it : flags) {
        delete it.second;
    }
}

void Flags::Parse(int argc, char** argv) {
    for (int pos = 1; pos < argc; pos++) {
        string line = argv[pos];
        if (!line.compare(0, 2, "--")) {
            // keyとvalueを抽出
            stringstream ss(line.substr(2));
            string key, value;
            if (getline(ss, key, '=')) {
                getline(ss, value, '=');
            }
            //検索
            if (key == "help") {
                for (auto [name,usage] : flags) {
                    cout << name << "\t" << usage->GetUsage()<<endl;
                }
                exit(0);
            } else if (auto it = flags.find(key); it != flags.end()) {
                it->second->Set(value);
            }
        }
    }
    // argv[0]について展開する
    const char* begin;
    for (const char* it = begin = argv[0]; *it != '\0'; it++) {
        if (*it == '/') {
            begin = it + 1;
        }
    }
    path= string(begin);
}

}  // namespace Boot
