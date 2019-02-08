#include <iostream>
#include <thread>
#include <zmq.hpp>
#include "json/json.hpp"
#include "json/parser.hpp"

using namespace std;
using namespace BindJson::Parser;
int main(int argc, char **argv) {
    wstring line;
    while (getline(wcin, line)) {
        auto now = Region(line);
        wcout << ">"<<now.ToString() << endl;
        //auto next = Integer()->Fetch(now);
       /* if (next.has_value()) {
            wcout <<">"<< now.Difference(next.value()).ToString()<<endl;
        }else{
            wcout<<">"<<"error"<<endl;
        }*/
    }
    return 0;
}
