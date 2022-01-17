#ifndef _WALLET_H_
#define _WALLET_H_

#include "utils.hpp"
#include "Transaction.hpp"
#include "Block.hpp"
#include <vector>
#include <string>

class Wallet {
    public:
        std::vector<std::string> key_pair;
        std::string private_key;
        std::string public_key;
        std::string address;

    Wallet();
    ~Wallet();

    void genKeyPair();
    std::string sign(std::string data);
    Transaction create_transaction(std::string receiver, double amount, std::string type);
    Block create_block(vector<Transaction> transactions, std::string last_hash, unsigned long long block_count);
    std::string generateAddress();

};

#endif // _WALLET_H_