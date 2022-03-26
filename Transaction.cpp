#include <iostream>
#include <string>
#include <ctime>
#include "config.hpp"
#include "Transaction.hpp"
#include <nlohmann/json.hpp>
#include "utils.hpp"

Transaction::Transaction(string senderAddress, 
                         string receiverAddress, 
                         double amount, 
                         string type)
    : senderAddress(senderAddress), 
      receiverAddress(receiverAddress), 
      amount(amount), 
      type(type)  {
    id = generateUUID();
    timestamp = timeSinceEpoch();
}

Transaction::Transaction() {}

Transaction::~Transaction() {}

bool operator==(const Transaction &lhs, const Transaction &rhs){ 
    /* do actual comparison */ 
    return lhs.id == rhs.id;
}

string Transaction::generateUUID() {
    std::string uuid1 = utils::uuid_gen();
    return uuid1;
}

string Transaction::toJson() {
    nlohmann::json j;
    
    j["senderAddress"] = senderAddress;
    j["senderPublicKey"] = senderPublicKey;
    j["receiverAddress"] = receiverAddress;
    j["amount"] = amount;
    j["type"] = type;
    j["id"] = id;
    j["timestamp"] = timestamp;
    j["signature"] = signature;
    
    return j.dump();
}

string Transaction::payload() {
    nlohmann::json j;
    
    j["senderAddress"] = senderAddress;
    j["receiverAddress"] = receiverAddress;
    j["amount"] = amount;
    j["type"] = type;
    j["id"] = id;
    j["timestamp"] = timestamp;
    j["signature"] = "";
    
    return to_string(j);
}

void Transaction::sign(std::string sig) {
    signature = sig;
}