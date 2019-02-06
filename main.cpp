#include <iostream>
#include <thread>
#include <zmq.hpp>
#include "boot.hpp"


using namespace std;
using namespace Boot;
int main(int argc, char**argv) {
    BootFlags flags;
    std::string test;
    flags.Add("test",test,"for test");
    flags.Parse(argc,argv);
    cout<<test<<endl;
    return 0;
}

