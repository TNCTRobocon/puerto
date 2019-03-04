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
    std::cout << response << std::endl;
    thread.join();
}

void Server::Transfer() {
    zmq::socket_t nearSocket(context, zmq::socket_type::rep);
    zmq::socket_t farSocket(context, zmq::socket_type::rep);

    nearSocket.bind(nearLocation);
    farSocket.bind(farLocation);

    zmq::pollitem_t items[2]{{nearSocket, 0, ZMQ_POLLIN, 0}, {farSocket, 0, ZMQ_POLLIN, 0}};
    while (1) {
        int n = zmq::poll(items, 2, -1);
        std::cout << "test" << n << std::endl;
        if (items[0].revents & ZMQ_POLLIN) {
            zmq::message_t message;
            nearSocket.recv(&message);
            std::string request((char*)message.data(), message.size());
            std::cout << request << std::endl;
            nearSocket.send(request.data(), request.size());
            if (request == "quit") return;
        }
        if (items[1].revents & ZMQ_POLLIN) {
            zmq::message_t message;
            farSocket.recv(&message);
            std::string request((char*)message.data(), message.size());
            std::cout << request << std::endl;
            farSocket.send(request.data(), request.size());
        }
    }
}
}  // namespace Net