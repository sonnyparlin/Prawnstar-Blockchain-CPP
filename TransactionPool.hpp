#ifndef _TRANSACTIONPOOL_H_
#define _TRANSACTIONPOOL_H_
#include <vector>
#include <string>
#include "Transaction.hpp"
#include <crow.h>

class TransactionPool {
public:
    std::vector<Transaction> transactions;

    TransactionPool();           // Constructor
    ~TransactionPool();          // Destructor

    void addTransaction(Transaction transaction);
    bool transactionExists(Transaction transaction);
    void removeFromPool(vector<Transaction> txs);
    std::string getPoolTransactionsJsonString();
    bool forgerRequired();
};

#endif // _TRANSACTIONPOOL_H_