#ifndef _WALLET_H_
#define _WALLET_H_

#include "utils.hpp"
#include "Transaction.hpp"
#include "Block.hpp"
#include <vector>
#include <string>
#include "Node.hpp"

class Node;

/*!
 * Wallet class.
 */
class Wallet {

public:
    Wallet(Node *node, const char *filename);
    explicit Wallet(Node *node);
    Wallet(const char *address, Node *node);
    ~Wallet();

    void genKeyPair();
    utils::Signature sign(const std::string &) const;
    Transaction createTransaction(std::string receiverAddress, double amount, std::string type) const;
    Block createBlock(vector<Transaction> transactions, std::string lastHash, long blockCount) const;
    static std::string generateAddress(const std::string &str);
    std::string toJson();
    void fromKey(const char *file);

    std::vector<std::string> walletKeyPair {}; /*!< vector of keypair */
    std::string walletPrivateKey {}; /*!< private key */
    std::string walletPublicKey {}; /*!< public key */
    std::string address {}; /*!< address */
};

#endif // _WALLET_H_