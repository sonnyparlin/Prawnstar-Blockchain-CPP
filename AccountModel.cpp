#include "AccountModel.hpp"

AccountModel::AccountModel() {
}

AccountModel::AccountModel(std::string address) {
    std::string publicKey = addressToPublicKey[address];
}

AccountModel::~AccountModel() {
}

bool AccountModel::accountExists(const std::string walletAddress) {
    std::sort (accounts.begin(), accounts.end());
    return std::binary_search (accounts.begin(), accounts.end(), walletAddress);
}

void AccountModel::addAccount(std::string walletAddress, std::string pubKey, std::string privkey) {
    if (!accountExists(walletAddress)) {
        accounts.push_back(walletAddress);
        balances[walletAddress] = 0;
        addressToPublicKey.insert(std::pair<std::string,std::string>(walletAddress, pubKey));
        addressToPrivateKey.insert(std::pair<std::string,std::string>(walletAddress, privkey));
    }
}

double AccountModel::getBalance(const std::string walletAddress) {
    return balances[walletAddress];
}

void AccountModel::updateBalance(const std::string walletAddress, double amount) {
    balances[walletAddress] += amount;
}