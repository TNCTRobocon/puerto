#include <iostream>
#include <thread>
#include <zmq.hpp>
#include "json/json.hpp"


using namespace std;
using namespace Boot;
int main(int argc, char**argv) {
    cout<<"run"<<endl;
    std::string value="abc";
    TreeString test(value);
    cout<<test.Serialize()<<endl;
    test.Deserialize("\"def\"");
    cout<<test.Serialize()<<endl;
    return 0;
}

