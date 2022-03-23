#ifndef _SOCKETCOMMUNICATION_H_
#define _SOCKETCOMMUNICATION_H_
#include <vector>
#include <string>
#include "utils.hpp"
#include "Node.hpp"
#include "p2putils.hpp"
#include "SocketConnector.hpp"

class Node;

class SocketCommunication {
private:
    Node *node;

public:
    std::vector<std::string> peers;
    std::vector<std::string> inactivePeers;
    const int MESSAGELENGTH = 20;
    std::string id;

    SocketConnector sc;

    SocketCommunication(Node *node);           // Constructor
    ~SocketCommunication();          // Destructor

    void startSocketCommunication(int argc, char *argv[]);
    void startP2POperations ( int argc, char **argv );
    int startP2PServer ( int argc, char **argv );
    void handshake(int sock);
    void broadcastPeerDiscovery(const char *message);
    void broadcast(const char *message);
    void inbound_node_connected(int sock);
    void outbound_node_connected(int sock);
    int processArgs(int argc, char **argv);
    void send_node_message(int sock, const char *message);
    void receive_node_message(int sock);
    void peerDiscoveryStatus(); // Status
    void peerDiscovery(); // Discovery
    std::string handshakeMessage();
    void peerDiscoveryHandleMessage(const char *message);
};

#endif // _SOCKETCOMMUNICATION_H_
