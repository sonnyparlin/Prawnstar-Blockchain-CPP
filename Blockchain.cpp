#include <iostream>
#include "Blockchain.hpp"
#include "utils.hpp"

Blockchain::Blockchain(Node *node) {
    this->node = node;
    blocks.push_back(genesis());
}

Blockchain::~Blockchain()=default;

Block Blockchain::genesis() {
    vector<Transaction> zeroTransactions;
    std::string lastHash = "***no***last***hash***";
    Block genesisBlock(zeroTransactions, lastHash, 0);
    genesisBlock.timestamp = 0;
    genesisBlock.hash = "***genesis***hash***";
    genesisBlock._id = genesisBlock.hash;
    
    return genesisBlock;
}

bool Blockchain::addBlock(const Block &block) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    executeTransactions(block.transactions);
    blocks.push_back(block);
    blocks.shrink_to_fit();
    return true;
}

std::vector<Transaction> Blockchain::calculateForgerReward(std::vector<Transaction> &transactions) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    std::vector<Transaction> resultTransactions;
    double reward;

    for (auto &itx : transactions) {

        if (itx.type == "STAKE")
            return transactions;

        reward = (itx.amount * 0.005);
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
        auto str = oss.str();
        node->log(str);
        std::string r = "reward: " + std::to_string(reward);
        node->log(r);

        try {
            Transaction rewardTx = node->exchangeWallet->createTransaction(node->nodeWallet->address, reward, "REWARD");
            resultTransactions.push_back(rewardTx);
        } catch(std::exception& e) {
            std::cerr << "Error with createTransaction: " << e.what() << std::endl;
        }
        // node->accountModel->updateBalance(node->nodeWallet->address, reward);
        itx.amount -= reward;

        resultTransactions.push_back(itx);
    }
    return resultTransactions;
}

bool Blockchain::blockCountValid(const Block &block) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    return blocks[blocks.size()-1].blockCount == block.blockCount - 1;
}

bool Blockchain::lastBlockHashValid(const Block &block) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    return blocks[blocks.size()-1].hash == block.lastHash;
}

bool Blockchain::transactionCovered(const Transaction &transaction) {
    if (transaction.type == tx.exchange || transaction.type == tx.reward)
        return true;
    double senderBalance = node->accountModel->getBalance(transaction.senderAddress);
    return senderBalance >= transaction.amount;
}

std::vector<Transaction> Blockchain::getCoveredTransactionSet(const vector<Transaction> &transactions) {
    std::vector<Transaction> coveredTransactions;
    for (auto const &transaction : transactions) {
        if (transactionCovered(transaction))
            coveredTransactions.push_back(transaction);
        else {
            std::cout << "Transaction is not covered by the sender" << std::endl;
            std::vector<Transaction> transactionsToRemove {transaction};
            node->transactionPool.removeFromPool(transactionsToRemove);
        }
    }
    return coveredTransactions;
}

bool Blockchain::blockHasTransactions(const Block &block) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    auto response = block.transactions.size();
    return response > 0;
}

void Blockchain::executeTransactions(const std::vector<Transaction> &transactions) {
    for (auto const &transaction : transactions) {
        executeTransaction(transaction);
    }
}

void Blockchain::executeTransaction(const Transaction &transaction) {

    if (transaction.type == "STAKE") {
        std::string sender = transaction.senderAddress;
        std::string publicKeyString = transaction.senderPublicKey;
        std::string receiver = transaction.receiverAddress;

        if (sender == receiver) {
            double amount = transaction.amount;
            node->proofOfStake->update(publicKeyString, amount);
            node->accountModel->updateBalance(sender, -amount);
        }
    } else {
        std::string senderAddress = transaction.senderAddress;
        std::string receiverAddress = transaction.receiverAddress;
        double amount = transaction.amount;

        node->accountModel->updateBalance(senderAddress, -amount);
        node->accountModel->updateBalance(receiverAddress, amount);
    }
}

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
                                              blocks.size());
    blocks.push_back(newBlock);

    return newBlock;
}

bool Blockchain::transactionExists(const Transaction &transaction) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    for(auto const &block : blocks) {
        for(auto const &itx : block.transactions) {
            if (itx == transaction)
                return true;
        }
    }
    return false;
}

std::string Blockchain::getTransaction(const std::string &txid) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    for(auto &block : blocks) {
        for(auto &itx : block.transactions) {
            if (itx.id == txid)
                return itx.toJson();
        }
    }
    return "";
}

std::vector<nlohmann::json> Blockchain::txsByAddress(const std::string &address) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    std::vector<nlohmann::json> txids;
    std::vector<Block> localChain = blocks;
    
    for(auto &block : localChain) {
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

bool Blockchain::transactionValid(const std::vector<Transaction> &transactions) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    std::vector<Transaction> coveredTransactions = this->node->blockchain->getCoveredTransactionSet(transactions);
    return coveredTransactions.size() == transactions.size();
}

vector<nlohmann::json> Blockchain::blockList(vector<Block> &blocks) {
    nlohmann::json j;
    vector<nlohmann::json> blks;

    for (auto &block: blocks) {
        blks.push_back(block.jsonView());
    }
    return blks;
}

std::string Blockchain::nextForger() {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    auto blocksSize = blocks.size();
    std::string nextForger;
    std::string lastBlockHash;
    lastBlockHash = blocks[blocksSize-1].lastHash;
    nextForger = node->proofOfStake->forger(lastBlockHash);
    return nextForger;
}

nlohmann::json Blockchain::toJson() {
    nlohmann::json j;

    j["blocks"] = blockList(blocks);
    return j;
}

std::string Blockchain::toJsonString(std::vector<Block> blocks) {
    nlohmann::json j;

    j["blocks"] = blockList(blocks);
    return j.dump();
}

std::string Blockchain::toJsonWebView() {
    nlohmann::json j;

    j["blocks"] = blockList(blocks);
    return j.dump();
}