#include "TransactionPool.hpp"

TransactionPool::TransactionPool()=default;
TransactionPool::~TransactionPool()=default;

void TransactionPool::addTransaction(const Transaction &transaction) {
    std::lock_guard<std::mutex> guard(tpoolMutex);
    transactions.push_back(transaction);
}

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

void TransactionPool::removeFromPool(const vector<Transaction> &txs) {
    std::lock_guard<std::mutex> guard(tpoolMutex);
    vector<Transaction> newPoolTransactions;
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

bool TransactionPool::forgerRequired() {
    for (auto const &itx : transactions) {
        if (itx.type == "EXCHANGE" || itx.type == "STAKE")
            return true;
    }
    return transactions.size() >= 20;
}