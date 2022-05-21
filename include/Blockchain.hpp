#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_
#include <vector>
#include <string>
#include "Block.hpp"
#include "AccountModel.hpp"
#include "Node.hpp"
#include <mutex>

class Node;

/*!
 * Blockchain class.
 */
class Blockchain {
private:
    Node *node = nullptr;

public:
    explicit Blockchain(Node *node);           // Constructor
    ~Blockchain();          // Destructor
    
    bool addBlock(const Block &block);
    static Block genesis();
    bool blockCountValid(const Block&);
    bool lastBlockHashValid(const Block&);
    bool transactionCovered(const Transaction&);
    std::vector<Transaction> getCoveredTransactionSet(const std::vector<Transaction>&);
    void executeTransaction(const Transaction&);
    void executeTransactions(const std::vector<Transaction>&);
    Block createBlock(const std::vector<Transaction>&, const std::string&);
    bool transactionExists(const Transaction&);
    bool forgerValid(const Block&);
    bool transactionValid(const std::vector<Transaction>&);
    static std::vector<nlohmann::json> blockList(std::vector<Block>&);
    std::string nextForger();
    bool blockHasTransactions(const Block&);
    std::string toJsonWebView();
    std::vector<Transaction> calculateForgerReward(std::vector<Transaction>&);
    std::vector<nlohmann::json> txsByAddress(const std::string&);
    std::string getTransaction(const std::string&);
    nlohmann::json toJson();
    static std::string toJsonString(std::vector<Block>);

    std::vector<Block> blocks {}; /*!< list of blocks */
    std::mutex blockchainMutex {}; /*!< mutex for accessing shared blockchain data */
};

#endif // _BLOCKCHAIN_H_
