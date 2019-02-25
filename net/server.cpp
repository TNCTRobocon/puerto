#include "server.hpp"
#include <algorithm>
#include <boost/format.hpp>
#include <exception>
#include <iostream>
#include <map>
namespace Net {
using namespace json11;

Server::Server(unsigned int port) {
    location = (boost::format("tcp://*:%1%") % port).str();
}

Server::~Server() {
    Stop();
}

void Server::Start() {
    if (!runner) {
        runner =
            std::make_unique<std::thread>(&Server::Transfer, this, location);
    }
}

void Server::Stop() {
    if (runner) {
        killer = true;
        runner->join();
        runner.reset();
    }
}

void Server::Add(const std::string& name, IAdapterPointer adapter) {
    if (!runner) {
        adapters.push_back(
            std::pair<std::string, IAdapterPointer>{name, adapter});
    }
}
void Server::Transfer(std::string location) {
    zmq::context_t context;
    zmq::socket_t socket(context, zmq::socket_type::rep);
    try {
        socket.bind(location);
    } catch (zmq::error_t error) {
        using namespace std;
        cerr << error.what() << endl;
        return;
    }
    while (!killer) {
        zmq::message_t message;
        std::string error;
        socket.recv(&message);
        const Json request = Json::parse(message.str(), error);
        Json response;
        if (!error.empty()) {
            using namespace std;
            cerr << error << endl;
            response = Json::object{{"error", error}};
        } else {
            response = Apply(request);
        }
        std::string text = response.dump();
        socket.send(text.data(), text.size());
    }
}

Json Server::Apply(const Json& request) {
    std::map<std::string, Json> merge;
    for (auto [name, adapter] : adapters) {
        if (auto it =request[name];it.is_object()){
            merge[name]=adapter->Apply(it);
        }
    }

    return Json{merge};
}
/*
Json Server::ApplyMotors(const Json& motors) {
    std::map<std::string, Json> merge;
    //単一モーター
    auto list = motors.object_items();
    for (const auto& [key, value] : list) {
        //命名規則"#[0-9]*"に従っているか?
        if (key[0] != '#') {
            continue;
        }
        //
    }

    return Json::object{};
}
*/
}  // namespace Net