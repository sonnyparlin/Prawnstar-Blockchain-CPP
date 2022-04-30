#ifndef _WALLET_H_
#define _WALLET_H_

#include "utils.hpp"
#include "Transaction.hpp"
#include "Block.hpp"
#include <vector>
#include <string>
#include "Node.hpp"

class Node;

class Wallet {
public:
    std::vector<std::string> walletKeyPair;
    std::string walletPrivateKey;
    std::string walletPublicKey;
    std::string address;

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
};

#endif // _WALLET_H_