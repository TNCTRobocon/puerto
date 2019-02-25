#pragma once
#ifndef __SERVER_HEADER_GUARD__
#define __SERVER_HEADER_GURAD__

#include <memory>
#include <string>
#include <thread>
#include <optional>
#include <zmq.hpp>
#include <atomic>
namespace Net {

class Server final {
    std::string location;
    std::unique_ptr<std::thread> runner;
    std::atomic_bool killer{false};//runnerの動作を止める
public:
    Server(unsigned int port);
    Server(const Server&) = delete;
    ~Server() ;

private:
    void Transfer(std::string location);
};

}  // namespace Net

#endif