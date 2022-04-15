#ifndef _P2PUTILS_H_
#define _P2PUTILS_H_

#include <stdio.h>

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
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <cstring>
#include <limits.h>
#include <errno.h>
#include <thread>
#include <sstream>
#include <unordered_map>
#include <future>
#include <nlohmann/json.hpp>

namespace p2putils {
    void logit(std::string message);
    bool isValidIpAddress(std::string ipAddress);
    int createSocket();
    bool Bind(int server, struct sockaddr_in address, int PORT);
    int setOutgoingNodeConnection(std::string ipaddress, int port);
    char *sock_ntop (const struct sockaddr *sa, socklen_t salen);
}

#endif // _P2PUTILS_H_