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
    Blockchain();
    explicit Blockchain(Node *node);           // Constructor
    ~Blockchain();          // Destructor
    
    void addBlock(const Block &block);
    static Block genesis();
    bool blockCountValid(const Block&);
    bool lastBlockHashValid(const Block&);
    bool transactionCovered(const Transaction&);
    void executeTransaction(const Transaction&);
    void executeTransactions(const std::vector<Transaction>&);
    Block createBlock(const std::vector<Transaction>&, const std::string&);
    bool transactionExists(const Transaction&);
    bool forgerValid(const Block&);
    static std::vector<nlohmann::json> blockList(std::vector<Block>&);
    std::string nextForger();
    bool blockHasTransactions(const Block&);
    std::vector<Transaction> calculateForgerReward(std::vector<Transaction>&);
    std::vector<nlohmann::json> txsByAddress(const std::string&);
    std::string getTransaction(const std::string&);
    nlohmann::json toPureJson();
    std::string toJson();
    static std::string toJson(std::vector<Block> blocks);

    AccountModel accountModel; /*!< accountModel data member */
    TransactionPool transactionPool; /*!< transactionPool data member */
    std::vector<Block> blocks {}; /*!< list of blocks */
    std::mutex blockchainMutex {}; /*!< mutex for accessing shared blockchain data */
};

#endif // _BLOCKCHAIN_H_
