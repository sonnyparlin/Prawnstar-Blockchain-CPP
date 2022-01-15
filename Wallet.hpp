#ifndef _WALLET_H_
#define _WALLET_H_

#include "utils.hpp"
#include "Transaction.hpp"
#include <vector>
#include <string>

class Wallet {
    public:
        std::vector<std::string> key_pair;
        std::string private_key;
        std::string public_key;

    Wallet();
    ~Wallet();

    void genKeyPair();
    std::string sign(std::string data);
    Transaction create_transaction(std::string receiver, double amount, std::string type);

};

#endif // _WALLET_H_