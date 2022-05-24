#include <iostream>
#include "Block.hpp"
#include "TransactionPool.hpp"
#include <nlohmann/json.hpp>

Block::Block()=default;

/*!
 *
 * @param transactions
 * @param lastHash
 * @param forgerAddress
 * @param blockCount
 *
 * Constructor for the Block class when the forger is local
 */
Block::Block(std::vector <Transaction> transactions,
             std::string lastHash,
             std::string forgerAddress,
             long blockCount)
    : transactions(std::move(transactions)),
      lastHash(std::move(lastHash)),
      forgerAddress(std::move(forgerAddress)),
      blockCount(blockCount) {}

/*!
*
* @param transactions
* @param lastHash
* @param blockCount
*
* Constructor for the Block class when the forger is NOT local
*/
Block::Block(std::vector <Transaction> transactions,
             std::string lastHash,
             long blockCount)
    : transactions(std::move(transactions)),
      lastHash(std::move(lastHash)),
      blockCount(blockCount) {}

/*!
 * Block destructor
 */
Block::~Block()=default;

/*!
 *
 * @param _signature
 *
 * Add the generated signature to the new block.
 */
void Block::sign(utils::Signature sig) {
    signature = std::move(sig.hex);
    signatureLength = sig.len;
}

/*!
 *
 * @param transactions
 * @return vector<nlohmann::json>
 *
 * Create a json representation for a list of given transactions
 */
std::vector<nlohmann::json> Block::transactionList(std::vector<Transaction> &transactions) {
    nlohmann::json j;
    std::vector<nlohmann::json> txs;

    for (auto const &itx: transactions) {
        j["senderAddress"] = itx.senderAddress;
        j["receiverAddress"] = itx.receiverAddress;
        j["amount"] = itx.amount;
        j["type"] = itx.type;
        j["id"] = itx.id;
        j["timestamp"] = itx.timestamp;
        j["signature"] = itx.signature;
        txs.push_back(j);
    }
    return txs;
}

/*!
 *
 * @return std::string
 *
 * Create a json representation of the block and return
 * it as a string.
 */
std::string Block::toJson() {
    nlohmann::json j;

    j["transactions"] = transactionList(transactions);
    j["lastHash"] = lastHash;
    j["hash"] = hash;
    j["forgerAddress"] = forgerAddress;
    j["timestamp"] = timestamp;
    j["blockCount"] = blockCount;
    j["signature"] = signature;
    j["_id"] = _id;

    return to_string(j);
}

/*!
 *
 * @return std::string
 *
 * Create a json representation of our payload for signing.
 */
std::string Block::payload() {
    nlohmann::json j;

    j["transactions"] = transactionList(transactions);
    j["lastHash"] = lastHash;
    j["hash"] = hash;
    j["forgerAddress"] = forgerAddress;
    j["timestamp"] = timestamp;
    j["blockCount"] = blockCount;
    j["signature"] = "";
    j["_id"] = _id;

    return to_string(j);
}

/*!
 *
 * @return lohmann::json
 *
 * Create a json representation of the block.
 */
nlohmann::json Block::jsonView() {
    nlohmann::json j;

    j["transactions"] = transactionList(transactions);
    j["lastHash"] = lastHash;
    j["hash"] = hash;
    j["forgerAddress"] = forgerAddress;
    j["timestamp"] = timestamp;
    j["blockCount"] = blockCount;
    j["signature"] = signature;
    j["_id"] = _id;
    
    return j;
}