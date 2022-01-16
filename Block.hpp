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
    string last_hash;
    string hash;
    string forger_address;
    time_t timestamp = time_since_epoch();
    unsigned long long block_count;
    string signature;

    // Constructors
    Block(vector <Transaction> transactions,
          string last_hash, 
          string forger_address,
          unsigned long long block_count);
    
    Block(vector <Transaction> transactions,
          string last_hash, 
          unsigned long long block_count);
        
    // Destructor
    ~Block();
    
    void display() const;
    string to_json() const;
    string payload() const;
    void sign(string data);
    vector<nlohmann::json> transactionList(vector <Transaction> transactions) const;
    nlohmann::json jsonview() const;
};

#endif // _BLOCK_H_