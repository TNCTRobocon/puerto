#include "server.hpp"
#include <boost/format.hpp>
#include <iostream>
#include <exception>
namespace Net {

Server::Server(unsigned int port) {
    using namespace std;
    location = (boost::format("tcp://*:%1%") % port).str();
    runner = make_unique<thread>(&Server::Transfer, this, location);
}

Server::~Server() {
    killer = true;
    runner->join();
}

void Server::Transfer(std::string location) {
    zmq::context_t context;
    zmq::socket_t socket(context, zmq::socket_type::rep);
    try {
        socket.bind(location);
    }catch (zmq::error_t error){
        using namespace std;
        cerr<<error.what()<<endl;
        return;
    }
    zmq::message_t message;
    std::string mes = "hello";
    while (!killer) {
        socket.recv(&message);
        std::cout << message.str() << std::endl;
        socket.send(mes.data(), mes.size());
    }
}

}  // namespace Net