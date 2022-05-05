#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <string>
#include <vector>
#include "TransactionPool.hpp"
#include <ctime>
#include <nlohmann/json.hpp>
#include "utils.hpp"

/*!
 * Block, class for holding block info.
 */
class Block {
public:
    // Constructors
    Block();
    Block(std::vector <Transaction>,
          std::string,
          std::string,
          long blockCount);
    Block(std::vector <Transaction>,
          std::string,
          long blockCount);
    ~Block();

    std::string toJson();
    std::string payload();
    void sign(std::string);
    static vector<nlohmann::json> transactionList(vector <Transaction> &transactions);
    nlohmann::json jsonView();

    std::vector <Transaction> transactions {}; /*!< list of block transactions */
    std::string lastHash {}; /*!< block last hash */
    std::string hash {}; /*!< current block hash */
    std::string forgerAddress {}; /*!< forger address of current block's forger */
    time_t timestamp = utils::timeSinceEpoch(); /*!< timestamp of block */
    long blockCount {}; /*!< block count */
    std::string signature {}; /*!< block signature */
    std::string _id {}; /*!< _id for use with mongodb if functionality is added */
    size_t signatureLength {}; /*!< signature length */
};

#endif // _BLOCK_H_