#include "Lot.hpp"

/*!
 *
 * @param _publicKeyString
 * @param _iteration
 * @param _lastBlockHash
 *
 * Used to create lottery tickets for the next forger.
 */
Lot::Lot(std::string _publicKeyString) {
    publicKeyString = std::move(_publicKeyString);
}

Lot::~Lot()=default;