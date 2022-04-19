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
    string lastHash;
    string hash;
    string forgerAddress;
    time_t timestamp = timeSinceEpoch();
    unsigned long long blockCount {};
    string signature;
    string _id;

    // Constructors
    Block();
    Block(std::vector <Transaction> &transactions,
          std::string &lastHash,
          std::string &forgerAddress,
          unsigned long long blockCount);

    Block(std::vector <Transaction> &transactions,
          std::string &lastHash,
          unsigned long long blockCount);
        
    // Destructor
    ~Block();
    
    string toJson();
    string payload();
    void sign(std::string&);
    static vector<nlohmann::json> transactionList(vector <Transaction> &transactions);
    nlohmann::json jsonView();
};

#endif // _BLOCK_H_