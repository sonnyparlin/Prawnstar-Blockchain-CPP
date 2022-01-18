#include <iostream>
#include "Blockchain.hpp"
#include "Transaction.hpp"
#include "TransactionPool.hpp"
#include "Wallet.hpp"
#include "Block.hpp"

int main() {

    Blockchain blockchain;
    std::string receiver {"receiverlasdsa87a8sd78asd7"};
    
    Wallet jane_wallet("private.ec.der");
    Transaction transaction = jane_wallet.create_transaction(receiver, 1, tx.transfer);
    bool r = utils::verify_signature(transaction.payload(), transaction.signature, jane_wallet.public_key);
    if (!r)
        std::cout << "Transaction signature verification failed" << std::endl;

    TransactionPool pool;
    if (pool.transaction_exists(transaction) == false && utils::verify_signature(transaction.payload(), transaction.signature, jane_wallet.public_key)) {
        pool.add_transaction(transaction);
    }

    std::cout << "Blockchain:\n" << std::endl;
    Block last_block = blockchain.blocks[blockchain.blocks.size()-1];
    std::string last_hash = last_block.hash;
    int block_count = last_block.block_count + 1;
    
    Block block = jane_wallet.create_block(pool.transactions, last_hash, block_count);
    
    bool signature_valid = false;
    if (!blockchain.last_block_hash_valid(block))
        std::cout << "Last block hash is not valid" << last_block.hash << std::endl;
    if (!blockchain.block_count_valid(block))
        std::cout << "Last block count is not valid" << std::endl;
    if (!utils::verify_signature(block.payload(), block.signature, jane_wallet.public_key))
        std::cout << "Block signature is not valid " + block.signature << std::endl;
    else
        signature_valid = true;

    if (blockchain.last_block_hash_valid(block) 
            && blockchain.block_count_valid(block)
            && signature_valid) {    
        blockchain.add_block(block);
        pool.transactions.clear();
    }

    nlohmann::json j = blockchain.to_json();
    std::cout << j.dump(4) << std::endl;
    
	return 0;
}