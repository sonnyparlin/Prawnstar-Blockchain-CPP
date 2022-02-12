#ifndef _NODE_H_
#define _NODE_H_
#include <vector>
#include <string>
#include "TransactionPool.hpp"
#include "Wallet.hpp"
#include "Blockchain.hpp"
#include "Message.hpp"
#include "AccountModel.hpp"
#include "SocketCommunication.hpp"
#include <mutex>

class SocketCommunication;

class Node {
private:
    Node();
    Node(int argc, char **argv); // Constructor
    static Node *node;

public:
    TransactionPool transactionPool;
    Wallet wallet;
    Blockchain blockchain;
    AccountModel accountModel;
    SocketCommunication *p2p;

    int argc;
    char **argv;
    
    ~Node();                      // Destructor

    static Node *createNode(int argc, char **argv);
    void startServers(int argc, char **argv);
    void handleTransaction (Transaction, bool broadcast=true);
};

#endif // _NODE_H_