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

/*!
 * TransactionPool, this is where transactions go before they land in a block.
 */
class TransactionPool {
public:
    TransactionPool();           // Constructor
    ~TransactionPool();          // Destructor

    void addTransaction(const Transaction &);
    bool transactionExists(const Transaction&);
    void removeFromPool(const vector<Transaction>&);
    std::string getPoolTransactionsJsonString();
    bool forgerRequired();

    std::vector<Transaction> transactions {}; /*!< Pool transaction list */
    std::mutex tpoolMutex {}; /*!< mutex for protecting the pool transactions */
};

#endif // _TRANSACTIONPOOL_H_