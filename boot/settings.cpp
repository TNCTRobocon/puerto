#include "settings.hpp"
#include <iostream>
namespace Boot {
using namespace std;
using namespace json11;

std::unique_ptr<Setting> settings;
Setting::Setting(const string& _filename) : filename(_filename) {
    Read();
}

Setting::~Setting() {
    Write();
}

void Setting::Read() {
    string text,error;
    // file access
    {
        ifstream reader(filename);
        if (!reader.is_open()) return;
        istreambuf_iterator<char> begin(reader), end;
        text=string(begin,end);
    }
    // read as json
    Json root=Json::parse(text,error);
    Deserialize(root);
}

void Setting::Write() const {
    auto root = Serialize();
    ofstream writer(filename);    
    writer<<root.dump();
}

json11::Json Setting::Serialize() const{
    Json root=Json::object{
        {"test","a"}
    };
    return root;
}

void Setting::Deserialize(const Json& root){
    auto it =root["test"];
    if (it.is_string()){
        cout<<it.string_value();
    }
}



}  // namespace Boot