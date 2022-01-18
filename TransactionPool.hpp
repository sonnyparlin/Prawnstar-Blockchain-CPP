#ifndef _TRANSACTIONPOOL_H_
#define _TRANSACTIONPOOL_H_
#include <vector>
#include <string>
#include "Transaction.hpp"

class TransactionPool {
public:
    std::vector<Transaction> transactions;

    TransactionPool();           // Constructor
    ~TransactionPool();          // Destructor

    void addTransaction(Transaction transaction);
    bool transactionExists(Transaction transaction);
    void removeFromPool(vector<Transaction> txs);
};

#endif // _TRANSACTIONPOOL_H_