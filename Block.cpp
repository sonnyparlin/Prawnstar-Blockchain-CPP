#include <iostream>
#include "Block.hpp"
#include "TransactionPool.hpp"
#include <nlohmann/json.hpp>

// Implemention of the construcor

Block::Block(vector <Transaction> transactions, string lastHash, 
             string forgerAddress, 
             unsigned long long blockCount)
    : transactions(transactions), 
      lastHash(lastHash),
      forgerAddress(forgerAddress),
      blockCount(blockCount) {}

Block::Block(vector <Transaction> transactions, string lastHash,  
             unsigned long long blockCount)
    : transactions(transactions), 
      lastHash(lastHash),
      blockCount(blockCount) {}

// Implementation of the destructor
Block::~Block() {}

void Block::sign(std::string sig) {
    signature = sig;
}

vector<nlohmann::json> Block::transactionList(vector <Transaction> transactions) const {
    nlohmann::json j;
    vector<nlohmann::json> txs;

    for (auto tx: transactions) {
        j["senderAddress"] = tx.senderAddress;
        j["receiverAddress"] = tx.receiverAddress;
        j["amount"] = tx.amount;
        j["type"] = tx.type;
        j["id"] = tx.id;
        j["timestamp"] = tx.timestamp;
        j["signature"] = tx.signature;
        txs.push_back(j);
    }
    return txs;
}

std::string Block::toJson() const {
    nlohmann::json j;

    j["transactions"] = transactionList(transactions);
    j["lastHash"] = lastHash;
    j["hash"] = hash;
    j["forgerAddress"] = forgerAddress;
    j["timestamp"] = timestamp;
    j["blockCount"] = blockCount;
    j["signature"] = signature;

    return to_string(j);
}

std::string Block::payload() {
    nlohmann::json j;

    j["transactions"] = transactionList(transactions);
    j["lastHash"] = lastHash;
    j["hash"] = hash;
    j["forgerAddress"] = forgerAddress;
    j["timestamp"] = timestamp;
    j["blockCount"] = blockCount;
    j["signature"] = "";

    return to_string(j);
}

nlohmann::json Block::jsonView() const {
    nlohmann::json j;

    j["transactions"] = transactionList(transactions);
    j["lastHash"] = lastHash;
    j["hash"] = hash;
    j["forgerAddress"] = forgerAddress;
    j["timestamp"] = timestamp;
    j["blockCount"] = blockCount;
    j["signature"] = signature;

    return j;
}