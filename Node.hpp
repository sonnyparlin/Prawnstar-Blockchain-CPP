#ifndef _NODE_H_
#define _NODE_H_
#include <vector>
#include <string>
#include "TransactionPool.hpp"
#include "Wallet.hpp"
#include "Blockchain.hpp"
#include "SocketCommunication.hpp"
#include "NodeApi.hpp"

class Node {
public:
    TransactionPool transactionPool;
    Wallet wallet;
    Blockchain blockchain;
    int argc;
    char *argv[];

    Node(int argc, char *argv[]); // Constructor
    ~Node();                      // Destructor

    void startServers(int argc, char *argv[]);
};

#endif // _NODE_H_