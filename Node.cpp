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

void Node::handleTransaction (Transaction transaction) {
    std::string data = transaction.payload();
    std::string signature = transaction.signature;
    std::string signerPublicKey = transaction.senderPublicKey;
    bool signatureValid = utils::verifySignature(data, signature, signerPublicKey);
    bool transactionExists = transactionPool.transactionExists(transaction);
    if (!transactionExists && signatureValid)
        transactionPool.addTransaction(transaction);
}