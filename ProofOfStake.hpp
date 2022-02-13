#ifndef _PROOFOFSTAKE_H_
#define _PROOFOFSTAKE_H_
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

class ProofOfStake {
public:
    std::unordered_map<std::string, double>stakers;

    ProofOfStake();           // Constructor
    ~ProofOfStake();          // Destructor

    void update(std::string publicKeyString, double stake);
    double getStake(std::string publicKeyString);
};

#endif // _PROOFOFSTAKE_H_
