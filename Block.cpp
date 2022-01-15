#include <iostream>
#include "Block.hpp"
#include "TransactionPool.hpp"
#include <nlohmann/json.hpp>

// Implemention of the construcor

Block::Block(vector <Transaction> transactions, string last_hash, 
             string hash, string forger_address, 
             unsigned long long block_count)
    : transactions(transactions), 
      last_hash(last_hash),
      hash(hash),
      forger_address(forger_address),
      block_count(block_count) {}

Block::Block(vector <Transaction> transactions, string last_hash, 
             string hash, unsigned long long block_count)
    : transactions(transactions), 
      last_hash(last_hash),
      hash(hash),
      block_count(block_count) {}

// Implementation of the destructor
Block::~Block() {}

void Block::sign(std::string sig) {
    signature = sig;
}

vector<nlohmann::json> Block::transactionList(vector <Transaction> transactions) const {
    nlohmann::json j;
    vector<nlohmann::json> txs;

    for (auto tx: transactions) {
        j["sender_address"] = tx.sender_address;
        j["receiver_address"] = tx.receiver_address;
        j["amount"] = tx.amount;
        j["type"] = tx.type;
        j["id"] = tx.id;
        j["timestamp"] = tx.timestamp;
        j["signature"] = tx.signature;
        txs.push_back(j);
    }
    return txs;
}

std::string Block::to_json() const {
    nlohmann::json j;

    j["transactions"] = transactionList(transactions);
    j["last_hash"] = last_hash;
    j["hash"] = hash;
    j["forger_address"] = forger_address;
    j["timestamp"] = timestamp;
    j["block_count"] = block_count;
    j["signature"] = signature;

    return to_string(j);
}

std::string Block::payload() const {
    nlohmann::json j;

    j["transactions"] = transactionList(transactions);
    j["last_hash"] = last_hash;
    j["hash"] = hash;
    j["forger_address"] = forger_address;
    j["timestamp"] = timestamp;
    j["block_count"] = block_count;
    j["signature"] = "";

    return to_string(j);
}

nlohmann::json Block::jsonview() const {
    nlohmann::json j;

    j["transactions"] = transactionList(transactions);
    j["last_hash"] = last_hash;
    j["hash"] = hash;
    j["forger_address"] = forger_address;
    j["timestamp"] = timestamp;
    j["block_count"] = block_count;
    j["signature"] = signature;

    return j;
}