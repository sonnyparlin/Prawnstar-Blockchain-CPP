#include <iostream>
#include "Blockchain.hpp"
#include "Block.hpp"
#include "utils.hpp"

Blockchain::Blockchain(Node *node) {
    this->node = node;
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
    //blocks.reserve(1000);
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
            std::runtime_error notCovered("Transaction is not covered by the sender");
            throw notCovered;
        }
    }
    return coveredTransactions;
}

void Blockchain::executeTransactions(std::vector<Transaction> transactions) {
    for (auto transaction : transactions) {
        executeTransaction(transaction);
    }
}

void Blockchain::executeTransaction(Transaction transaction) {
    if (transaction.type == "STAKE") {
        std::string sender = transaction.senderAddress;
        std::string receiver = transaction.receiverAddress;

        if (sender == receiver) {
            double amount = transaction.amount;
            node->proofOfStake->update(sender, amount);
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

Block Blockchain::createBlock(std::vector<Transaction> transactionsFromPool, std::string forgerAddress) {
    std::vector<Transaction> coveredTransactions = getCoveredTransactionSet(
        transactionsFromPool
    );

    std::string transactionListAsString;
    for (auto tx : coveredTransactions) {
        transactionListAsString += tx.toJson();
    }

    std::string hash = utils::hash(transactionListAsString);
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
    for(auto block : blocks) {
        for(auto tx : block.transactions) {
            if (transaction.equals(tx))
                return true;
        }
    }
    return false;
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
    std::string lastBlockHash = blocks[blocks.size()-1].lastHash;
    std::string nextForger = node->proofOfStake->forger(lastBlockHash);
    return nextForger;
}

nlohmann::json Blockchain::toJson() const {
    nlohmann::json j;

    j["blocks"] = blockList(blocks);
    return j;
}

std::string Blockchain::toJsonString() const {
    nlohmann::json j;

    j["blocks"] = blockList(blocks);
    return j.dump();
}