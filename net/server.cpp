#include "server.hpp"
#include <algorithm>
#include <boost/format.hpp>
#include <exception>
#include <iostream>
#include <map>
namespace Net {
using namespace json11;
using namespace std;

ExternalServer::ExternalServer(const std::string& _endpoint, int _port)
    : endpoint(_endpoint), port(_port), thread(&ExternalServer::Communicate, this) {}

ExternalServer::~ExternalServer() {
    thread.join();
}

void ExternalServer::Communicate() {
    cout << endpoint << endl;
}

Server::Server(const std::string& name, int port) {
    std::string endpoint = (boost::format("ipc:///tmp/%1%") % name).str();
    internal.emplace(endpoint);
    external.emplace(endpoint, port);
}

}  // namespace Net