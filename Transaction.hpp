#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_
#include "config.hpp"
#include <string>

using namespace std;

class Transaction {
public:
    string senderAddress;
    string senderPublicKey;
    string receiverAddress;
    string receiverPublicKey;
    long double amount;
    string type;
    string id;
    time_t timestamp;
    string signature;

    // Constructor
    Transaction(string senderAddress, 
                string receiverAddress, 
                double amount, 
                string type);

    ~Transaction();          // Destructor

    string toJson();
    string payload();
    string generateUUID();
    void sign(std::string sig);
    bool equals(Transaction transaction);
};

#endif // _TRANSACTION_H_