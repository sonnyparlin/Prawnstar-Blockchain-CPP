#include "AccountModel.hpp"

AccountModel::AccountModel()=default;
AccountModel::~AccountModel()=default;

/*!
 * @param walletAddress
 * @return bool
 *
 * Check if the account exists, return true if found, false otherwise.
 */
bool AccountModel::accountExists(const std::string &walletAddress) {
    return std::find(accounts.begin(), accounts.end(), walletAddress) != accounts.end();
}

/*!
 *
 * @param walletAddress
 * @param pubKey
 * @param privkey
 *
 * Adds a new account to the account model with a private key and an initial value of 0
 */
void AccountModel::addAccount(const std::string &walletAddress,
                              const std::string &pubKey,
                              const std::string &privkey) {
    std::lock_guard<std::mutex> guard(acMutex);

    if (!accountExists(walletAddress)) {
        accounts.push_back(walletAddress);
        balances[walletAddress] = 0;
        addressToPublicKey.insert(std::pair<std::string,std::string>(walletAddress, pubKey));
        addressToPrivateKey.insert(std::pair<std::string,std::string>(walletAddress, privkey));
    }
}

/*!
 *
 * @param walletAddress
 * @param pubKey
 *
 * Adds a new account to the account model without a private key and an initial value of 0
 */
void AccountModel::addAccount(const std::string &walletAddress, const std::string &pubKey) {
    std::lock_guard<std::mutex> guard(acMutex);

    if (!accountExists(walletAddress)) {
        accounts.push_back(walletAddress);
        balances[walletAddress] = 0;
        addressToPublicKey.insert(std::pair<std::string,std::string>(walletAddress, pubKey));
    }
}

/*!
 *
 * @param walletAddress
 * @return double
 *
 * Get the balance of an account in the accountModel
 */
double AccountModel::getBalance(const std::string &walletAddress) {
    std::lock_guard<std::mutex> guard(acMutex);
    return balances[walletAddress];
}

/*!
 *
 * @param walletAddress
 * @param amount
 *
 * Updates the balance of an account in the accountModel
 */
void AccountModel::updateBalance(const std::string &walletAddress, double amount) {
    std::lock_guard<std::mutex> guard(acMutex);
    balances[walletAddress] += amount;
}