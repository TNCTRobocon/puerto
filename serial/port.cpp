#include "port.hpp"

namespace Serial {
using namespace std;
using namespace boost::asio;

bool Session::Send(const std::string& line) {
    unique_lock<mutex> block(lock, defer_lock);
    if (block.try_lock()) {
        send.emplace_back(line);
        return true;
    }
    return false;
}

bool Session::Receive(std::string& line) {
    unique_lock<mutex> block(lock, defer_lock);
    if (block.try_lock()) {
        if (!receive.empty()) {
            line = move(receive.front());
            receive.pop_front();
            return true;
        }
    }
    return false;
}

bool Session::IsConnected() const {
    unique_lock<mutex> block(lock, defer_lock);
    if (block.try_lock()) {
        return !receive.empty();
    }
    return false;
}

bool Session::Clear() {
    unique_lock<mutex> block(lock, defer_lock);
    if (block.try_lock()) {
        send.clear();
        receive.clear();
        return true;
    }
    return false;
}

SerialPort::SerialPort(const std::string& path, int baud)
    : port(service, path) {
    // serial portを初期化する
    port.set_option(serial_port_base::baud_rate(baud));
    port.set_option(serial_port_base::character_size(8));
    port.set_option(
        serial_port_base::flow_control(serial_port_base::flow_control::none));
    port.set_option(serial_port_base::parity(serial_port_base::parity::none));
    port.set_option(
        serial_port_base::stop_bits(serial_port_base::stop_bits::one));
}

}  // namespace Serial
