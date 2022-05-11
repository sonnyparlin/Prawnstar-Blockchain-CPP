#include <iostream>
#include <algorithm>
#include "Blockchain.hpp"
#include "utils.hpp"

/*!
 *
 * @param node
 *
 * Create the blockchain object.
 */
Blockchain::Blockchain(Node *node) {
    this->node = node;
    blocks.push_back(genesis());
}

/*!
 * Blockchain destructor
 */
Blockchain::~Blockchain()=default;

/*!
 *
 * @return Block
 *
 * Create the genesis block for the blockchain.
 */
Block Blockchain::genesis() {
    vector<Transaction> zeroTransactions;
    std::string lastHash = "***no***last***hash***";
    Block genesisBlock(zeroTransactions, lastHash, 0);
    genesisBlock.timestamp = 0;
    genesisBlock.hash = "***genesis***hash***";
    genesisBlock._id = genesisBlock.hash;
    
    return genesisBlock;
}

/*!
 *
 * @param block
 * @return bool
 *
 * Add a new block to the blockchain.
 */
bool Blockchain::addBlock(const Block &block) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    executeTransactions(block.transactions);
    blocks.push_back(block);
    // blocks.shrink_to_fit();
    return true;
}

/*!
 *
 * @param transactions
 * @return std::vector<Transaction>
 *
 * Calculate the forger reward and set new balances accordingly.
 * Forgers get paid by the senders.
 */
std::vector<Transaction> Blockchain::calculateForgerReward(
        std::vector<Transaction> &transactions
) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    std::vector<Transaction> resultTransactions;
    double reward {};

    std::for_each(transactions.begin(), transactions.end(), [this, &resultTransactions, &reward](Transaction &itx){
        if (itx.type == "STAKE") {
            resultTransactions.push_back(itx);

            /*!
             * No forger reward for STAKE or REWARD transactions.
             * We don't need to add REWARD transactions to the
             * if statement above because REWARD transactions are
             * only created after the above check is performed.
             */
        }

        if (itx.type != "STAKE") {
            reward += (itx.amount * 0.005);
            itx.amount -= (itx.amount * 0.005);
            resultTransactions.push_back(itx);

            auto str = Node::getTimeStr();
            node->log(str);
            std::string r = "reward: " + std::to_string(reward);
            node->log(r);
        }
    });

    try {
        Transaction rewardTx = node->exchangeWallet->
                createTransaction(node->nodeWallet->address, reward, "REWARD");
        resultTransactions.push_back(rewardTx);
    } catch(std::exception& e) {
        std::cerr << "Error with createTransaction: " << e.what() << std::endl;
    }

    return resultTransactions;
}

/*!
 *
 * @param block
 * @return bool
 *
 * Ensure that the block count is valid by comparing the blockCount of the new block
 * minus one is equal to the last block of the blockchain.
 */
bool Blockchain::blockCountValid(const Block &block) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    return blocks[blocks.size()-1].blockCount == block.blockCount - 1;
}

/*!
 *
 * @param block
 * @return bool
 *
 * Make sure the lastHash of the block is equal to the hash of the blockchain's last
 * block.
 */
bool Blockchain::lastBlockHashValid(const Block &block) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    return blocks[blocks.size()-1].hash == block.lastHash;
}

/*!
 *
 * @param transaction
 * @return bool
 *
 * Make sure the sender has enough funds to continue the transaction. Note that
 * we forgo this check if the transaction type is a REWARD or an EXCHANGE
 * transaction.
 */
bool Blockchain::transactionCovered(const Transaction &transaction) {
    if (transaction.type == "EXCHANGE" || transaction.type == "REWARD")
        return true;
    double senderBalance = node->accountModel->getBalance(transaction.senderAddress);
    return senderBalance >= transaction.amount;
}

/*!
 *
 *
 * @return std::vector<Transaction>
 *
 * Get covered transactions from list of transactions.
 */
std::vector<Transaction> Blockchain::getCoveredTransactionSet(
        const vector<Transaction> &transactions
) {
    std::vector<Transaction> coveredTransactions;
    std::for_each(transactions.begin(), transactions.end(), [this, &coveredTransactions]
                                                                        (const Transaction& transaction) {
        if (transactionCovered(transaction))
            coveredTransactions.push_back(transaction);
        else {
            std::cout << "Transaction is not covered by the sender" << std::endl;
            std::vector<Transaction> transactionsToRemove {transaction};
            node->transactionPool.removeFromPool(transactionsToRemove);
        }
    });
    return coveredTransactions;
}

/*!
 *
 * @param block
 * @return bool
 *
 * Ensure the new block has transactions before execution.
 */
bool Blockchain::blockHasTransactions(const Block &block) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    return !block.transactions.empty();
}

/*!
 *
 * @param transactions
 *
 * Wrapper for executing each transaction.
 */
void Blockchain::executeTransactions(const std::vector<Transaction>& transactions) {
    std::for_each(transactions.begin(), transactions.end(), [this](const Transaction& transaction) {
        executeTransaction(transaction);
    });
}

/*!
 *
 * @param transaction
 *
 * Execute the given transaction.
 */
void Blockchain::executeTransaction(const Transaction& transaction) {

    if (transaction.type == "STAKE") {
        if (transaction.senderAddress == transaction.receiverAddress) {
            node->proofOfStake->
                    update(transaction.senderPublicKey, transaction.amount);
            node->accountModel->
                    updateBalance(transaction.senderAddress, -transaction.amount);
        }
    }

    if (transaction.type != "STAKE") {
        node->accountModel->
                    updateBalance(transaction.senderAddress, -transaction.amount);
        node->accountModel->
                    updateBalance(transaction.receiverAddress, transaction.amount);
    }
}

