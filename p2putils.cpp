#include "p2putils.hpp"
#include "utils.hpp"

/*!
 *
 * @param ipAddress
 * @return bool
 *
 * Checks to make sure passed in ip address is valid using
 * inet_pton()
 */
bool p2putils::isValidIpAddress(const std::string &ipAddress)
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
int p2putils::createSocket() {
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
bool p2putils::Bind(int server, struct sockaddr_in address, int PORT) {
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
int p2putils::setOutgoingNodeConnection(const std::string &ipaddress, int port) {
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

const char* p2putils::Recv(int sock) {
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

void p2putils::handleError(const std::string &message) {
    #ifndef _WIN32
    std::cout << message << errno << std::endl;
    #else
    std::cout << message << WSAGetLastError() << std::endl;
    #endif
}

bool p2putils::Close(int sock) {
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

int p2putils::Accept(p2putils::p2pServer server) {
    int incomingSocket{};
    if ((incomingSocket = static_cast<int>(accept(server.socket,
                                                  (struct sockaddr *)&server.address,
                                                  (socklen_t*)&server.address_length))) < 0) {
        p2putils::handleError("accept() error ");
        exit(0);
    }
    return incomingSocket;
}

p2putils::p2pServer p2putils::setupP2PServerIPAndPort(const char* portStr) {
    auto PORT = std::stoi(portStr);
    std::string id = utils::get_uuid();
    int serverSocket = p2putils::createSocket();

    p2pServer s{};
    s.address.sin_family = AF_INET;
    s.address.sin_addr.s_addr = INADDR_ANY;
    s.port = PORT;
    s.address_length = sizeof(s.address);
    s.id = id;
    s.socket = serverSocket;

    if (!p2putils::Bind(s.socket, s.address, s.port))
        exit (1);
    return s;
}