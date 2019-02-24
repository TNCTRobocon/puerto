#include "settings.hpp"
#include <iostream>
namespace Boot {
using namespace std;
using namespace json11;

Setting::Setting(const string& _filename) : filename(_filename) {
    Read();
}

Setting::~Setting() {
    Write();
}

void Setting::Read() {
    string text, error;
    // file access
    {
        ifstream reader(filename);
        if (!reader.is_open()) return;
        istreambuf_iterator<char> begin(reader), end;
        text = string(begin, end);
    }
    // read as json
    Json root = Json::parse(text, error);
    Deserialize(root);
}

void Setting::Write() const {
    auto root = Serialize();
    ofstream writer(filename);
    writer << root.dump();
}

json11::Json Setting::Serialize() const {
    Json root=Json::object{{"network",network->Serialize()}};
    return root;
}

void Setting::Deserialize(const Json& root) {
   
    if (auto it = root["network"]; it.is_object()) {
        network = std::make_unique<NetWork>(it);
    } else {
        network = std::make_unique<NetWork>();
    }
}

NetWork::NetWork(const json11::Json& items) {
    Deserialize(items);
}

json11::Json NetWork::Serialize() const {
    return json11::Json::object{{"port", port}};
}

void NetWork::Deserialize(const json11::Json& items) {
    if (auto it = items["network"]; it.is_number()) {
        port = it.int_value();
    } else {
        port = 40000;
    }
}

}  // namespace Boot