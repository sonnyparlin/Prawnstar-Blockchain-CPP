#ifndef _WALLET_H_
#define _WALLET_H_

#include "utils.hpp"
#include "Transaction.hpp"
#include "Block.hpp"
#include <vector>
#include <string>
#include "AccountModel.hpp"

class Wallet {
    public:
        std::vector<std::string> walletKeyPair;
        std::string walletPrivateKey;
        std::string walletPublicKey;
        std::string address;

    Wallet();
    Wallet(std::string address, AccountModel accountModel);
    Wallet(const bool useFile);
    ~Wallet();

    void genKeyPair(const bool useFile);
    std::string sign(std::string data);
    Transaction createTransaction(std::string receiverAddress, double amount, std::string type);
    Block createBlock(vector<Transaction> transactions, std::string lastHash, unsigned long long blockCount);
    std::string generateAddress();
    void loadWalletFromAddress(const std::string& address, AccountModel );

};

#endif // _WALLET_H_