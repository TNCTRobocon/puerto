#include "boot.hpp"
#include <string.h>
#include <algorithm>
#include <iostream>
#include <istream>
#include <sstream>
using namespace std;
namespace Boot {

void FlagBool::Set(const std::string& inst) {
    if (inst == "true") {
        value = true;
    } else if (inst == "false") {
        value = false;
    } else {
        value = stoi(inst) != 0;
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
                for (auto pair : flags) {
                    cout << pair.first << "\t" << pair.second->GetUsage();
                }
            } else if (auto it = flags.find(key); it != flags.end()) {
                it->second->Set(value);
            }
        }
    }
}





}  // namespace Boot
