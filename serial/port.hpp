#pragma once
#ifndef __PORT_HEADER_GUARD__
#define __PORT_HEADER_GUARD__
#include <stdint.h>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>
namespace Serial {

class Session;

class SerialPort final {
    friend class Session;

public:
    // 定数
    constexpr static size_t receive_size{256};
    constexpr static char newline{'\r'};
    constexpr static unsigned int timeout_ms = 10;  //[ms]
    // 命令及び要求
    constexpr static uint8_t request_disconnect = 0xFE;
    constexpr static uint8_t command_disconnect = 0xFF;
    constexpr static inline uint8_t command_select(uint8_t address) {
        return address | 0x80;
    }

    const std::string path;

private:
    boost::asio::io_service io_service;
    boost::asio::serial_port port;
    bool is_opened{false};  //資源管理

public:
    SerialPort(const std::string& _path, int baud = B115200);
    SerialPort() = delete;
    ~SerialPort() = default;
    std::unique_ptr<Session> CreateSession(
        unsigned int address);  //警告開けない場合はnull
private:
    void ReleaseSession();
    bool Connect(uint8_t address);  //成功なら真
    bool Disconnect();
    std::optional<std::string> Transfer(const std::string&);
    void SendCommand(uint8_t c);  // cを送信する
    bool WaitCommand(uint8_t c);  // cを受信するまで
};

using SerialPortPointer = std::shared_ptr<SerialPort>;

static inline SerialPortPointer CreateSerialPortPointer(const std::string& path,
                                                        int baud = B115200) {
    return std::make_shared<SerialPort>(path, baud);
}

// SessionはSerialPortにつき一つまで実体化できる。open&closeを自動化できるぞ
class Session final {
public:
    const uint8_t address;

private:
    SerialPort& port;
    mutable bool active;

public:
    Session(SerialPort&, uint8_t _address);
    Session(const Session&) = delete;
    ~Session();
    inline bool IsActive() const {
        return active;
    }
    std::optional<std::string> Transfer(const std::string& line) {
        if (active) {
            return port.Transfer(line);
        } else {
            return std::nullopt;
        }
    }
};

}  // namespace Serial
#endif