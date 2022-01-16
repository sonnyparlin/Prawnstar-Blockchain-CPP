#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_
#include <vector>
#include <string>
#include "Block.hpp"

class Blockchain {
public:
    std::vector<Block> blocks;
    Blockchain();           // Constructor
    ~Blockchain();          // Destructor
    
    bool add_block(Block block);
    vector<nlohmann::json> blockList(vector <Block> blocks) const;
    nlohmann::json to_json() const;
    Block genesis();
    bool block_count_valid(Block block);
    bool last_block_hash_valid(Block block);
};

#endif // _BLOCKCHAIN_H_
