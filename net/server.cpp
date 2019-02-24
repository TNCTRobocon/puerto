#include "server.hpp"
#include <boost/format.hpp>
#include <iostream>
namespace Net {

Server::Server(unsigned int port) {
    context = std::make_unique<zmq::context_t>();
    socket = std::make_unique<zmq::socket_t>(*context, zmq::socket_type::rep);
    location = (boost::format("tcp://*:%1%") % port).str();
}

void Server::operator()() {
    using namespace std;
    socket->bind(location);
    zmq::message_t msg;
    for (;;) {
        socket->recv(&msg);
        cout << msg.str();
        string res="test b";
        socket->send(res.data(),res.size());
    }
}

}  // namespace Net