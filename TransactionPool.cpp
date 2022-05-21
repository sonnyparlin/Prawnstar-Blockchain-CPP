#include "TransactionPool.hpp"

TransactionPool::TransactionPool()=default;
TransactionPool::~TransactionPool()=default;

/*!
 *
 * @param transaction
 *
 * Add a transaction to the transaction pool.
 */
void TransactionPool::addTransaction(const Transaction &transaction) {
    std::lock_guard<std::mutex> guard(tpoolMutex);
    transactions.push_back(transaction);
}

/*!
 *
 * @param transaction
 * @return bool
 *
 * Check if the transaction already exists in the pool.
 */
bool TransactionPool::transactionExists(const Transaction &transaction) {
    std::lock_guard<std::mutex> guard(tpoolMutex);
    for (auto const &itx : transactions) {
        if (itx == transaction) {
            std::cout << "Transaction already exists in the pool" << std::endl;
            return true;
        }
    }
    return false;
}

/*!
 *
 * @param txs
 *
 * Remove the list of given transactions from the transaction pool.
 */
void TransactionPool::removeFromPool(const std::vector<Transaction> &txs) {
    std::lock_guard<std::mutex> guard(tpoolMutex);
    std::vector<Transaction> newPoolTransactions;
    for (auto const &poolTransaction : this->transactions) {
        bool insert = true;
        for (auto const &transaction : txs) {
            if (poolTransaction == transaction)
                insert = false;
        }
        if (insert)
            newPoolTransactions.push_back(poolTransaction);
    }
    transactions = newPoolTransactions;
}

/*!
 *
 * @return std::string
 *
 * Stringified json representation of the transactions in the pool.
 */
std::string TransactionPool::getPoolTransactionsJsonString() {
    std::lock_guard<std::mutex> guard(tpoolMutex);
    nlohmann::json j;
    nlohmann::json jContainer;

    for (auto &transaction : transactions) {
        j = nlohmann::json::parse(transaction.toJson());
        jContainer.push_back(j);
    }

    std::string jsonDump = jContainer.dump();    
    return jsonDump;
}

/*!
 *
 * @return bool
 *
 * Check to see if a forger is required. We require a forger after 20 transactions
 * have been added to the pool or if 300 seconds has passed, see
 * SocketCommunication::blockForger().
 */
bool TransactionPool::forgerRequired() {
    for (auto const &itx : transactions) {
        if (itx.type == "EXCHANGE" || itx.type == "STAKE")
            return true;
    }
    return transactions.size() >= 20;
}