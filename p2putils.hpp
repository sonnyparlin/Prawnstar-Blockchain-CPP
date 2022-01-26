#ifndef _P2PUTILS_H_
#define _P2PUTILS_H_

#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <cstring>
#include <pthread.h>
#include <limits.h>
#include <arpa/inet.h>
#include <errno.h>
#include <thread>
#include <sstream>
#include <unordered_map>
#include <future>
#include <cryptopp/hex.h>

namespace p2putils {
    void logit(std::string message);
    bool isValidIpAddress(const char *ipAddress);
    int createSocket();
    bool Bind(int server, struct sockaddr_in address, int PORT);
    std::string uuid_gen();
    int setOutgoingNodeConnection(const char *ipaddress);
    char *sock_ntop (const struct sockaddr *sa, socklen_t salen);
}

#endif // _P2PUTILS_H_