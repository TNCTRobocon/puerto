#include "port.hpp"
#include <algorithm>
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

constexpr char SerialPort::newline;

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

bool SerialPort::Add(std::shared_ptr<Session> session) {
    unique_lock<mutex> block(lock, defer_lock);
    if (block.try_lock()) {
        list.emplace_back(session);
        return true;
    }
    return false;
}

bool SerialPort::Remove(std::shared_ptr<Session> target) {
    unique_lock<mutex> block(lock, defer_lock);
    if (block.try_lock()) {
        list.erase(remove(list.begin(), list.end(), target), list.end());
        return true;
    }
    return false;
}

bool SerialPort::Process() {
    unique_lock<mutex> block(lock, defer_lock);
    if (block.try_lock()) {
        for (shared_ptr<Session> session : list) {
            // Open
            unique_lock<mutex> block(session->lock);
            uint8_t target = (session->address & 0x7f) | 0x80;
            port.write_some(target);
            // TODO あとで真面目に書く
            // Write All
            for (const auto& line : session->send) {
                auto tmp = line + newline;
                port.write_some(buffer(tmp));
            }
            // Read All Until 0x80
            string line;
            while (1) {
                uint8_t c;
                port.read_some(buffer(&c, 1));
                switch (c) {
                    case 0x80:
                        break;
                    case '\r':
                        session->receive.emplace_back(line);
                        line = "";
                        break;
                    default:
                        line += c;
                }
            }
        }
        // Close
        port.write_some(0x80);
        return true;
    }
    return false;
}

}  // namespace Serial
