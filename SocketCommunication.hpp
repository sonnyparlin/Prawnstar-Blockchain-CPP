#ifndef _SOCKETCOMMUNICATION_H_
#define _SOCKETCOMMUNICATION_H_
#include <vector>
#include <string>
#include "utils.hpp"
#include "Node.hpp"
#include "p2putils.hpp"
#include "p2pserver.hpp"
 
class SocketCommunication {
public:
    std::unordered_map<std::string, int> peers;

    SocketCommunication();           // Constructor
    ~SocketCommunication();          // Destructor

    void startSocketCommunication(int argc, char *argv[]);
    int createServer ( int argc, char **argv );
    int initializeWithMasterNode(int sock);
    void inbound_node_connected(int sock);
    void outbound_node_connected(int sock);
    int processArgs(int argc, char **argv);
    void handle_incoming_node_message(int sock, const char *message);
};

class PeerDiscoveryHandler {
public:

    PeerDiscoveryHandler(SocketCommunication *sc); // Constructor
    ~PeerDiscoveryHandler();                       // Destructor

    void start(); // Start
    void status(); // Status
    void discovery(); // Discovery
};

#endif // _SOCKETCOMMUNICATION_H_
