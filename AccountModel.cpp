#include "AccountModel.hpp"

AccountModel::AccountModel() {
}

AccountModel::~AccountModel() {
}

bool AccountModel::accountExists(const std::string walletAddress) {
    std::sort (accounts.begin(), accounts.end());
    return std::binary_search (accounts.begin(), accounts.end(), walletAddress);
}

void AccountModel::addAccount(const std::string walletAddress) {
    if (!accountExists(walletAddress)) {
        accounts.push_back(walletAddress);
        balances[walletAddress] = 0;
    }
}

double AccountModel::getBalance(const std::string walletAddress) {
    if (!accountExists(walletAddress)) {
        addAccount(walletAddress);
    }
    return balances[walletAddress];
}

void AccountModel::updateBalance(const std::string walletAddress, double amount) {
    if (!accountExists(walletAddress)) {
        addAccount(walletAddress);
    }
    balances[walletAddress] += amount;
}