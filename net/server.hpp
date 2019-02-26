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

class IAdapter {
public:
    IAdapter() = default;
    IAdapter(const IAdapter&) = default;
    virtual ~IAdapter() = default;
    virtual json11::Json Apply(const json11::Json&) = 0;
};

using IAdapterPointer = std::shared_ptr<IAdapter>;

class Server final {
    std::string location;
    std::unique_ptr<std::thread> runner;
    std::atomic_bool killer{false};                                 // runnerの動作を止める
    std::vector<std::pair<std::string, IAdapterPointer>> adapters;  //動作中には追加されません
    zmq::context_t context;


public:
    Server(unsigned int port);
    Server(const Server&) = delete;
    ~Server();
    void Start();
    void Stop();
    void Add(const std::string& name, IAdapterPointer adpater);

private:
    void Transfer(std::string location);
    json11::Json Apply(const json11::Json&);
};

}  // namespace Net

#endif