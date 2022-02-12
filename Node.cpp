#include "Node.hpp"
#include "NodeApi.hpp"
#include <crow.h>

Node * Node::node=nullptr;

Node::Node() {
}

Node::Node(int argc, char **argv) {
    std::cout << "Node address: " << this << std::endl;
    std::cout << "transactionPool transactions address: " << &(this->transactionPool.transactions) << std::endl;
    std::cout << "transactionPool address: " << &(this->transactionPool) << std::endl;
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

// Node *Node::createNode() {
//     if (node != nullptr)
//         return node;
//     else 
//         std::cerr << "Node creation failed" << std::endl;

//     return nullptr;
// }


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

    std::cout << "handleTransaction transactions address: " << &transactionPool.transactions << std::endl;

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

        std::cout << "======================================" << std::endl;
        for (auto tx : transactionPool.transactions)
            std::cout << tx.toJson() << std::endl;
        std::cout << "======================================" << std::endl;

        if (broadcast) {
            Message message("TRANSACTION", transaction.toJson());
            std::string msgJson = message.toJson();
            p2p->broadcast(msgJson.c_str());
        }
    }
}