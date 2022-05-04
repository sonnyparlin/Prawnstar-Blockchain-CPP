#include <string>
#include "Transaction.hpp"
#include <nlohmann/json.hpp>
#include <utility>
#include "utils.hpp"

Transaction::Transaction(string senderAddress, 
                         string receiverAddress, 
                         double amount, 
                         string type)
    : senderAddress(std::move(senderAddress)),
      receiverAddress(std::move(receiverAddress)),
      amount(amount), 
      type(std::move(type))  {
    id = utils::get_uuid();
    timestamp = utils::timeSinceEpoch();
}

Transaction::Transaction()=default;
Transaction::~Transaction()=default;

bool operator==(const Transaction &lhs, const Transaction &rhs) {
    return lhs.id == rhs.id;
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
    signature = std::move(sig);
}