#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <string>
#include <vector>
#include "TransactionPool.hpp"
#include <ctime>
#include <nlohmann/json.hpp>

using namespace std;

class Block {
public:
    vector <Transaction> transactions;
    std::string lastHash;
    std::string hash;
    std::string forgerAddress;
    time_t timestamp = timeSinceEpoch();
    unsigned long long blockCount {0};
    std::string signature;
    std::string _id;

    // Constructors
    Block();
    Block(vector <Transaction> &transactions,
          std::string &lastHash,
          std::string &forgerAddress,
          unsigned long long blockCount);

    Block(vector <Transaction> &transactions,
          std::string &lastHash,
          unsigned long long blockCount);
        
    // Destructor
    ~Block();

    std::string toJson();
    std::string payload();
    void sign(std::string &data);
    static vector<nlohmann::json> transactionList(std::vector <Transaction> &transactions);
    nlohmann::json jsonView();
};

#endif // _BLOCK_H_