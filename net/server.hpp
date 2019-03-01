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
    std::string endpoint;  //プロセス通信用エンドポイント

public:
    InternalServer(const std::string& _endpoint) : endpoint(_endpoint) {}
    InternalServer(const InternalServer&) = delete;
    virtual ~InternalServer() = default;
    std::optional<std::string> Communicate(const std::string&);
};

// 外部と通信するクラス(別スレッドで動作する)
class ExternalServer {
    std::string endpoint_global;  //外部通信用エンドポイント
    std::string endpoint_local;   //プロセス通信用エンドポイント
    std::thread thread;

public:
    ExternalServer(const std::string& _endpoint_global, const std::string _endpoint_local);
    ExternalServer(const ExternalServer&) = delete;
    virtual ~ExternalServer();
    void Communicate();
};

class Server {
    int port;
    std::optional<InternalServer> internal{std::nullopt};
    std::optional<ExternalServer> external{std::nullopt};

public:
    Server(const std::string& path,const std::string& host, int port);
};

}  // namespace Net

#endif