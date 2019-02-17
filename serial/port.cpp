#include "port.hpp"
#include <algorithm>
#include <boost/timer.hpp>
#include <iostream>
namespace Serial {
namespace asio = boost::asio;

constexpr char SerialPort::newline;
constexpr unsigned int SerialPort::timeout_ms;
constexpr uint8_t SerialPort::request_disconnect;
constexpr uint8_t SerialPort::command_disconnect;

SerialPort::SerialPort(const std::string& _path, int baud)
    : port(io_service, _path), path(_path) {
    // serial portを初期化する
    port.set_option(asio::serial_port_base::baud_rate(baud));
    port.set_option(asio::serial_port_base::character_size(8));
    port.set_option(asio::serial_port_base::flow_control(
        asio::serial_port_base::flow_control::none));
    port.set_option(
        asio::serial_port_base::parity(asio::serial_port_base::parity::none));
    port.set_option(asio::serial_port_base::stop_bits(
        asio::serial_port_base::stop_bits::one));
}

std::unique_ptr<Session> SerialPort::CreateSession(unsigned int address) {
    // address 範囲を確認する
    if (0x02 >= address || address >= 0x80) {
        return nullptr;
    }
    // 資源があるか
    if (!is_opened) {
        return nullptr;
    }
    // 生成
    auto instance =std::make_unique<Session>(*this, address);
    if (!instance->IsActive()){
        return nullptr;
    }
    // 確保
    is_opened=true;
    return std::move(instance);

}

void SerialPort::ReleaseSession() {
    // 資源を開放する
    is_opened = false;
}

bool SerialPort::Connect(uint8_t address) {
    using namespace std;
    // address 形式に直す
    SendCommand(address | 0x80);
    //応答確認 (hostが選択されるまで待つ)
    return WaitCommand(0x81);
}

bool SerialPort::Disconnect() {
    // 切断要求
    SendCommand(request_disconnect);
    // 切断待ち
    bool result = WaitCommand(command_disconnect);
    // 切断
    SendCommand(command_disconnect);
    return result;
}

std::optional<std::string> SerialPort::Transfer(const std::string& line) {
    //送信
    std::string request = line + newline;
    port.write_some(asio::buffer(request));

    //受信
    bool is_connected{false};
    asio::deadline_timer timer(io_service);
    timer.expires_from_now(boost::posix_time::microseconds(timeout_ms));
    timer.async_wait([this, &is_connected](boost::system::error_code& error) {
        if (error != boost::asio::error::operation_aborted) {
            port.cancel();  //接続キャンセル
            is_connected = true;
        }
    });
    asio::streambuf response;
    asio::async_read_until(
        port, response, newline,
        [&timer](boost::system::error_code& error, size_t size) {
            if (error != boost::asio::error::operation_aborted) {
                timer.cancel();
            }
        });
    port.get_io_service().run();
    //整形
    if (!is_connected) {
        return std::nullopt;
    } else {
        return asio::buffer_cast<const char*>(response.data());
    }
}

void SerialPort::SendCommand(uint8_t c) {
    port.write_some(asio::buffer(&c, sizeof(c)));
}

bool SerialPort::WaitCommand(uint8_t c) {
    bool is_connected{false};
    asio::deadline_timer timer(io_service);
    timer.expires_from_now(boost::posix_time::microseconds(timeout_ms));
    timer.async_wait([this, &is_connected](boost::system::error_code& error) {
        if (error != boost::asio::error::operation_aborted) {
            port.cancel();  //接続キャンセル
            is_connected = true;
        }
    });
    asio::streambuf response;
    asio::async_read_until(
        port, response, 0x80,
        [&timer](boost::system::error_code& error, size_t size) {
            if (error != boost::asio::error::operation_aborted) {
                timer.cancel();
            }
        });
    port.get_io_service().run();
    return is_connected;
}

Session::Session(SerialPort& _port, uint8_t _address)
    : port(_port), address(_address) {
    active = port.Connect(address);
}

Session::~Session(){
    if (active){
        port.Disconnect();
        port.ReleaseSession();
    }
}



}  // namespace Serial
