#include <iostream>
#include <string>
#include <ctime>
#include "config.hpp"
#include "Transaction.hpp"

Transaction::Transaction(string sender_address, 
                         string receiver_address, 
                         double amount, 
                         string type)
    : sender_address(sender_address), 
      receiver_address(receiver_address), 
      amount(amount), 
      type(type)  {
    id = generateUUID();
    timestamp = time(nullptr);
}

Transaction::~Transaction() {}

string Transaction::generateUUID() {
    boost::uuids::random_generator generator;
    boost::uuids::uuid uuid1 = generator();
    return to_string(uuid1);
}

void Transaction::display() const {
    char* dt = ctime(&timestamp);

    cout << endl;
    cout << "from: " << sender_address << " to: " << receiver_address << endl;
    cout << "amount: " << amount << endl;
    cout << "type: " << type << endl;
    cout << "id: " << id << endl;
    cout << "timestamp: " << dt << endl;
    cout << "signature: " << signature << endl;
    cout << endl;
}

string Transaction::to_json() {
    time_t seconds = time_since_epoch();

    string j = "[{\"sender_address\":\"" + sender_address + "\"," +
               "\"receiver_address\":\"" + receiver_address + "\"," + 
               "\"amount\":" + to_string(amount) + "," +
               "\"type\":\"" + type + "\"," +
               "\"id\":\"" + id + "\"," +
               "\"timestamp\":\"" + to_string(seconds) + "\"," +
               "\"signature\":\"" + signature + "\"}]";
    return j;
}