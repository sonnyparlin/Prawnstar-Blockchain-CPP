#include "SocketConnector.hpp"

SocketConnector::SocketConnector(){}
SocketConnector::SocketConnector(std::string ip, int port) 
    : ip(ip), port(port) {}
SocketConnector::~SocketConnector() {}

bool SocketConnector::equals(const SocketConnector sc) {
    return sc.ip == ip && sc.port == port;
}