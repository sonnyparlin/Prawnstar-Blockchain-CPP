#ifndef _NODE_H_
#define _NODE_H_
#include <vector>
#include <string>
#include "TransactionPool.hpp"
#include "Wallet.hpp"
#include "Blockchain.hpp"
#include "Block.hpp"
#include "Message.hpp"
#include "AccountModel.hpp"
#include "SocketCommunication.hpp"
#include "ProofOfStake.hpp"
#include <iostream>
#include <fstream>
#include <mutex>

class SocketCommunication;
class Blockchain;
class Wallet;
class ProofOfStake;
class AccountModel;

class Node {
private:
    Node(int argc, char **argv); // Constructor
    static Node *node;

public:
    TransactionPool transactionPool;
    Wallet *nodeWallet;
    Wallet *exchangeWallet;
    Wallet *bobWallet;
    Wallet *aliceWallet;
    Wallet *node2Wallet;
    Blockchain *blockchain;
    AccountModel *accountModel;
    SocketCommunication *p2p;
    ProofOfStake *proofOfStake;

    int argc;
    char **argv;
    
    ~Node();                      // Destructor

    static Node *createNode(int argc, char **argv);
    void startServers(int argc, char **argv);
    bool handleTransaction (Transaction, bool broadcast=true);
    void handleBlock (Block block, bool broadcast);
    void requestChain();
    void forge();
    void handleBlockchainRequest(std::string);
    void handleBlockchain(std::string blockchainString);
};

#endif // _NODE_H_