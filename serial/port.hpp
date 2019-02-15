#pragma once
#ifndef __PORT_HEADER_GUARD__
#define __PORT_HEADER_GUARD__
#include <boost/asio.hpp>
#include <deque>
#include <memory>
#include <mutex>
#include <string>
namespace Serial {

//仮想的に1対1の接続を行うクラス
class SerialPort;
class Session final {
public:
    friend class SerialPort;
    const int address;

private:
    mutable std::mutex lock;
    std::deque<std::string> send;
    std::deque<std::string> receive;

public:
    Session(int _address) : address(_address) {}
    Session(const Session&) = delete;
    ~Session() = default;
    bool Send(const std::string& line);  //失敗したらfalse
    bool Receive(std::string& line);     //失敗したらfalse
    bool IsConnected() const;            //通信が終わっていたらtrue
    bool Clear();                        //消すことができたらtrue
};

class SerialPort final {
public:
    constexpr static char newline{'\r'};
private:
    boost::asio::io_service service;
    boost::asio::serial_port port;
    std::vector<std::shared_ptr<Session>> list;
    std::mutex lock;

public:
    SerialPort(const std::string& path, int baud = B115200);
    SerialPort() = delete;
    ~SerialPort() = default;
    bool Add(std::shared_ptr<Session> session);
    bool Remove(std::shared_ptr<Session> target);
    bool Process();
};

}  // namespace Serial
#endif