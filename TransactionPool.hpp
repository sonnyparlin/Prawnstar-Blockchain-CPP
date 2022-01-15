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

    void add_transaction(Transaction transaction);
    bool transaction_exists(Transaction transaction);
};

#endif // _TRANSACTIONPOOL_H_