#include "boot.hpp"
#include <string.h>

using namespace std;

void BootOption::Parse(int argc,char** argv){
    for (int idx=1;idx<argc;idx++){
        char* it=argv[idx];
        if (!strncmp(it,"--",2)){
            it+=2;
            char *key=strtok(it,"="),*value=strtok(NULL,"=");
            auto target = list.find(string(key));
            if (target!=list.end()){
                target->second->Parse(string(value));
            }
        }
    }
}