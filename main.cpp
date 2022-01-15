#include <iostream>
#include "Blockchain.hpp"
#include "Transaction.hpp"
#include "TransactionPool.hpp"
#include "Wallet.hpp"
#include "Block.hpp"

int main() {
    std::string receiver {"receiveryyyy"};

    Wallet sonny_wallet;
    Wallet jane_wallet;
    Transaction transaction = jane_wallet.create_transaction(receiver, 1, tx.transfer);
    Transaction transaction2 = jane_wallet.create_transaction(receiver, 12, tx.transfer);
    
    TransactionPool pool;

    if (pool.transaction_exists(transaction) == false) {
        pool.add_transaction(transaction);
    }

    if (pool.transaction_exists(transaction2) == false) {
        pool.add_transaction(transaction2);
    }

    std::cout << "Block:\n" << std::endl;
    Block block = sonny_wallet.create_block(pool.transactions, "lasthash", "hash", 1);
    nlohmann::json data = block.jsonview();
    std::cout << data.dump(4) << std::endl;

    bool signatureValid = utils::verify_signature(
        block.payload(), block.signature, sonny_wallet.public_key);
    std::cout << boolalpha << signatureValid << std::endl;
    
	return 0;
}