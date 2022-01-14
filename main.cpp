#include <iostream>
#include "Blockchain.hpp"
#include "Transaction.hpp"
#include "Wallet.hpp"

int main() {
    Blockchain blockchain;    
    blockchain.display();

    std::string sender {"sender"};
    std::string receiver {"receiver"};
    Transaction transaction(sender, receiver, 1, tx.exchange);
    transaction.display();
    std::cout << transaction.to_json() << std::endl;

    Wallet sonny_wallet;
    std::cout << std::endl << "Private: " << sonny_wallet.private_key << endl;
    std::cout << std::endl << "Public: " << sonny_wallet.public_key << endl;
    
    std::string somedata{"some data"};
    std::cout << std::endl << sonny_wallet.sign(somedata) << endl;
    
    
	return 0;
}