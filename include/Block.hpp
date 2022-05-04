#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <string>
#include <vector>
#include "TransactionPool.hpp"
#include <ctime>
#include <nlohmann/json.hpp>
#include "utils.hpp"

class Block {
public:
    // Constructors
    Block();
    Block(std::vector <Transaction>,
          std::string,
          std::string,
          long blockCount);
    Block(std::vector <Transaction>,
          std::string,
          long blockCount);
    ~Block();

    std::string toJson();
    std::string payload();
    void sign(std::string);
    static vector<nlohmann::json> transactionList(vector <Transaction> &transactions);
    nlohmann::json jsonView();

    std::vector <Transaction> transactions;
    std::string lastHash;
    std::string hash;
    std::string forgerAddress;
    time_t timestamp = utils::timeSinceEpoch();
    long blockCount {0};
    std::string signature;
    std::string _id;
    size_t signatureLength {0};
};

#endif // _BLOCK_H_