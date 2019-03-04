#pragma once
#ifndef __SERVER_HEADER_GUARD__
#define __SERVER_HEADER_GUARD__

#include <memory>
#include <string>
#include <thread>
#include <zmq.hpp>

namespace Net {

class Server final {
    zmq::context_t context;  //必ず先頭に置くこと
    const std::string nearLocation, farLocation;
    std::thread thread;

public:
    Server(const std::string& host = "*", int port = 40000);
    Server(const Server&) = delete;
    ~Server();

private:
    void Transfer();  //制約 thread以外から呼び出してはならない
};

}  // namespace Net

#endif