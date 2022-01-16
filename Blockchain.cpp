#include <iostream>
#include "Blockchain.hpp"
#include "Block.hpp"
#include "utils.hpp"

Blockchain::Blockchain() {
    blocks.push_back(genesis());
}

Blockchain::~Blockchain() {
    //blockchain.clear();
}

Block Blockchain::genesis() {
    vector<Transaction> zero_transactions;

    Block genesis_block(zero_transactions, "***no***last***hash***", 0);
    genesis_block.timestamp = 0;
    genesis_block.hash = "***genesis***hash***";
    
    return genesis_block;
}

bool Blockchain::add_block(Block block) {
    blocks.push_back(block);
    return true;
}

bool Blockchain::block_count_valid(Block block) {
    int n = blocks.size();
    if (blocks[n-1].block_count == block.block_count - 1)
        return true;
    return false;
}

bool Blockchain::last_block_hash_valid(Block block) {
    int n = blocks.size();
    if (blocks[n-1].hash == block.last_hash)
        return true;
    return false;
}

vector<nlohmann::json> Blockchain::blockList(vector <Block> blocks) const {
    nlohmann::json j;
    vector<nlohmann::json> blks;

    for (auto block: blocks) {
        blks.push_back(block.jsonview());
    }
    return blks;
}

nlohmann::json Blockchain::to_json() const {
    nlohmann::json j;

    j["blocks"] = blockList(blocks);
    return j;
}