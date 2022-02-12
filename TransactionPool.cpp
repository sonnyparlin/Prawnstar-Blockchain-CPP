#include <iostream>
#include "TransactionPool.hpp"
#include <nlohmann/json.hpp>
#include <thread>

TransactionPool::TransactionPool() {
}

TransactionPool::~TransactionPool() {
     std::cout << "Calling destructor of transactionPool" << std::endl;
}

void TransactionPool::addTransaction(Transaction transaction) {
    //std::cout << "Size before adding: " << transactions.size() << std::endl;
    transactions.push_back(transaction);
    // std::cout << "transactionPool address: " << this << std::endl;
    // std::cout << "transactions address: " << &transactions << std::endl;
    // std::cout << "Size after adding: " << transactions.size() << std::endl;
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
    nlohmann::json jContainer;

    //std::cout << "inside getPoolTransactionsJsonString() txs size: " << transactions.size() << std::endl;

    for (auto transaction : transactions) {
        j = nlohmann::json::parse(transaction.toJson());
        jContainer.push_back(j);
    }

    // std::cout << "transactionPool address inside transaction-pool endpoint: " << this << std::endl;

    // std::cout << "transactions address inside transaction-pool endpoint: " << &transactions << std::endl;

    std::string jsonDump = jContainer.dump();

    // std::cout << "after dump inside getPoolTransactionsJsonString() txs size: " << transactions.size() << std::endl;
    
    return jsonDump;
}