#pragma once
#ifndef __SERVER_HEADER_GUARD__
#define __SERVER_HEADER_GUARD__

#include <atomic>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <tuple>
#include <utils/json11.hpp>
#include <vector>
#include <zmq.hpp>

namespace Net {

class Server final {
    zmq::context_t context;
    const std::string nearLocation,farLocation;
    std::thread thread;


public:
    Server(const std::string& host = "*", int port = 40000);
    Server(const Server&) = delete;
    ~Server();
    void Transfer();
};

}  // namespace Net

#endif