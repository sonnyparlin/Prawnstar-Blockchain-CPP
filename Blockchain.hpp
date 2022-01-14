#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_
#include <vector>
#include <string>
#include "Block.hpp"

class Blockchain {
private:
    std::vector<Block> blockchain;
public:
    Blockchain();             // Constructor
    ~Blockchain();          // Destructor
    
    bool add_block();
    void display() const;
};

#endif // _BLOCKCHAIN_H_
