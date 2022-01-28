#ifndef _SOCKETCOMMUNICATION_H_
#define _SOCKETCOMMUNICATION_H_
#include <vector>
#include <string>
#include "utils.hpp"
#include "Node.hpp"
#include "p2putils.hpp"
#include "SocketConnector.hpp"
 
class SocketCommunication {
public:
    std::unordered_map<char *, int> peers;
    SocketConnector sc;

    SocketCommunication();           // Constructor
    ~SocketCommunication();          // Destructor

    void startSocketCommunication(int argc, char *argv[]);
    void startP2POperations ( int argc, char **argv );
    int startP2PServer ( int argc, char **argv );
    void handshake(int sock);
    void broadcast(const char *message);
    void inbound_node_connected(int sock);
    void outbound_node_connected(int sock);
    int processArgs(int argc, char **argv);
    void sendToNode(int sock, const char *message);
    void startPeerDiscovery(); // Start
    void peerDiscoveryStatus(); // Status
    void peerDiscovery(); // Discovery
    std::string handshakeMessage();
};

#endif // _SOCKETCOMMUNICATION_H_
