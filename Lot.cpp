#include "Lot.hpp"

/*!
 *
 * @param _publicKeyString
 *
 * Used to create lottery tickets for the next forger.
 */
Lot::Lot(std::string _publicKeyString) {
    publicKeyString = std::move(_publicKeyString);
}

Lot::~Lot()=default;