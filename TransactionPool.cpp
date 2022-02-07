#include <iostream>
#include "TransactionPool.hpp"
#include <nlohmann/json.hpp>

TransactionPool::TransactionPool() {
}

TransactionPool::~TransactionPool() {
}

void TransactionPool::addTransaction(Transaction transaction) {
    transactions.push_back(transaction);
}

bool TransactionPool::transactionExists(Transaction transaction) {
    for (auto tx : transactions) {
        if (tx.equals(transaction)) {
            std::cout << "Transaction already exists in the pool" << std::endl;
            return true;
        }
    }
    return false;
}

void TransactionPool::removeFromPool(vector<Transaction> txs) {
    vector<Transaction> newPoolTransactions;
    for (auto poolTransaction : this->transactions) {
        bool insert = true;
        for (auto transaction : txs) {
            if (poolTransaction.equals(transaction))
                insert = false;
        }
        if (insert == true)
            newPoolTransactions.push_back(poolTransaction);
    }
    transactions = newPoolTransactions;
}

std::string TransactionPool::getPoolTransactionsJsonString() {
   nlohmann::json j;

    for (auto transaction : transactions) {
        j.push_back(transaction.toJson());
    }
    
    return j.dump();
}