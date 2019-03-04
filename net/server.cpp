#include "server.hpp"
#include <algorithm>
#include <boost/format.hpp>
#include <exception>
#include <iostream>
#include <map>
namespace Net {

Server::Server(const std::string& host, int port)
    : nearLocation((boost::format("inproc://%1%") % port).str()),
      farLocation((boost::format("tcp://%1%:%2%") % host % port).str()),
      thread(&Server::Transfer, this) {}

Server::~Server() {
    zmq::socket_t nearSocket(context, zmq::socket_type::req);
    nearSocket.connect(nearLocation);
    std::string request = "quit";
    nearSocket.send(request.data(), request.size());
    zmq::message_t message;
    nearSocket.recv(&message);
    std::string response((char*)message.data(), message.size());
    std::cout << message << std::endl;
    thread.join();
}

void Server::Transfer() {
    zmq::socket_t nearSocket(context, zmq::socket_type::rep);
    nearSocket.bind(nearLocation);
    zmq::message_t message;
    while (1) {
        nearSocket.recv(&message);
        std::string request((char*)message.data(), message.size());
        std::cout << request << std::endl;
        nearSocket.send(request.data(), request.size());
        if (request == "quit") return;
    }
}

}  // namespace Net