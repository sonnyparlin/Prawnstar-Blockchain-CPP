#include <iostream>
#include <string>
#include <ctime>
#include "config.hpp"
#include "Transaction.hpp"
#include <nlohmann/json.hpp>
#include "utils.hpp"

Transaction::Transaction(string sender_address, 
                         string receiver_address, 
                         double amount, 
                         string type)
    : sender_address(sender_address), 
      receiver_address(receiver_address), 
      amount(amount), 
      type(type)  {
    id = generateUUID();
    timestamp = time_since_epoch();
;
}

Transaction::~Transaction() {}

string Transaction::generateUUID() {
    std::string uuid1 = utils::generate_uuid_hex();
    return uuid1;
}

string Transaction::to_json() {
    nlohmann::json j;
    
    j["sender_address"] = sender_address;
    j["receiver_address"] = receiver_address;
    j["amount"] = amount;
    j["type"] = type;
    j["id"] = id;
    j["timestamp"] = timestamp;
    j["signature"] = signature;
    
    return to_string(j);
}

string Transaction::payload() {
    nlohmann::json j;
    
    j["sender_address"] = sender_address;
    j["receiver_address"] = receiver_address;
    j["amount"] = amount;
    j["type"] = type;
    j["id"] = id;
    j["timestamp"] = timestamp;
    j["signature"] = "";
    
    return to_string(j);
}

bool Transaction::equals(Transaction transaction) {
    return this->id == transaction.id;
}

void Transaction::sign(std::string sig) {
    signature = sig;
}