#ifndef _NODE_H_
#define _NODE_H_
#include <vector>
#include <string>
#include "TransactionPool.hpp"
#include "Wallet.hpp"
#include "Blockchain.hpp"

class Node {
public:
    TransactionPool transactionPool;
    Wallet wallet;
    Blockchain blockchain;

    Node();           // Constructor
    ~Node();          // Destructor
};

#endif // _NODE_H_