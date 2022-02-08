#ifndef _NODE_H_
#define _NODE_H_
#include <vector>
#include <string>
#include "TransactionPool.hpp"
#include "Wallet.hpp"
#include "Blockchain.hpp"
#include "SocketCommunication.hpp"
#include "AccountModel.hpp"

class Node {
public:
    TransactionPool transactionPool;
    Wallet wallet;
    Blockchain blockchain;
    AccountModel accountModel;
    int argc;
    char **argv;

    Node();
    Node(int argc, char **argv); // Constructor
    ~Node();                      // Destructor

    void startServers(int argc, char **argv);
    void handleTransaction (Transaction);
};

#endif // _NODE_H_