/*!
 *
 * @param transactionsFromPool
 * @param forgerAddress
 * @return Block
 *
 * Create a new block with the given transactions and forger.
 */
Block Blockchain::createBlock(const std::vector<Transaction> &transactionsFromPool,
                              const std::string &forgerAddress) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    std::vector<Transaction> coveredTransactions = getCoveredTransactionSet(
        transactionsFromPool
    );

    std::string lastHash = blocks[blocks.size()-1].hash;
    Wallet forgerWallet(forgerAddress.c_str(), node);
    executeTransactions(coveredTransactions);
    Block newBlock = forgerWallet.createBlock(coveredTransactions, 
                                              lastHash,
                                              static_cast<long>(blocks.size()));
    blocks.push_back(newBlock);

    return newBlock;
}

/*!
 *
 * @param transaction
 * @return bool
 *
 * Check to see if the given transaction is already on the blockchain.
 */
bool Blockchain::transactionExists(const Transaction &transaction) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    return std::any_of(blocks.begin(), blocks.end(), [&transaction](const Block &block){
        if ( std::find(block.transactions.begin(), block.transactions.end(),
                                                transaction) != block.transactions.end() )
            return true;
        return false;
    });
}

/*!
 *
 * @param txid
 * @return std::string
 *
 * Return a json string of the transaction with the given transaction id.
 */
std::string Blockchain::getTransaction(const std::string &txid) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    std::string jsonString {};
    auto result = std::any_of(blocks.begin(), blocks.end(),
                                                [&txid, &jsonString](const Block &block){
        for(Transaction transaction : block.transactions) {
            if (transaction.id == txid) {
                jsonString = transaction.toJson();
                return true;
            }
        }
        return false;
    });
    if (result) return jsonString;
    return "";
}

/*!
 *
 * @param address
 * @return std::vector<nlohmann::json>
 *
 * Return a json representation of all transactions associated with a specific
 * address.
 */
std::vector<nlohmann::json> Blockchain::txsByAddress(const std::string &address) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    std::vector<nlohmann::json> txids {};

    for(const auto &block : blocks) {
        for(auto &itx : block.transactions) {
            if (itx.senderAddress == address || itx.receiverAddress == address) {
                nlohmann::json j;
                j["id"] = itx.id;
                txids.push_back(j);
            }
        }
    }
    // std::cout << "txids: " << txids.size() << std::endl;
    return txids;
}

/*!
 *
 * @param block
 * @return bool
 *
 * Run the proof of work algorithm to double check our forger is valid.
 */
bool Blockchain::forgerValid(const Block &block) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    Wallet proposedForger(block.forgerAddress.c_str(), this->node);

    std::string lastBlockHash = blocks[blocks.size()-1].lastHash;
    std::string forgerPublicKey = node->proofOfStake->forger(lastBlockHash);

    if (forgerPublicKey != proposedForger.walletPublicKey) {
        std::cout << "proposedForger: " << proposedForger.walletPublicKey << std::endl;
        std::cout << "actualForger  : " << forgerPublicKey << std::endl;
    }
    return forgerPublicKey == proposedForger.walletPublicKey;
}

/*!
 *
 * @param transactions
 * @return bool
 *
 * Run getCoveredTransactionSet() on the list of transactions and compare the resulting vector size
 * with the original vector size for validity.
 */
bool Blockchain::transactionValid(const std::vector<Transaction> &transactions) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    std::vector<Transaction> coveredTransactions = this->node->blockchain->
                                                getCoveredTransactionSet(transactions);
    return coveredTransactions.size() == transactions.size();
}

/*!
 *
 * @param blocks
 * @return vector<nlohmann::json>
 *
 * Return a json representation for a list of given blocks.
 */
vector<nlohmann::json> Blockchain::blockList(vector<Block> &blocks) {
    nlohmann::json j;
    vector<nlohmann::json> blks;

    for (auto &block: blocks) {
        blks.push_back(block.jsonView());
    }
    return blks;
}

/*!
 *
 * @return std::string
 *
 * Run the proof of work algorithm to get the next forger.
 */
std::string Blockchain::nextForger() {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    auto blocksSize = blocks.size();
    std::string nextForger;
    std::string lastBlockHash;
    lastBlockHash = blocks[blocksSize-1].lastHash;
    nextForger = node->proofOfStake->forger(lastBlockHash);
    return nextForger;
}

/*!
 *
 * @return nlohmann::json
 *
 * Return json representation of the entire blockchain.
 */
nlohmann::json Blockchain::toJson() {
    nlohmann::json j;

    j["blocks"] = blockList(blocks);
    return j;
}

/*!
 *
 * @param blocks
 * @return std::string
 *
 * Given a list of blocks, return a stringified json representation.
 */
std::string Blockchain::toJsonString(std::vector<Block> blocks) {
    nlohmann::json j;

    j["blocks"] = blockList(blocks);
    return j.dump();
}

/*!
 *
 * @return std::string
 *
 * Return a stringified json representation of all blocks.
 */
std::string Blockchain::toJsonWebView() {
    nlohmann::json j;

    j["blocks"] = blockList(blocks);
    return j.dump();
}