#include "port.hpp"
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
namespace Serial {
using namespace std;

Port::Port(const std::string& _path, unsigned int bps, size_t buffer_size)
    : path(_path) {
    // create buffer
    buffer = new unsigned char[buffer_size];
    if (buffer == nullptr) {
        cerr << "Error : " << buffer_size << "[Byte] does not reserve" << endl;
    }
    // open serial port
    fd = open(_path.c_str(), O_RDWR);
    if (fd < 0) {
        cerr << "Error :" << path << "does not open" << endl;
        return;
    }
    // make configuration
    termios tio;
    memset(&tio, 0, sizeof(tio));
    tio.c_cflag |= CREAD;    // enable receiving
    tio.c_cflag |= CLOCAL;   // disable mordem
    tio.c_cflag |= CS8;      // bit width is 8 bit
    tio.c_cflag |= 0;        // stop bit is 0 bit
    tio.c_cflag |= 0;        // parity is none
    cfsetispeed(&tio, bps);  // set band
    cfsetospeed(&tio, bps);
    cfmakeraw(&tio);  // raw mode
    // apply device
    tcsetattr(fd, TCSANOW, &tio);
    tio.c_cc[VTIME] = 1;  // timeout [1/10 s]
    tio.c_cc[VMIN] = 1;   // minunm size 1[byte]
    ioctl(fd, TCSETS, &tio);
    // send initilize command
    constexpr static unsigned char reset = 254u;
    if (write(fd, &reset, sizeof(reset) != sizeof(reset))) {
        cerr << "Warn:" << path << "does not send reset command." << endl;
    }
}

Port::~Port() {
    if (buffer != nullptr) {
        delete[] buffer;
    }
    if (fd >= 0) {
        close(fd);
    }
}

void Port::Communicate() {
    auto connect = [this](unsigned int address) {
        const uint8_t target = (address & 0x7f) | 0x80;
        // clean up
        tcdrain(fd);  //書き込み中のデータをすべて書き込み
        tcflush(fd, TCIFLUSH);  //読み込み前のデータを破棄

        //接続要求を送信する
        if (write(fd, &address, sizeof(address)) != sizeof(address)) {
            cerr << "Error" << path << "does not write" << endl;
            return false;  //書き込み失敗
        }

        //接続許可を受信する
        uint8_t host;
        if (read(fd, &host, sizeof(host)) != sizeof(host)) {
            cerr << "Error" << path << ":" << address << "lost responce"
                 << endl;
            return false;
        }
        if (host != 0x81) {
            cerr << "Error" << path << ":" << address << "is bad responce"
                 << endl;
            return false;
        }
        return true;
    };

    auto disconnect = [this](unsigned int address) {
        constexpr static uint8_t request = 0xFE;
        //切断要求
        if (write(fd, &request, sizeof(request)) != sizeof(request)) {
            cerr << "Error" << path << "does not write" << endl;
            return;  //書き込み失敗
        }
        //切断許可
        constexpr static uint8_t kill = 0xFF;
        uint8_t responce = 0;
        while (responce != kill) {
            if (read(fd, &responce, sizeof(responce) != sizeof(responce))) {
                cerr << "Error" << path << ":" << address
                     << "lost responce for disconnection" << endl;
                break;
            }
        }
        //切断
        if (write(fd, &kill, sizeof(kill)) != sizeof(kill)) {
            cerr << "Error" << path << ":"<<address<<"does not kill" << endl;
        }
    };

    auto kill = [this]() {
        constexpr static uint8_t kill = 0xFF;
        //切断
        if (write(fd, &kill, sizeof(kill)) != sizeof(kill)) {
            cerr << "Error" << path <<"does not kill" << endl;
        }
    };

    for (auto& session : list_session) {
        if (!connect(session->address)){
            kill();
        }
        static constexpr char* newline="\r";

        for (auto &pair:session->orders){
            string message =pair.second+newline;
            if (write(fd,message.c_str(),message.length()!=message.length())){
                cerr<<"Error:"<<path<<"does not write"<<endl;
                break;
            }
            


        }   
    }
    list_session.clear();
}

}  // namespace Serial
