#pragma once
#ifndef __SERVER_HEADER_GUARD__
#define __SERVER_HEADER_GURAD__

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

// メインスレッドから操作するときに使うクラス
class InternalServer {
    zmq::context_t context;
    std::string endpoint;  //プロセス通信用

public:
    InternalServer(const std::string& _endpoint) : endpoint(_endpoint) {}
    InternalServer(const InternalServer&) = delete;
    virtual ~InternalServer()=default;
};

// 外部と通信するクラス(別スレッドで動作する)
class ExternalServer {
    zmq::context_t context;
    std::string endpoint;  //プロセス通信用
    int port;              //外部に開放するポート
    std::thread thread;

public:
    ExternalServer(const std::string& _endpoint, int _port);
    ExternalServer(const ExternalServer&) = delete;
    virtual ~ExternalServer();
    void Communicate();
};

class Server {
    int port;
    std::optional<InternalServer> internal{std::nullopt};
    std::optional<ExternalServer> external{std::nullopt};

public:
    Server(const std::string& name, int port);
};

}  // namespace Net

#endif