#ifndef _SOCKETCOMMUNICATION_H_
#define _SOCKETCOMMUNICATION_H_
#include <vector>
#include <string>
#include "utils.hpp"
#include "Node.hpp"
#include "p2putils.hpp"
#include "SocketConnector.hpp"

//class Node;

/*!
 * SocketCommunication class for network and peer to peer communication.
 */
class SocketCommunication {
private:
    Node *node;

public:
    explicit SocketCommunication(Node *node);           // Constructor
    ~SocketCommunication();          // Destructor

    void startSocketCommunication(int argc, char *argv[]);
    void startP2POperations ( int argc, char **argv );
    [[noreturn]] void startP2PServer ( int argc, char **argv );
    [[noreturn]] void blockForger();
    [[noreturn]] void peerDiscoveryStatus();
    [[noreturn]] void peerDiscovery();
    void broadcastPeerDiscovery(const char *message);
    void broadcast(const char *message) const;
    static void send_node_message(int sock, const char *message);
    void receive_node_message(int sock);
    void handshake(int sock) const;
    std::string handshakeMessage() const;
    void peerDiscoveryHandleMessage(nlohmann::json &j);
    void transactionHandler(const std::string&);
    void blockHandler(const std::string&);
    void handleNodeMessage(const char*);

    std::vector<std::string> peers {}; /*!< List of peers connected to this node. */
    std::vector<std::string> inactivePeers {}; /*!< list of inactive peers, used for temporary storage */
    std::string id {}; /*!< peer to peer server id */
    SocketConnector sc {}; /*!< this node's socket connecter (ip and port) */
    const int MESSAGELENGTH = 20; /*!< Our predetermined message length for discovery messages */
};

#endif // _SOCKETCOMMUNICATION_H_
