#include "AccountModel.hpp"

AccountModel::AccountModel()=default;

AccountModel::~AccountModel()=default;

bool AccountModel::accountExists(const std::string &walletAddress) {
    return std::find(accounts.begin(), accounts.end(), walletAddress) != accounts.end();
}

void AccountModel::addAccount(const std::string &walletAddress,
                              const std::string &pubKey,
                              const std::string &privkey) {
    if (!accountExists(walletAddress)) {
        accounts.push_back(walletAddress);
        balances[walletAddress] = 0;
        addressToPublicKey.insert(std::pair<std::string,std::string>(walletAddress, pubKey));
        addressToPrivateKey.insert(std::pair<std::string,std::string>(walletAddress, privkey));
    }
}

void AccountModel::addAccount(const std::string &walletAddress, const std::string &pubKey) {
    if (!accountExists(walletAddress)) {
        accounts.push_back(walletAddress);
        balances[walletAddress] = 0;
        addressToPublicKey.insert(std::pair<std::string,std::string>(walletAddress, pubKey));
    }
}

double AccountModel::getBalance(const std::string &walletAddress) {
    std::lock_guard<std::mutex> guard(acMutex);
    return balances[walletAddress];
}

void AccountModel::updateBalance(const std::string &walletAddress, double amount) {
    std::lock_guard<std::mutex> guard(acMutex);
    balances[walletAddress] += amount;
}