#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_
#include <vector>
#include <string>
#include "Block.hpp"
#include "AccountModel.hpp"
#include "config.hpp"
#include "Node.hpp"
#include <mutex>

class Node;

class Blockchain {
private:
    Node *node;

public:
    std::vector<Block> blocks;
    std::mutex blockchainMutex;

    Blockchain(Node *node);           // Constructor
    ~Blockchain();          // Destructor
    
    bool addBlock(Block block);
    vector<nlohmann::json> blockList(vector <Block> blocks) const;
    nlohmann::json toJson() const;
    std::string toJsonString(std::vector<Block> blocks) const;
    Block genesis();
    bool blockCountValid(Block block);
    bool lastBlockHashValid(Block block);
    bool transactionCovered(Transaction transaction);
    std::vector<Transaction> getCoveredTransactionSet(std::vector<Transaction> transactions);
    void executeTransaction(Transaction transaction);
    void executeTransactions(std::vector<Transaction> transactions);
    std::string nextForger();
    Block createBlock(std::vector<Transaction> transactionsFromPool, std::string forgerAddress);
    bool transactionExists(Transaction transaction);
    bool forgerValid(Block block);
    bool transactionValid(std::vector<Transaction> transactions);
    bool blockHasTransactions(Block block);
    std::string toJsonWebView() const;
    std::vector<Transaction> calculateForgerReward(std::vector<Transaction>&);
};

#endif // _BLOCKCHAIN_H_
