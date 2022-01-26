#include "SocketConnector.hpp"

SocketConnector::SocketConnector() {}
SocketConnector::~SocketConnector() {}

bool SocketConnector::equals(const SocketConnector sc) {
    return sc.ip == ip && sc.port == port;
}