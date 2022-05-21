#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_
#include <nlohmann/json.hpp>
#include <string>

/*!
 * Our transaction class.
 */
class Transaction {
public:
    // class methods
    std::string toJson();
    std::string payload();
    void sign(std::string sig);

    // operator overload
    friend bool operator==(const Transaction &lhs, const Transaction &rhs);

    // constructors
    Transaction(std::string senderAddress,
                std::string receiverAddress,
                double amount,
                std::string type);
    Transaction();

    // destructor
    ~Transaction();

    // class data members
    std::string senderAddress {}; /*!< sending address */
    std::string senderPublicKey {}; /*!< sender's public key */
    std::string receiverAddress {}; /*!< receiver's address */
    std::string receiverPublicKey {}; /*!< receiver's public key */
    double amount{}; /*!< transaction amount */
    std::string type {}; /*!< type (EXCHANGE, TRANSFER, STAKE, REWARD) */
    std::string id {}; /*!< Transaction id */
    time_t timestamp{}; /*!< Transaction timestamp */
    std::string signature {}; /*!< Transaction's signature. */
    size_t signatureLength {}; /*!< Signature length */
};

#endif // _TRANSACTION_H_