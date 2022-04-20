#include "p2putils.hpp"

namespace p2putils {

bool isValidIpAddress(const std::string &ipAddress)
{
    struct sockaddr_in sa{};
    int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
    return result != 0;
}

int createSocket() {
    int opted=1;
    int obj_server;
    if (( obj_server = socket ( AF_INET, SOCK_STREAM, 0)) == 0)
    {
        #ifndef _WIN32
        std::cerr << "socket: " << errno << std::endl;
        #else
        printf("\nsocket: %d\n", WSAGetLastError());
        #endif
    }
    
    if ( setsockopt(obj_server, SOL_SOCKET, SO_REUSEADDR,
    (char *)&opted, sizeof ( opted )))
    {
        #ifndef _WIN32
        std::cerr << "setsockopt: " << errno << std::endl;
        #else
        printf("\nsetsockopt: %d\n", WSAGetLastError());
        #endif
    }
    return obj_server;
}

bool Bind(int server, struct sockaddr_in address, int PORT) {
    address.sin_port = htons( PORT );

    if (bind(server, ( struct sockaddr * )&address,
    sizeof(address))<0)
    {
        #ifndef _WIN32
        std::cerr << "bind: " << errno << std::endl;
        #else
        printf("\nbind: %d\n", WSAGetLastError());
        #endif
        return false;
    }
    
    if (listen ( server, 3) < 0)
    {
        #ifndef _WIN32
        std::cerr << "listen: " << errno << std::endl;
        #else
        printf("\nlisten: %d\n", WSAGetLastError());
        #endif
        return false;
    }
    return true;
}

int setOutgoingNodeConnection(const std::string &ipaddress, int port) {
    int outgoingSocket;

    if (!p2putils::isValidIpAddress(ipaddress)) {
        std::cout << "Invalid master ip address in set outgoing connection" << std::endl;
        return -1;
    }

    struct sockaddr_in cli_addr{};
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_addr.s_addr = INADDR_ANY;

    if (( outgoingSocket = socket (AF_INET, SOCK_STREAM, 0 )) < 0)
    {
        #ifndef _WIN32
        std::cout << "socket() error " << errno << std::endl;
        #else
        printf("\nsocket() error: %d\n", WSAGetLastError());
        #endif
        return -1;
    }

    cli_addr.sin_port = htons(port);
    // Converting IPv4 and IPv6 addresses from text to binary form
    if(inet_pton ( AF_INET, ipaddress.c_str(), &cli_addr.sin_addr)<=0)
    {
        #ifndef _WIN32
        std::cout << "inet_pton() error " << errno << std::endl;
        #else
        printf("\ninet_pton() error: %d\n", WSAGetLastError());
        #endif
        return -1;
    }
    
    if ( connect( outgoingSocket, (struct sockaddr *)&cli_addr, sizeof(cli_addr )) < 0)
    {
        #ifndef _WIN32
        std::cout << "connect() error: " << errno << std::endl;
        #else
        printf("\n\nconnect() error: %d\n", WSAGetLastError());
        #endif
        return -1;
    }
    
    return outgoingSocket;
}

} // namespace