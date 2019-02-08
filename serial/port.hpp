#pragma once
#ifndef __PORT_HEADER_GUARD__
#define __PORT_HEADER_GUARD__
#include <stdint.h>
#include <functional>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
namespace Serial {
class Port;

using receiver = std::function<void(const std::string&)>;
class Session final{
    friend class Port;
    std::vector<std::pair<receiver, std::string>> orders;
public:
    const unsigned int address;
    Session (unsigned int _address):address(_address){}
    Session* Add(receiver _receiver,const std::string& _message){
        orders.emplace_back(std::pair(_receiver,_message));
        return this;
    }//method chain対応
};

static inline std::unique_ptr<Session>CreateSession(unsigned int address){
    return std::make_unique<Session>(address);
}

class Port final{
    std::vector<std::unique_ptr<Session>> list_session;
    int fd;
    unsigned char *buffer;
    const std::string path; 
public:
    Port(const std::string& _path,unsigned int bps=115200,size_t buffer_size=256);
    Port(const Port&)=delete;
    ~Port();
    void Join(std::unique_ptr<Session> session){
        list_session.emplace_back(std::move(session));
    }
    void Communicate();
    bool IsEnable()const{
        return fd>0;
    }
};

}  // namespace Serial
#endif