#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <string>
#include <chrono>

struct TX_TYPE {
    std::string transfer;
    std::string exchange;
    std::string stake;
    std::string reward;
};

extern struct TX_TYPE tx;
time_t timeSinceEpoch();

#endif // _CONFIG_H_