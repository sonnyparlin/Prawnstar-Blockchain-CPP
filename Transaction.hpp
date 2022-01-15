#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_
#include "config.hpp"
#include <string>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

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
    string generateUUID();
    void sign(std::string sig);
    void display() const;
};

#endif // _TRANSACTION_H_