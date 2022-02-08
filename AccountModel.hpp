#ifndef _ACCOUNTMODEL_H_
#define _ACCOUNTMODEL_H_
#include <vector>
#include <string>
#include <unordered_map>

class AccountModel {
public:
    std::vector<std::string> accounts;
    std::unordered_map<std::string, std::string> addressToPublicKey;
    std::unordered_map<std::string, std::string> addressToPrivateKey;
    std::unordered_map<std::string, double> balances;

    AccountModel();           // Constructor
    AccountModel(std::string address);
    ~AccountModel();          // Destructor

    bool accountExists(const std::string walletAddress);
    void addAccount(const std::string walletAddress, const std::string pubKey, const std::string privKey);
    double getBalance(const std::string walletAddress);
    void updateBalance(const std::string walletAddress, double amount);
};

#endif // _ACCOUNTMODEL_H_