#include "Node.hpp"
#include "NodeApi.hpp"

Node::Node() {
}

Node::Node(int argc, char **argv) {
}

Node::~Node() {
}

void Node::startServers(int argc, char **argv) {
    SocketCommunication p2p;
    p2p.startSocketCommunication(argc, argv);
    
    NodeApi api;
    if (argc == 4) {
        std::string po = std::string(argv[3]);
        api.start(po);
    } else if (argc == 5) {
        std::string po = std::string(argv[4]);
        api.start(po);
    }
}