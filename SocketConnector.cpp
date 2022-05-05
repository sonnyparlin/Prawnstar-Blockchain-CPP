#include "SocketConnector.hpp"

#include <utility>

SocketConnector::SocketConnector()=default;
/*!
 *
 * @param ip
 * @param port
 *
 * Node's socket connector constructor
 */
SocketConnector::SocketConnector(std::string ip, int port)
: ip(std::move(ip)), port(port) {}
SocketConnector::~SocketConnector()=default;