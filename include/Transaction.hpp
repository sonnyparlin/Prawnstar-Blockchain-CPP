#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_
#include <nlohmann/json.hpp>
#include <string>

using namespace std;

class Transaction {
public:
    string toJson();
    string payload();
    void sign(std::string sig);
    friend bool operator==(const Transaction &lhs, const Transaction &rhs);

    // ctor
    Transaction(string senderAddress, 
                string receiverAddress, 
                double amount, 
                string type);
     
    Transaction(); // ctor
    ~Transaction(); // dtor

    string senderAddress;
    string senderPublicKey;
    string receiverAddress;
    string receiverPublicKey;
    double amount{};
    string type;
    string id;
    time_t timestamp{};
    string signature;
    size_t signatureLength{};
};

#endif // _TRANSACTION_H_