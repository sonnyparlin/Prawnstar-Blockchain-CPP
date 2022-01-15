#include <iostream>
#include "Blockchain.hpp"
#include "Transaction.hpp"
#include "Wallet.hpp"

int main() {
    std::string sender {"senderxxxx"};
    std::string receiver {"receiveryyyy"};
    Transaction transaction(sender, receiver, 1, tx.exchange);

    Wallet sonny_wallet;    
    std::string sig = sonny_wallet.sign(transaction.to_json());
    //transaction.sign(sig);
    std::cout << sig << std::endl;
    
    std::cout << transaction.to_json() << std::endl;
    bool signatureValid = utils::verify_signature(transaction.to_json(), sig, sonny_wallet.public_key);

    std::cout << std::boolalpha;
    std::cout << signatureValid << std::endl;
    
	return 0;
}