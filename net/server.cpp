#include "server.hpp"
#include <boost/format.hpp>

namespace Net {

Server::Server(unsigned int port) {
    context = std::make_unique<zmq::context_t>();
    socket = std::make_unique<zmq::socket_t>(*context, zmq::socket_type::rep);
    const auto location = (boost::format("tcp://*:%1%") % port).str();
    socket->bind(location);
}

void Server::Run() {
    while (1) {
        zmq::message_t message;
        socket->recv(&message);
        
    }
}

}  // namespace Net