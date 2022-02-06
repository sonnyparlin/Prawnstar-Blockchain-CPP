#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_
#include <vector>
#include <string>
#include "Block.hpp"
#include "AccountModel.hpp"
#include "config.hpp"

class Blockchain {
public:
    std::vector<Block> blocks;
    AccountModel accountModel;

    Blockchain();           // Constructor
    ~Blockchain();          // Destructor
    
    bool addBlock(Block block);
    vector<nlohmann::json> blockList(vector <Block> blocks) const;
    nlohmann::json toJson() const;
    std::string toJsonString() const;
    Block genesis();
    bool blockCountValid(Block block);
    bool lastBlockHashValid(Block block);
    bool transactionCovered(Transaction transaction);
    std::vector<Transaction> getCoveredTransactionSet(std::vector<Transaction> transactions);
    void executeTransaction(Transaction transaction);
    void executeTransactions(std::vector<Transaction> transactions);
};

#endif // _BLOCKCHAIN_H_
