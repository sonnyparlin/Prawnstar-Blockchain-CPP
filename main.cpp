#include <iostream>
#include "Blockchain.hpp"
#include "Transaction.hpp"
#include "Wallet.hpp"

int main() {
    std::string sender {"senderxxxx"};
    std::string receiver {"receiveryyyy"};
    //Transaction transaction(sender, receiver, 1, tx.exchange);

    Wallet sonny_wallet;
    Transaction transaction = sonny_wallet.create_transaction(receiver, 1, tx.transfer);
    std::cout << transaction.to_json() << std::endl;

    bool verified = utils::verify_signature(transaction.payload(), transaction.signature, sonny_wallet.public_key);
    if (verified) {
        std::cout << "transaction verified!" << std::endl;
    }
    
	return 0;
}