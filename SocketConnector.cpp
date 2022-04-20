#include "SocketConnector.hpp"

#include <utility>

SocketConnector::SocketConnector()=default;
SocketConnector::SocketConnector(std::string ip, int port)
: ip(std::move(ip)), port(port) {}
SocketConnector::~SocketConnector()=default;