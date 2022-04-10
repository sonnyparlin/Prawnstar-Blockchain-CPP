#include <iostream>
#include "TransactionPool.hpp"
#include <nlohmann/json.hpp>
#include <thread>
#include <chrono>

TransactionPool::TransactionPool() {
}

TransactionPool::~TransactionPool() {
}

void TransactionPool::addTransaction(Transaction transaction) {
    std::lock_guard<std::mutex> guard(tpoolMutex);
    transactions.push_back(transaction);
}

bool TransactionPool::transactionExists(Transaction transaction) {
    std::lock_guard<std::mutex> guard(tpoolMutex);
    for (auto tx : transactions) {
        if (tx == transaction) {
            std::cout << "Transaction already exists in the pool" << std::endl;
            return true;
        }
    }
    return false;
}

void TransactionPool::removeFromPool(vector<Transaction> txs) {
    std::lock_guard<std::mutex> guard(tpoolMutex);
    vector<Transaction> newPoolTransactions;
    for (auto poolTransaction : this->transactions) {
        bool insert = true;
        for (auto transaction : txs) {
            if (poolTransaction == transaction)
                insert = false;
        }
        if (insert == true)
            newPoolTransactions.push_back(poolTransaction);
    }
    transactions = newPoolTransactions;
}

std::string TransactionPool::getPoolTransactionsJsonString() {
    std::lock_guard<std::mutex> guard(tpoolMutex);
    nlohmann::json j;
    nlohmann::json jContainer;

    for (auto transaction : transactions) {
        j = nlohmann::json::parse(transaction.toJson());
        jContainer.push_back(j);
    }

    std::string jsonDump = jContainer.dump();    
    return jsonDump;
}

bool TransactionPool::forgerRequired() {
    for (auto tx : transactions) {
        if (tx.type == "EXCHANGE" || tx.type == "STAKE")
            return true;
        
        auto now = std::chrono::system_clock::now();
        time_t current_time = std::chrono::system_clock::to_time_t( now );
        time_t start_time = tx.timestamp;
        float difference = current_time - start_time;
        
        if ((difference / 60) > 1)
            return true;
    }
    return transactions.size() >= 20;
}