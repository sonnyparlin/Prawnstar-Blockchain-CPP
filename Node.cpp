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

    if (port == 10001)
        nodeWallet = new Wallet(this, true, "genesisNode.pem");
    else
        nodeWallet = new Wallet(this);

    exchangeWallet = new Wallet(this, true, "exchange.pem");
    aliceWallet = new Wallet(this, true, "alice.pem");
    bobWallet = new Wallet(this, true, "bob.pem"); 

    std::cout << "walletAddress: " << nodeWallet->address << std::endl;
    std::cout << "exchangeAddress: " << exchangeWallet->address << std::endl;
    std::cout << "aliceAddress: " << aliceWallet->address << std::endl;
    std::cout << "bobAddress: " << bobWallet->address << std::endl;

    proofOfStake = new ProofOfStake(this, port);
}

Node::~Node() {
    delete p2p;
    delete blockchain;
    delete nodeWallet;
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

void Node::handleBlock (Block block, bool broadcast) {
    Wallet forger(block.forgerAddress.c_str(), this);

    // std::cout << "forger: " << forger.toJson() << std::endl;

    std::string blockHash = block.hash;
    std::string signature = block.signature;

    bool blockCountValid = blockchain->blockCountValid(block);
    bool lastBlockHashValid = blockchain->lastBlockHashValid(block);
    bool forgerValid = blockchain->forgerValid(block);
    bool transactionValid = blockchain->transactionValid(block.transactions);
    bool signatureValid = utils::verifySignature(block.payload(), block.signature, forger.walletPublicKey);

    if (lastBlockHashValid && forgerValid && 
        transactionValid && signatureValid && blockCountValid) {
        blockchain->addBlock(block);
        transactionPool.removeFromPool(block.transactions);
        
        if (broadcast) {
            Message message("BLOCK", block.toJson());
            std::string msgJson = message.toJson();
            p2p->broadcast(msgJson.c_str());
        }
    } else {
        std::cerr << "Block verification failed" << std::endl;
        std::cerr << "lastBlockHashValid: " << std::boolalpha << lastBlockHashValid << std::endl;
        std::cerr << "forgerValid: " << std::boolalpha << forgerValid << std::endl;
        std::cerr << "transactionValid: " << std::boolalpha << transactionValid << std::endl;
        std::cerr << "signatureValid: " << std::boolalpha << signatureValid << std::endl;
    }
}

void Node::forge() {
    std::string forger = blockchain->nextForger();
    if (forger == nodeWallet->walletPublicKey) {
        std::cout << "i am the next forger" << std::endl;
        try {
            Block block = blockchain->createBlock(transactionPool.transactions, nodeWallet->address); 
            transactionPool.removeFromPool(block.transactions);
            Message message("BLOCK", block.toJson());
            std::string msgJson = message.toJson();
            p2p->broadcast(msgJson.c_str());
        } catch (std::exception &e) {std::cerr << "exception: " << e.what() << std::endl; }
    } else
        std::cout << "i am not the next forger" << std::endl;
}