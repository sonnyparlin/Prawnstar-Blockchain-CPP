#include "Lot.hpp"
#include "utils.hpp"

Lot::Lot(){
    
}

Lot::Lot(std::string pubString, int i, std::string lBH) {
    publicKeyString = pubString;
    iteration = i;
    lastBlockHash = lBH;
}

Lot::~Lot() {
}

std::string Lot::lotHash() {
    std::string hashData = publicKeyString + lastBlockHash;
    for (int i=0; i < iteration; i++) {
        hashData = utils::hash(hashData);
    }
    return hashData;
}