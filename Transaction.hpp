#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_
#include "config.hpp"
#include <string>

using namespace std;

class Transaction {
public:
    string sender_address;
    string sender_public_key;
    string receiver_address;
    string receiver_public_key;
    long double amount;
    string type;
    string id;
    time_t timestamp;
    string signature;

    // Constructor
    Transaction(string sender_address, 
                string receiver_address, 
                double amount, 
                string type);

    ~Transaction();          // Destructor

    string to_json();
    string payload();
    string generateUUID();
    void sign(std::string sig);
    bool equals(Transaction transaction);
};

#endif // _TRANSACTION_H_