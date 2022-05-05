#ifndef _P2PUTILS_H_
#define _P2PUTILS_H_

#include <cstdio>

#include <unistd.h>

#ifndef _WIN32
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#else
#include <Ws2tcpip.h>
#include <Winsock2.h>
#endif

#include <pthread.h>

#include <string>
#include <cstdlib>
#include <cerrno>
#include <iostream>
#include <cstring>
#include <climits>
#include <thread>
#include <sstream>
#include <unordered_map>
#include <future>
#include <nlohmann/json.hpp>

/*!
 * p2putils for random functions related to the peer to peer network.
 */
namespace p2putils {
    bool isValidIpAddress(const std::string&);
    int createSocket();
    bool Bind(int server, struct sockaddr_in address, int PORT);
    int setOutgoingNodeConnection(const std::string &ipaddress, int port);
}

#endif // _P2PUTILS_H_