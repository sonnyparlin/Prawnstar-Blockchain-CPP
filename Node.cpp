#include "Node.hpp"
#include "NodeApi.hpp"
#include <crow.h>

Node * Node::node=nullptr;

Node::Node(int argc, char **argv) {
    p2p = new SocketCommunication(this);
}

Node::~Node() {
    delete p2p;
}

Node *Node::createNode(int argc, char **argv) {
    if (node == nullptr) {
        node = new Node(argc, argv);
    }
    return node;
}

void Node::startServers(int argc, char **argv) {
    p2p->startSocketCommunication(argc, argv);
    
    NodeApi api(this);
    if (argc == 4) {
        std::string po = std::string(argv[3]);
        api.start(po);
    } else if (argc == 5) {
        std::string po = std::string(argv[4]);
        api.start(po);
    }
}

void Node::handleTransaction (Transaction transaction, bool broadcast ) {
    std::string data = transaction.payload();
    std::string signature = transaction.signature;
    std::string signerPublicKey = transaction.senderPublicKey;
    bool signatureValid = utils::verifySignature(data, signature, signerPublicKey);
    bool transactionExists = transactionPool.transactionExists(transaction);
    if (!transactionExists && signatureValid) {
        std::cout << "Adding transaction " 
                  << transaction.id 
                  << " to the pool on " 
                  << p2p->sc.ip << ":" << p2p->sc.port 
                  << std::endl;
        transactionPool.addTransaction(transaction);
        if (broadcast) {
            Message message("TRANSACTION", transaction.toJson());
            std::string msgJson = message.toJson();
            p2p->broadcast(msgJson.c_str());
        }
    }
}