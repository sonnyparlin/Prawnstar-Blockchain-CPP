#ifndef _ACCOUNTMODEL_H_
#define _ACCOUNTMODEL_H_
#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>
#include <algorithm>

/*!
 * Account Model class, used for holding account information about users.
 */
class AccountModel {
public:
    AccountModel();
    ~AccountModel();

    bool accountExists(const std::string &walletAddress);
    void addAccount(const std::string &walletAddress,
                    const std::string &pubKey,
                    const std::string &privKey);
    void addAccount(const std::string &walletAddress, const std::string &pubKey);
    double getBalance(const std::string &walletAddress);
    void updateBalance(const std::string &walletAddress, double amount);

    std::unordered_map<std::string, double> balances {}; /*!< address to balances map */
    std::unordered_map<std::string, std::string> addressToPublicKey {}; /*!< address to Public Key map*/
    std::unordered_map<std::string, std::string> addressToPrivateKey {}; /*!< address to private key map*/

private:
    std::mutex acMutex {}; /*!< mutex for locking shared data */
    std::vector<std::string> accounts {}; /*!< list of accounts */
};

#endif // _ACCOUNTMODEL_H_