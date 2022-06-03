#include "P2PServer.hpp"
#include "utils.hpp"

/*!
 *
 * @param ipAddress
 * @return bool
 *
 * Checks to make sure passed in ip address is valid using
 * inet_pton()
 */
bool P2P::isValidIpAddress(const std::string &ipAddress)
{
    struct sockaddr_in sa{};
    int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
    return result != 0;
}

/*!
 *
 * @return int
 *
 * Method to create a new socket.
 */
int P2P::createSocket() {
    int opted=1;
    int obj_server;
    if (( obj_server = socket ( AF_INET, SOCK_STREAM, 0)) == 0)
        handleError("socket() error: ");
    
    if ( setsockopt(obj_server, SOL_SOCKET, SO_REUSEADDR,
    (char *)&opted, sizeof ( opted )))
        handleError("setsockopt() error: ");

    return obj_server;
}

/*!
 *
 * @param server
 * @param address
 * @param PORT
 * @return bool
 *
 * Method for binding the server to the address and port.
 */
bool P2P::bindAndListen(int server, struct sockaddr_in address, int PORT) {
    address.sin_port = htons( PORT );

    if (bind(server, ( struct sockaddr * )&address,
    sizeof(address))<0)
    {
        handleError("bind() error: ");
        return false;
    }
    
    if (listen ( server, 3) < 0)
    {
        handleError("listen() error: ");
        return false;
    }
    return true;
}

/*!
 *
 * @param ipaddress
 * @param port
 * @return int
 *
 * Set up outgoing connection.
 */
int P2P::setOutgoingNodeConnection(const std::string &ipaddress, int port) {
    int outgoingSocket;

    if (!P2P::isValidIpAddress(ipaddress)) {
        std::cout << "Invalid master ip address in set outgoing connection" << std::endl;
        return -1;
    }

    struct sockaddr_in cli_addr{};
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_addr.s_addr = INADDR_ANY;

    if (( outgoingSocket = socket (AF_INET, SOCK_STREAM, 0 )) < 0)
    {
        handleError("socket() error ");
        return -1;
    }

    cli_addr.sin_port = htons(port);
    // Converting IPv4 and IPv6 addresses from text to binary form
    if(inet_pton ( AF_INET, ipaddress.c_str(), &cli_addr.sin_addr)<=0)
    {
        handleError("inet_pton() error ");
        return -1;
    }
    
    if ( connect( outgoingSocket, (struct sockaddr *)&cli_addr, sizeof(cli_addr )) < 0)
    {
        handleError("connect() error: ");
        return -1;
    }
    
    return outgoingSocket;
}

const char* P2P::Recv(int sock) {
    // read message body length
    unsigned long long msgLength;
    char msgLengthBuffer[MESSAGELENGTH];

    auto reader = recv (sock, msgLengthBuffer, MESSAGELENGTH, MSG_WAITALL);
    if (reader < 0) {
        handleError("recv() error ");
        return "";
    }

    msgLength = std::stoi (msgLengthBuffer);
    if (msgLength <= 0) {
        std::cout << "Message length is not valid, errno: " << errno << std::endl;
        return "";
    }

    // allocate memory with message's length
    char* buffer = new char[msgLength + 1]();

    // read socket data to the allocated buffer
    reader = recv (sock, buffer, static_cast<int>(msgLength), MSG_WAITALL);
    if (reader <= 0) {
        Close(sock);
    }

    return buffer;
}

void P2P::handleError(const std::string &message) {
    #ifndef _WIN32
    std::cout << message << errno << std::endl;
    #else
    std::cout << message << WSAGetLastError() << std::endl;
    #endif
}

bool P2P::Close(int sock) {
    #ifndef _WIN32
    int returnVal = close(sock);
    #else
    int returnVal = closesocket(sock);
    #endif

    if (returnVal != 0) {
        handleError("close() error ");
        return false;
    }
    return true;
}

int P2P::Accept(P2P::Server server) {
    int incomingSocket = static_cast<int>(accept(server.socket,
                                                  (struct sockaddr *)&server.address,
                                                  (socklen_t*)&server.address_length));
    if (incomingSocket < 0) {
        P2P::handleError("accept() error ");
        exit(0);
    }
    return incomingSocket;
}

P2P::Server P2P::startOnPort(const char* portStr) {
    auto PORT = std::stoi(portStr);
    std::string id = utils::get_uuid();
    int serverSocket = P2P::createSocket();

    Server s{};
    s.address.sin_family = AF_INET;
    s.address.sin_addr.s_addr = INADDR_ANY;
    s.port = PORT;
    s.address_length = sizeof(s.address);
    s.id = id;
    s.socket = serverSocket;

    if (!P2P::bindAndListen(s.socket, s.address, s.port))
        exit (1);
    return s;
}