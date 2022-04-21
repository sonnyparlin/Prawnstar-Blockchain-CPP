#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <string>
#include <vector>
#include "TransactionPool.hpp"
#include <ctime>
#include <nlohmann/json.hpp>


class Block {
public:
    std::vector <Transaction> transactions;
    std::string lastHash;
    std::string hash;
    std::string forgerAddress;
    time_t timestamp = timeSinceEpoch();
    long blockCount {};
    std::string signature;
    std::string _id;

    // Constructors
    Block();
    Block(std::vector <Transaction>&,
          std::string&,
          std::string&,
          long blockCount);

    Block(std::vector <Transaction>&,
          std::string&,
          long blockCount);
        
    // Destructor
    ~Block();

    std::string toJson();
    std::string payload();
    void sign(std::string&);
    static vector<nlohmann::json> transactionList(vector <Transaction> &transactions);
    nlohmann::json jsonView();
};

#endif // _BLOCK_H_