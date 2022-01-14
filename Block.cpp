#include <iostream>
#include "Block.hpp"
#include "TransactionPool.hpp"

// Implemention of the construcor

Block::Block(vector <TransactionPool> transactions, string last_hash, 
             string hash, string forger_address, time_t timestamp, 
             unsigned long long block_count)
    : transactions(transactions), 
      last_hash(last_hash),
      hash(hash),
      forger_address(forger_address),
      timestamp(timestamp),
      block_count(block_count) {
}

// Implementation of the destructor
Block::~Block() {}

// Implementation of the display method
// should just insert the block attributes to cout

void Block::display() const {
    std::cout << "Block class" << std::endl;
}
