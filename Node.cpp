#include "Node.hpp"
#include "NodeApi.hpp"
#include <crow.h>

Node * Node::node=nullptr;

Node::Node(int argc, char **argv) {
    p2p = new SocketCommunication(this);
    accountModel = new AccountModel(this);
    blockchain = new Blockchain(this);

    int port = utils::getPort(argv[2]);
    std::cout << "port is: " << port << std::endl;

    if (port == utils::MASTER_NODE_PORT)
        wallet = new Wallet(true, "private.ec.der", this);
    else
        wallet = new Wallet(this);

    exchangeWallet = new Wallet("exchangePrivate.ec.der", this);
    aliceWallet = new Wallet("aliceWalletPrivate.ec.der", this);
    bobWallet = new Wallet("bobWalletPrivate.ec.der", this);

    std::cout << "walletAddress: " << wallet->toJson() << std::endl;
    std::cout << "exchangeAddress: " << exchangeWallet->toJson() << std::endl;
    std::cout << "aliceAddress: " << aliceWallet->toJson() << std::endl;
    std::cout << "bobAddress: " << bobWallet->toJson() << std::endl;

    proofOfStake = new ProofOfStake(this, port);
}

Node::~Node() {
    delete p2p;
    delete blockchain;
    delete wallet;
    delete proofOfStake;
    delete accountModel;
    delete exchangeWallet;
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

bool Node::handleTransaction (Transaction transaction, bool broadcast ) {
    std::string data = transaction.payload();
    std::string signature = transaction.signature;
    std::string signerPublicKey = transaction.senderPublicKey;
    bool signatureValid = utils::verifySignature(data, signature, signerPublicKey);
    bool transactionExists = transactionPool.transactionExists(transaction);
    bool transactionInBlock = blockchain->transactionExists(transaction);
    if (!transactionExists && signatureValid && !transactionInBlock) {
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
        bool forgingRequired = transactionPool.forgerRequired();
        if (forgingRequired)
            forge();
        
        return true;
    }
    return false;
}

void Node::forge() {
    std::string forger = blockchain->nextForger();
    if (forger == wallet->walletPublicKey) {
        std::cout << "i am the next forger" << std::endl;
        try {
            Block block = blockchain->createBlock(transactionPool.transactions, wallet->address); 
            transactionPool.removeFromPool(block.transactions);
            Message message("BLOCK", block.toJson());
            std::string msgJson = message.toJson();
            p2p->broadcast(msgJson.c_str());
        } catch (std::exception &e) {std::cerr << "exception: " << e.what() << std::endl; }
    } else
        std::cout << "i am not the next forger" << std::endl;
}