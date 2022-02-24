#ifndef _PROOFOFSTAKE_H_
#define _PROOFOFSTAKE_H_
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <cmath>
#include "Lot.hpp"
#include "Node.hpp"

class Node;

class ProofOfStake {
private:
    Node *node;
public:
    std::unordered_map<std::string, double>stakers;

    ProofOfStake(Node *node, int);           // Constructor
    ~ProofOfStake();          // Destructor

    void update(std::string publicKeyString, double stake);
    double getStake(std::string publicKeyString);
    std::vector<Lot> validatorLots(std::string seed);
    Lot winnerLot(std::vector<Lot> lots, std::string seed);
    std::string forger(std::string lastBlockHash);
    void setGenesisNodeStake(std::string);
};

#endif // _PROOFOFSTAKE_H_
