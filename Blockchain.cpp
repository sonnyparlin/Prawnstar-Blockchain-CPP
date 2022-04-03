#include <iostream>
#include "Blockchain.hpp"
#include "Block.hpp"
#include "utils.hpp"

Blockchain::Blockchain(Node *node) {
    this->node = node;
    blocks.push_back(genesis());
}

Blockchain::~Blockchain() {
}

Block Blockchain::genesis() {
    vector<Transaction> zeroTransactions;

    Block genesisBlock(zeroTransactions, "***no***last***hash***", 0);
    genesisBlock.timestamp = 0;
    genesisBlock.hash = "***genesis***hash***";
    genesisBlock._id = genesisBlock.hash;
    
    return genesisBlock;
}

bool Blockchain::addBlock(Block block) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    executeTransactions(block.transactions);
    blocks.push_back(block);
    blocks.shrink_to_fit();
    return true;
}

std::vector<Transaction> Blockchain::calculateForgerReward(std::vector<Transaction> &transactions) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    std::vector<Transaction> resultTransactions;
    for (auto tx : transactions) {
        double reward {0};

        if (tx.type == "EXCHANGE" || tx.type == "TRANSFER") {
            double reward = (tx.amount * 0.005);
            auto t = std::time(nullptr);
            auto tm = *std::localtime(&t);
            std::ostringstream oss;
            oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
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
            tx.amount -= reward;
        }

        if (tx.type == "EXCHANGE")
            node->accountModel->updateBalance(tx.receiverAddress, -reward); 
        else if (tx.type == "TRANSFER" )
            node->accountModel->updateBalance(tx.senderAddress, -reward);

        resultTransactions.push_back(tx);
    }
    return resultTransactions;
}

bool Blockchain::blockCountValid(Block block) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    return blocks[blocks.size()-1].blockCount == block.blockCount - 1;
}

bool Blockchain::lastBlockHashValid(Block block) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    return blocks[blocks.size()-1].hash == block.lastHash;
}

bool Blockchain::transactionCovered(Transaction transaction) {
    if (transaction.type == tx.exchange || transaction.type == tx.reward)
        return true;
    double senderBalance = node->accountModel->getBalance(transaction.senderAddress);
    return senderBalance >= transaction.amount;
}

std::vector<Transaction> Blockchain::getCoveredTransactionSet(vector<Transaction> transactions) {
    std::vector<Transaction> coveredTransactions;
    for (auto transaction : transactions) {
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

bool Blockchain::blockHasTransactions(Block block) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    return block.transactions.size() > 0 ? true : false;
}

void Blockchain::executeTransactions(std::vector<Transaction> transactions) {
    for (auto transaction : transactions) {
        executeTransaction(transaction);
    }
}

void Blockchain::executeTransaction(Transaction transaction) {

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

        // std::cout << "sender: " << senderAddress << " amount: " << amount << std::endl;

        node->accountModel->updateBalance(senderAddress, -amount);
        node->accountModel->updateBalance(receiverAddress, amount);
    }
}

Block Blockchain::createBlock(std::vector<Transaction> transactionsFromPool, std::string forgerAddress) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    std::vector<Transaction> coveredTransactions = getCoveredTransactionSet(
        transactionsFromPool
    );

    // std::string transactionListAsString;
    // for (auto tx : coveredTransactions) {
    //     transactionListAsString += tx.toJson();
    // }

    // std::string hash = utils::hash(transactionListAsString);
    std::string lastHash = blocks[blocks.size()-1].hash;

    Wallet forgerWallet(forgerAddress.c_str(), node);
    executeTransactions(coveredTransactions);
    Block newBlock = forgerWallet.createBlock(coveredTransactions, 
                                              lastHash,
                                              blocks.size());
    blocks.push_back(newBlock);

    return newBlock;
}

bool Blockchain::transactionExists(Transaction transaction) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    for(auto block : blocks) {
        for(auto tx : block.transactions) {
            if (tx == transaction)
                return true;
        }
    }
    return false;
}

std::vector<std::string> Blockchain::txsByAddress(std::string address) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    std::vector<std::string> txids;
    for(auto block : blocks) {
        for(auto tx : block.transactions) {
            if (tx.senderAddress == address || tx.receiverAddress == address)
                txids.push_back(tx.id);    
        }
    }
    // std::cout << "txids: " << txids.size() << std::endl;
    return txids;
}

bool Blockchain::forgerValid(Block block) {
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

bool Blockchain::transactionValid(std::vector<Transaction> transactions) {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    std::vector<Transaction> coveredTransactions = this->node->blockchain->getCoveredTransactionSet(transactions);
    return coveredTransactions.size() == transactions.size();
}

vector<nlohmann::json> Blockchain::blockList(vector <Block> blocks) const {
    nlohmann::json j;
    vector<nlohmann::json> blks;

    for (auto block: blocks) {
        blks.push_back(block.jsonView());
    }
    return blks;
}

std::string Blockchain::nextForger() {
    std::lock_guard<std::mutex> guard(blockchainMutex);
    int blocksSize = blocks.size();
    std::string nextForger;
    std::string lastBlockHash;
    lastBlockHash = blocks[blocksSize-1].lastHash;
    nextForger = node->proofOfStake->forger(lastBlockHash);
    return nextForger;
}

nlohmann::json Blockchain::toJson() const {
    nlohmann::json j;

    j["blocks"] = blockList(blocks);
    return j;
}

std::string Blockchain::toJsonString(std::vector<Block> blocks) const {
    nlohmann::json j;

    j["blocks"] = blockList(blocks);
    return j.dump();
}

std::string Blockchain::toJsonWebView() const {
    nlohmann::json j;

    j["blocks"] = blockList(blocks);
    return j.dump();
}