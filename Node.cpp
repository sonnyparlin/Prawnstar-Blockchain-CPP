#include "Node.hpp"

Node::Node(int argc, char *argv[]) {
}

Node::~Node() {
}

void Node::startP2P(int argc, char *argv[]) {
    SocketCommunication p2p;
    p2p.startSocketCommunication(argc, argv);
}