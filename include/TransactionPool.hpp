#ifndef _TRANSACTIONPOOL_H_
#define _TRANSACTIONPOOL_H_
#include <vector>
#include <string>
#include "Transaction.hpp"
#include <crow.h>
#include <mutex>
#include <thread>
#include <nlohmann/json.hpp>
#include <iostream>

class TransactionPool {
public:
    std::vector<Transaction> transactions;
    std::mutex tpoolMutex;

    TransactionPool();           // Constructor
    ~TransactionPool();          // Destructor

    void addTransaction(const Transaction &);
    bool transactionExists(const Transaction&);
    void removeFromPool(const vector<Transaction>&);
    std::string getPoolTransactionsJsonString();
    bool forgerRequired();
};

#endif // _TRANSACTIONPOOL_H_