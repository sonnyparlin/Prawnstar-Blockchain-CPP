#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_
#include <nlohmann/json.hpp>
#include <string>

using namespace std;

/*!
 * Our transaction class.
 */
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

    string senderAddress; /*!< sending address */
    string senderPublicKey; /*!< sender's public key */
    string receiverAddress; /*!< receiver's address */
    string receiverPublicKey; /*!< receiver's public key */
    double amount{}; /*!< transaction amount */
    string type; /*!< type (EXCHANGE, TRANSFER, STAKE, REWARD) */
    string id; /*!< Transaction id */
    time_t timestamp{}; /*!< Transaction timestamp */
    string signature; /*!< Transaction's signature. */
    size_t signatureLength{}; /*!< Signature length */
};

#endif // _TRANSACTION_H_