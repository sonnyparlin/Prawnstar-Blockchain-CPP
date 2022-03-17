#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_
#include "config.hpp"
#include <nlohmann/json.hpp>
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
     
    Transaction();
    ~Transaction();          // Destructor

    string toJson();
    string payload();
    string generateUUID();
    void sign(std::string sig);
    bool equals(Transaction transaction);

    friend bool operator==(const Transaction &lhs, const Transaction &rhs); 
};

#endif // _TRANSACTION_H_