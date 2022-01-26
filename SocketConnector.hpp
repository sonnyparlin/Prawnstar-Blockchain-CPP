#ifndef _SOCKETCONNECTOR_H_
#define _SOCKETCONNECTOR_H_
#include <iostream>
#include <string>

class SocketConnector {
public:
    std::string ip;
    int port;

    SocketConnector();
    ~SocketConnector();

    bool equals(const SocketConnector sc);
};

#endif // _SOCKETCONNECTOR_H_