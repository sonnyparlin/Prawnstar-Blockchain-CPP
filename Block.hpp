#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <string>
#include <vector>
#include "TransactionPool.hpp"
#include <ctime>

using namespace std;

class Block {
public:
    vector <TransactionPool> transactions;
    string last_hash;
    string hash;
    string forger_address;
    time_t timestamp = time(nullptr);
    unsigned long long block_count;
    string signature;

    // Constructor
    Block(vector <TransactionPool> transactions,
          string last_hash, 
          string hash, string 
          forger_address,
          time_t timestamp, 
          unsigned long long block_count);
        
    // Destructor
    ~Block();
    
    void display() const;
};

#endif // _BLOCK_H_