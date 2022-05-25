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
    return std::any_of(transactions.begin(), transactions.end(),
                                                        [&transaction](const Transaction& tx) {
        if (transaction == tx)
            return true;
        return false;
    });
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
    std::for_each(transactions.begin(), transactions.end(),
                  [&txs, &newPoolTransactions](const Transaction& poolTransaction){
        bool insert = true;
        std::for_each(txs.begin(), txs.end(), [&insert, &poolTransaction](auto const &transaction){
            if (poolTransaction == transaction)
                insert = false;
        });
        if (insert)
            newPoolTransactions.push_back(poolTransaction);
    });
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

    std::for_each (transactions.begin(), transactions.end(), [&jContainer, &j](auto &transaction){
        j = nlohmann::json::parse(transaction.toJson());
        jContainer.push_back(j);
    });

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