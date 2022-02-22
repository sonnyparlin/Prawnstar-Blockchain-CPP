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
private:
    Node *node;
public:
    std::vector<std::string> walletKeyPair;
    std::string walletPrivateKey;
    std::string walletPublicKey;
    std::string address;

    Wallet(Node *node);
    //Wallet(std::string address, Node *node);
    Wallet(const char *address, Node *node);
    ~Wallet();

    void genKeyPair();
    utils::Signature sign(std::string data);
    Transaction createTransaction(std::string receiverAddress, double amount, std::string type);
    Block createBlock(vector<Transaction> transactions, std::string lastHash, unsigned long long blockCount);
    std::string generateAddress(const std::string str);
    void loadWalletFromAddress(const std::string& address, Node *node);
    std::string toJson();
};

#endif // _WALLET_H_