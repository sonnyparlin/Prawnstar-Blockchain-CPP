#ifndef _PROOFOFSTAKE_H_
#define _PROOFOFSTAKE_H_
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <cmath>
#include "Lot.hpp"
#include "Node.hpp"
#include <mutex>

class Node;

class ProofOfStake {
private:
    Node *node;
public:
    std::unordered_map<std::string, double>stakers;
    std::mutex posMutex;

    explicit ProofOfStake(Node *node);           // Constructor
    ~ProofOfStake();          // Destructor

    void update(const std::string&, double stake);
    double getStake(const std::string&);
    std::vector<Lot> validatorLots(std::string seed);
    static Lot winnerLot(const std::vector<Lot>&, const std::string &);
    std::string forger(std::string lastBlockHash);
    void setGenesisNodeStake(const std::string&);
};

#endif // _PROOFOFSTAKE_H_
