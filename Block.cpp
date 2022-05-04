#include <iostream>
#include "Block.hpp"
#include "TransactionPool.hpp"
#include <nlohmann/json.hpp>

// Implemention of the construcor
Block::Block()=default;

Block::Block(std::vector <Transaction> transactions,
             std::string lastHash,
             std::string forgerAddress,
             long blockCount)
    : transactions(std::move(transactions)),
      lastHash(std::move(lastHash)),
      forgerAddress(std::move(forgerAddress)),
      blockCount(blockCount) {}

Block::Block(std::vector <Transaction> transactions,
             std::string lastHash,
             long blockCount)
    : transactions(std::move(transactions)),
      lastHash(std::move(lastHash)),
      blockCount(blockCount) {}

// Implementation of the destructor
Block::~Block()=default;

void Block::sign(std::string sig) {
    signature = std::move(sig);
}

vector<nlohmann::json> Block::transactionList(vector <Transaction> &transactions) {
    nlohmann::json j;
    vector<nlohmann::json> txs;

    for (auto const &itx: transactions) {
        j["senderAddress"] = itx.senderAddress;
        j["receiverAddress"] = itx.receiverAddress;
        j["amount"] = itx.amount;
        j["type"] = itx.type;
        j["id"] = itx.id;
        j["timestamp"] = itx.timestamp;
        j["signature"] = itx.signature;
        txs.push_back(j);
    }
    return txs;
}

std::string Block::toJson() {
    nlohmann::json j;

    j["transactions"] = transactionList(transactions);
    j["lastHash"] = lastHash;
    j["hash"] = hash;
    j["forgerAddress"] = forgerAddress;
    j["timestamp"] = timestamp;
    j["blockCount"] = blockCount;
    j["signature"] = signature;
    j["_id"] = _id;

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
    j["_id"] = _id;

    return to_string(j);
}

nlohmann::json Block::jsonView() {
    nlohmann::json j;

    j["transactions"] = transactionList(transactions);
    j["lastHash"] = lastHash;
    j["hash"] = hash;
    j["forgerAddress"] = forgerAddress;
    j["timestamp"] = timestamp;
    j["blockCount"] = blockCount;
    j["signature"] = signature;
    j["_id"] = _id;
    
    return j;
}