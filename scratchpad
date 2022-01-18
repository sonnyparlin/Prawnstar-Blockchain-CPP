#include <iostream>
#include "Blockchain.hpp"
#include "Transaction.hpp"
#include "TransactionPool.hpp"
#include "Wallet.hpp"
#include "Block.hpp"
#include "AccountModel.hpp"

int main() {

    Blockchain blockchain;
    
    Wallet janeWallet("private.ec.der");
    Wallet exchange;
    Wallet bobWallet;
    Wallet forgerWallet;
    TransactionPool pool;
    
    Transaction exchangeTransaction = exchange.createTransaction(janeWallet.address, 10, tx.exchange);
    bool rr = utils::verifySignature(exchangeTransaction.payload(), exchangeTransaction.signature, exchange.walletPublicKey);
    if (!rr)
        std::cout << "Transaction signature verification failed" << std::endl;

    if (pool.transactionExists(exchangeTransaction) == false && utils::verifySignature(exchangeTransaction.payload(), exchangeTransaction.signature, exchange.walletPublicKey)) {
        pool.addTransaction(exchangeTransaction);
    }

    std::vector<Transaction> coveredTransactions = blockchain.getCoveredTransactionSet(pool.transactions);

    if (!coveredTransactions.empty()) {    
        Block lastBlock = blockchain.blocks[blockchain.blocks.size()-1];
        std::string lastHash = lastBlock.hash;
        int blockCount = lastBlock.blockCount + 1;

        Block block = forgerWallet.createBlock(coveredTransactions, lastHash, blockCount);
        
        bool signature_valid = false;
        if (!blockchain.lastBlockHashValid(block))
            std::cout << "Last block hash is not valid" << lastBlock.hash << std::endl;
        if (!blockchain.blockCountValid(block))
            std::cout << "Last block count is not valid" << std::endl;
        if (!utils::verifySignature(block.payload(), block.signature, forgerWallet.walletPublicKey))
            std::cout << "Block signature is not valid " + block.signature << std::endl;
        else
            signature_valid = true;

        if (blockchain.lastBlockHashValid(block) 
                && blockchain.blockCountValid(block)
                && signature_valid) {    
            blockchain.addBlock(block);
            pool.removeFromPool(block.transactions);
        }
    }

    Transaction transaction = janeWallet.createTransaction(bobWallet.address, 5, tx.transfer);
    bool r = utils::verifySignature(transaction.payload(), transaction.signature, janeWallet.walletPublicKey);
    if (!r)
        std::cout << "Transaction signature verification failed" << std::endl;

    if (pool.transactionExists(transaction) == false && utils::verifySignature(transaction.payload(), transaction.signature, janeWallet.walletPublicKey)) {
        pool.addTransaction(transaction);
    }

    std::vector<Transaction> coveredTransactions2 = blockchain.getCoveredTransactionSet(pool.transactions);

    if (!coveredTransactions2.empty()) {

        Block lastBlock2 = blockchain.blocks[blockchain.blocks.size()-1];
        std::string lastHash2 = lastBlock2.hash;
        int blockCount2 = lastBlock2.blockCount + 1;

        Block block2 = forgerWallet.createBlock(coveredTransactions2, lastHash2, blockCount2);
        
        bool signature_valid = false;
        if (!blockchain.lastBlockHashValid(block2))
            std::cout << "Last block hash is not valid" << lastBlock2.hash << std::endl;
        if (!blockchain.blockCountValid(block2))
            std::cout << "Last block count is not valid" << std::endl;
        if (!utils::verifySignature(block2.payload(), block2.signature, forgerWallet.walletPublicKey))
            std::cout << "Block signature is not valid " + block2.signature << std::endl;
        else
            signature_valid = true;

        if (blockchain.lastBlockHashValid(block2) 
                && blockchain.blockCountValid(block2)
                && signature_valid) {    
            blockchain.addBlock(block2);
            pool.removeFromPool(block2.transactions);
        }
    }

    nlohmann::json j = blockchain.toJson();
    std::cout << j.dump(4) << std::endl;
    
	return 0;
}