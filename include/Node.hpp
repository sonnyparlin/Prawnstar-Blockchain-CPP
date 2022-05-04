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
    static Node *node;

public:
    // constructors/destructors
    explicit Node(char **argv); // Constructor
    ~Node();                      // Destructor

    // class methods
    static Node *createNode(char **argv);
    void startServers(int argc, char **argv);
    bool handleTransaction (Transaction&, bool broadcast=true);
    void handleBlock (Block &block, bool broadcast);
    void requestChain() const;
    void forge();
    void handleBlockchainRequest(std::string) const;
    void handleBlockchain(const std::string &blockchainString) const;
    std::string getNodeID() const;
    static std::string getConsoleLog();
    void log(std::string const& msg);

    // class members
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
    std::mutex nodeMutex;
    std::mutex logMutex;
};

#endif // _NODE_H_