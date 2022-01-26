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
    P2PServer p2pserver;

    SocketCommunication();           // Constructor
    ~SocketCommunication();          // Destructor

    void startSocketCommunication(int argc, char *argv[]);
    int createServer ( int argc, char **argv );
    int initializeWithMasterNode(int sock);
    void inbound_node_connected(int sock);
    void outbound_node_connected(int sock);
    void handle_incoming_node_message(int sock, const char *message);
};

#endif // _SOCKETCOMMUNICATION_H_
