#include <iostream>
#include "Blockchain.hpp"
#include "Transaction.hpp"
#include "TransactionPool.hpp"
#include "Wallet.hpp"
#include "Block.hpp"

int main() {

    Blockchain blockchain;

    std::string receiver {"receiverlasdsa87a8sd78asd7"};

    Wallet sonny_wallet;
    Wallet jane_wallet;
    Transaction transaction = jane_wallet.create_transaction(receiver, 1, tx.transfer);
    Transaction transaction2 = jane_wallet.create_transaction(receiver, 12, tx.transfer);
    Transaction transaction3 = jane_wallet.create_transaction(receiver, 12, tx.transfer);
    Transaction transaction4 = jane_wallet.create_transaction(receiver, 12, tx.transfer);
    
    TransactionPool pool;

    if (pool.transaction_exists(transaction) == false) {
        pool.add_transaction(transaction);
    }

    if (pool.transaction_exists(transaction2) == false) {
        pool.add_transaction(transaction2);
    }

    std::cout << "Blockchain:\n" << std::endl;
    Block last_block = blockchain.blocks[blockchain.blocks.size()-1];
    std::string last_hash = last_block.hash;
    int block_count = last_block.block_count + 1;
    
    Block block = sonny_wallet.create_block(pool.transactions, last_hash, block_count);
    
    if (!blockchain.last_block_hash_valid(block))
        std::cout << "Last block hash is not valid" << last_block.hash << std::endl;
    if (!blockchain.block_count_valid(block))
        std::cout << "Last block count is not valid" << std::endl;

    if (blockchain.last_block_hash_valid(block) && blockchain.block_count_valid(block)) {    
        blockchain.add_block(block);
        pool.transactions.clear();
    }

    if (pool.transaction_exists(transaction3) == false) {
        pool.add_transaction(transaction3);
    }

    if (pool.transaction_exists(transaction4) == false) {
        pool.add_transaction(transaction4);
    }

    Block last_block2 = blockchain.blocks[blockchain.blocks.size()-1];
    std::string last_hash2 = last_block2.hash;
    unsigned long long block_count2 = last_block2.block_count + 1;
    
    Block block2 = sonny_wallet.create_block(pool.transactions, last_hash2, block_count2);

    if (!blockchain.last_block_hash_valid(block2))
        std::cout << "Last block hash is not valid" << std::endl;
    if (!blockchain.block_count_valid(block2))
        std::cout << "Last block count is not valid" << std::endl;

    if (blockchain.last_block_hash_valid(block2) && blockchain.block_count_valid(block2)) {    
        blockchain.add_block(block2);
        pool.transactions.clear();
    }
    nlohmann::json j = blockchain.to_json();
    std::cout << j.dump(4) << std::endl;
    
	return 0;
}