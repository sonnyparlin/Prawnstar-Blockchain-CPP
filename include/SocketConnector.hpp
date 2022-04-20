#ifndef _SOCKETCONNECTOR_H_
#define _SOCKETCONNECTOR_H_
#include <iostream>
#include <string>

class SocketConnector {
public:
    std::string ip{};
    int port{};

    SocketConnector(std::string, int);
    SocketConnector();
    ~SocketConnector();
};

#endif // _SOCKETCONNECTOR_H_