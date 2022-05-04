#include "Lot.hpp"

/*!
 * Default constructor
 */
Lot::Lot()=default;

/*!
 *
 * @param _publicKeyString
 * @param _iteration
 * @param _lastBlockHash
 */
Lot::Lot(std::string _publicKeyString, int _iteration, std::string _lastBlockHash) {
    publicKeyString = std::move(_publicKeyString);
    iteration = _iteration;
    lastBlockHash = std::move(_lastBlockHash);
}

Lot::~Lot()=default;