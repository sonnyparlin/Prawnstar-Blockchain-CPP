#include "Staker.hpp"

Staker::Staker(std::string& pubkey, std::string &type, double& stake)
:stakerPublicKey(std::move(pubkey)), stakerType(std::move(type)), stakerStake(stake) {}

Staker::~Staker()=default;

double Staker::getStake(std::string &pubkey)
{
    return stakerStake;
}

double Staker::updateStake(std::string& pubkey, double&& amount)
{
    return stakerStake += amount;
}