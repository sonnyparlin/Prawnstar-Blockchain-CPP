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
 * P2P for random functions related to the peer to peer network.
 */
namespace P2P {
    bool isValidIpAddress(const std::string&);
    int createSocket();
    bool bindAndListen(int server, struct sockaddr_in address, int PORT);
    int setOutgoingNodeConnection(const std::string &ipaddress, int port);
    const int MESSAGELENGTH = 20; /*!< Our predetermined message length for discovery messages */
    void handleError(const std::string&);
    bool Close(int);
    const char* Recv(int);
    struct Server createP2PServer(const char* portStr);
    int Accept(P2P::Server);

    struct Server {
        struct sockaddr_in address{};
        int port{};
        int address_length{};
        std::string id{};
        int socket{};
    };
}

#endif // _P2PUTILS_H_