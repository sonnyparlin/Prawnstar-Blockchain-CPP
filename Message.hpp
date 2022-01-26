#ifndef _MESSAGE_H_
#define _MESSAGE_H_
#include "SocketConnector.hpp"
#include <string>

class Message {
public:
    SocketConnector sc;
    std::string messageType;
    std::string data;

    Message();
    ~Message();

};

#endif // _MESSAGE_H_