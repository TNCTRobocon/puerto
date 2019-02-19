#pragma once
#ifndef __SERVER_HEADER_GUARD__
#define __SERVER_HEADER_GURAD__

#include <memory>
#include <string>
#include <zmq.hpp>
namespace Net {

class Server final{
    std::unique_ptr<zmq::context_t> context{nullptr};
    std::unique_ptr<zmq::socket_t> socket{nullptr};

public:
    Server(unsigned int port);
    Server(const Server&)=delete;
    ~Server()=default;
    void Run();
};

}  // namespace Net

#endif