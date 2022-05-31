#include <string>
#include "Transaction.hpp"
#include <nlohmann/json.hpp>
#include <utility>
#include "utils.hpp"

/*!
 *
 * @param senderAddress
 * @param receiverAddress
 * @param amount
 * @param type
 *
 * Transaction constructor
 */
Transaction::Transaction(std::string senderAddress, std::string receiverAddress, double amount, std::string type)
: senderAddress(std::move(senderAddress)), receiverAddress(std::move(receiverAddress)), amount(amount), type(std::move(type))  {
    id = utils::get_uuid();
    timestamp = utils::timeSinceEpoch();
}

/*!
 * Default transaction constructor
 */
Transaction::Transaction()=default;

/*!
 * Transaction destructor
 */
Transaction::~Transaction()=default;

/*!
 *
 * @param lhs
 * @param rhs
 * @return bool
 *
 * Overloaded == operator for comparing transactions using their id.
 */
bool operator==(const Transaction &lhs, const Transaction &rhs) {
    return lhs.id == rhs.id;
}

/*!
 *
 * @return std::string
 *
 * Stringified json representation of a transaction.
 */
std::string Transaction::toJson() const {
    nlohmann::json j;
    
    j["senderAddress"] = senderAddress;
    j["senderPublicKey"] = senderPublicKey;
    j["receiverAddress"] = receiverAddress;
    j["amount"] = amount;
    j["type"] = type;
    j["id"] = id;
    j["timestamp"] = timestamp;
    j["signature"] = signature;
    
    return j.dump();
}

/*!
 *
 * @return std::string
 *
 * stringified json repsentation of the transaction's payload, used for signing.
 */
std::string Transaction::payload() {
    nlohmann::json j;
    
    j["senderAddress"] = senderAddress;
    j["receiverAddress"] = receiverAddress;
    j["amount"] = amount;
    j["type"] = type;
    j["id"] = id;
    j["timestamp"] = timestamp;
    j["signature"] = "";
    
    return j.dump();
}

/*!
 *
 * @param sig
 *
 * Add signature to the transaction.
 */
void Transaction::sign(utils::Signature sig) {
    signature = std::move(sig.hex);
    signatureLength = sig.len;
}