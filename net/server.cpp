#include "server.hpp"
#include <algorithm>
#include <boost/format.hpp>
#include <exception>
#include <iostream>
#include <map>
namespace Net {
std::optional<std::string> InternalServer::Communicate(const std::string& request) {
    zmq::context_t context;
    zmq::socket_t socket(context, zmq::socket_type::req);
    try {
        zmq::message_t message;
        socket.connect(endpoint);
        socket.send(request.data(), request.size());
        socket.recv(&message);
        return std::string((char*)message.data(), message.size());
    } catch (...) {
        return std::nullopt;
    }
}

ExternalServer::ExternalServer(const std::string& _endpoint_global,
                               const std::string _endpoint_local)
    : endpoint_global(_endpoint_global),
      endpoint_local(_endpoint_local),
      thread(&ExternalServer::Communicate, this) {}

ExternalServer::~ExternalServer() {
    thread.join();
}

void ExternalServer::Communicate() {
    zmq::context_t context;
    zmq::socket_t global(context, zmq::socket_type::rep);
    zmq::socket_t local(context, zmq::socket_type::rep);

    // bind
    try {
        global.bind(endpoint_global);
        local.bind(endpoint_local);

    } catch (...) {
        return;
    }
    zmq::pollitem_t items[2] = {{(void*)global, 0, ZMQ_POLLIN, 0},
                                {(void*)local, 0, ZMQ_POLLIN, 0}};

    while (1) {
        zmq_poll(items, 2, -1);
        if (items[0].events & ZMQ_POLLIN) {
            zmq::message_t message;
            global.recv(&message);
            std::string request((char*)message.data(), message.size());
            std::cout << message << std::endl;
            std::string response = "hello";
            global.send(response.data(), response.size());
        }
        if (items[1].events & ZMQ_POLLIN) {
            zmq::message_t message;
            local.recv(&message);
            std::string request((char*)message.data(), message.size());
            std::cout << message << std::endl;
            std::string response = "hello";
            local.send(response.data(), response.size());
        }
    }
}

Server::Server(const std::string& path, const std::string& host, int port) {
    std::string global = (boost::format("tcp://%1%:%2%") % host % port).str();
    std::string local = (boost::format("ipc:///tmp/%1%") % path).str();
    internal.emplace(local);
    external.emplace(global, local);
}

}  // namespace Net