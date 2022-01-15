#include <iostream>
#include "Blockchain.hpp"
#include "Transaction.hpp"
#include "Wallet.hpp"

int main() {
    std::string sender {"senderxxxx"};
    std::string receiver {"receiveryyyy"};
    Transaction transaction(sender, receiver, 1, tx.exchange);

    Wallet sonny_wallet;    
    CryptoPP::SecByteBlock sig = sonny_wallet.sign(transaction.to_json());
    //std::cout << "Signature:\n" << sig << std::endl;
    //transaction.sign(sig);
    std::cout << transaction.to_json() << std::endl;
    bool signatureValid = utils::verify_signature(transaction.to_json(), sig, sonny_wallet.public_key);

    std::cout << std::boolalpha;
    std::cout << signatureValid << std::endl;
    
	return 0;
}