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
    unsigned long long blockCount;
    string signature;

    // Constructors
    Block();
    Block(vector <Transaction> transactions,
          string lastHash, 
          string forgerAddress,
          unsigned long long blockCount);

    Block(vector <Transaction> transactions,
          string lastHash,
          unsigned long long blockCount);
        
    // Destructor
    ~Block();
    
    void display() const;
    string toJson() const;
    string payload();
    void sign(string data);
    vector<nlohmann::json> transactionList(vector <Transaction> transactions) const;
    nlohmann::json jsonView() const;
};

#endif // _BLOCK_H_