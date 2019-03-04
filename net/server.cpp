#include "server.hpp"
#include <boost/format.hpp>
#include <iostream>

namespace Net {

Server::Server(const ServerHandler& _handler, const std::string& host, int port)
    : handler(_handler),
      nearLocation((boost::format("inproc://%1%") % port).str()),
      farLocation((boost::format("tcp://%1%:%2%") % host % port).str()),
      thread(&Server::Transfer, this) {}

Server::~Server() {
    zmq::socket_t nearSocket(context, zmq::socket_type::req);
    nearSocket.connect(nearLocation);
    std::string request = "quit";
    nearSocket.send(request.data(), request.size());
    zmq::message_t message;
    nearSocket.recv(&message);
    thread.join();
}

void Server::Transfer() {
    zmq::socket_t nearSocket(context, zmq::socket_type::rep);
    zmq::socket_t farSocket(context, zmq::socket_type::rep);

    nearSocket.bind(nearLocation);
    farSocket.bind(farLocation);

    zmq::pollitem_t items[2]{{nearSocket, 0, ZMQ_POLLIN, 0}, {farSocket, 0, ZMQ_POLLIN, 0}};
    while (1) {
        zmq::poll(items, 2, -1);
        if (items[0].revents & ZMQ_POLLIN) {
            zmq::message_t message;
            nearSocket.recv(&message);
            std::string request((char*)message.data(), message.size());
            nearSocket.send(request.data(), request.size());
            if (request == "quit") return;
        }
        if (items[1].revents & ZMQ_POLLIN) {
            zmq::message_t message;
            farSocket.recv(&message);
            std::string request((char*)message.data(), message.size());
            std::string response=handler(request);
            farSocket.send(response.data(), response.size());
        }
    }
}
}  // namespace Net