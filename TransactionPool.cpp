#include <iostream>
#include "TransactionPool.hpp"

TransactionPool::TransactionPool() {
}

TransactionPool::~TransactionPool() {
}

void TransactionPool::add_transaction(Transaction transaction) {
    transactions.push_back(transaction);
}

bool TransactionPool::transaction_exists(Transaction transaction) {
    for (auto tx : transactions) {
        if (tx.equals(transaction)) {
            std::cout << "Transaction already exists in the pool" << std::endl;
            return true;
        }
    }
    return false;
}