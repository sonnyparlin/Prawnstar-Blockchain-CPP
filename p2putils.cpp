#include "p2putils.hpp"

namespace p2putils {

void logit(std::string message) {
    std::cout << message << std::endl;
}

bool isValidIpAddress(std::string ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
    return result != 0;
}

int createSocket() {
    int opted=1;
    int obj_server;
    if (( obj_server = socket ( AF_INET, SOCK_STREAM, 0)) == 0)
    {
        p2putils::logit ( "Opening of Socket Failed !");
        exit ( EXIT_FAILURE);
    }
    
    if ( setsockopt(obj_server, SOL_SOCKET, SO_REUSEADDR,
    &opted, sizeof ( opted )))
    {
        p2putils::logit ( "Can't set the socket" );
        exit ( EXIT_FAILURE );
    }
    return obj_server;
}

bool Bind(int server, struct sockaddr_in address, int PORT) {
    address.sin_port = htons( PORT );

    if (bind(server, ( struct sockaddr * )&address,
    sizeof(address))<0)
    {
        p2putils::logit ( "Binding of socket failed !" );
        exit(EXIT_FAILURE);
    }
    
    if (listen ( server, 3) < 0)
    {
        p2putils::logit ( "Can't listen from the server !");
        exit(EXIT_FAILURE);
    }
    return 1;
}

int setOutgoingNodeConnection(std::string ipaddress, int port) {
    int outgoingSocket;

    if (!p2putils::isValidIpAddress(ipaddress)) {
        std::cout << "Invalid master ip address in set outgoing connection" << std::endl;
        return -1;
    }

    struct sockaddr_in cli_addr;
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_addr.s_addr = INADDR_ANY;    
    inet_pton(AF_INET, ipaddress.c_str(), &(cli_addr));
    if (( outgoingSocket = socket (AF_INET, SOCK_STREAM, 0 )) < 0)
    {
        printf ( "Socket creation error !" );
        return -1;
    }

    cli_addr.sin_port = htons(port);
    // Converting IPv4 and IPv6 addresses from text to binary form
    if(inet_pton ( AF_INET, ipaddress.c_str(), &cli_addr.sin_addr)<=0)
    {
        printf ( "\nInvalid address ! This IP Address is not supported !\n" );
        return -1;
    }
    
    if ( connect( outgoingSocket, (struct sockaddr *)&cli_addr, sizeof(cli_addr )) < 0)
    {
        std::cout << "Connection Failed : when connecting to " << ipaddress.c_str() << ":" << std::to_string(port) << std::endl;
        return -1;
    }
    
    return outgoingSocket;
}

} // namespace