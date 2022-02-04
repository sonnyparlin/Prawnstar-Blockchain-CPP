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
    vector<Transaction> zeroTransactions;

    Block genesisBlock(zeroTransactions, "***no***last***hash***", 0);
    genesisBlock.timestamp = 0;
    genesisBlock.hash = "***genesis***hash***";
    
    return genesisBlock;
}

bool Blockchain::addBlock(Block block) {
    executeTransactions(block.transactions);
    blocks.push_back(block);
    blocks.shrink_to_fit();
    return true;
}

bool Blockchain::blockCountValid(Block block) {
    int n = blocks.size();
    if (blocks[n-1].blockCount == block.blockCount - 1)
        return true;
    return false;
}

bool Blockchain::lastBlockHashValid(Block block) {
    int n = blocks.size();
    if (blocks[n-1].hash == block.lastHash)
        return true;
    return false;
}

bool Blockchain::transactionCovered(Transaction transaction) {
    if (transaction.type == tx.exchange)
        return true;
    double senderBalance = accountModel.getBalance(transaction.senderAddress);
    return senderBalance >= transaction.amount;
}

std::vector<Transaction> Blockchain::getCoveredTransactionSet(vector<Transaction> transactions) {
    std::vector<Transaction> coveredTransactions;
    for (auto transaction : transactions) {
        if (transactionCovered(transaction))
            coveredTransactions.push_back(transaction);
        else
            std::cout << "Transaction is not covered by the sender" << std::endl;
    }
    return coveredTransactions;
}

void Blockchain::executeTransactions(std::vector<Transaction> transactions) {
    for (auto transaction : transactions) {
        executeTransaction(transaction);
    }
}

void Blockchain::executeTransaction(Transaction transaction) {
    std::string senderAddress = transaction.senderAddress;
    std::string receiverAddress = transaction.receiverAddress;
    double amount = transaction.amount;

    accountModel.updateBalance(senderAddress, -amount);
    accountModel.updateBalance(receiverAddress, amount);
}

vector<nlohmann::json> Blockchain::blockList(vector <Block> blocks) const {
    nlohmann::json j;
    vector<nlohmann::json> blks;

    for (auto block: blocks) {
        blks.push_back(block.jsonView());
    }
    return blks;
}

nlohmann::json Blockchain::toJson() const {
    nlohmann::json j;

    j["blocks"] = blockList(blocks);
    return j;
}