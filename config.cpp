#include "config.hpp"

struct TX_TYPE tx =
{
    .transfer = "TRANSFER",
    .exchange = "EXCHANGE",
    .stake    = "STAKE",
    .reward   = "REWARD"
};

time_t timeSinceEpoch()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t( now );
}