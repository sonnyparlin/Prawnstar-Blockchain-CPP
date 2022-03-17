#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <random>
#include <sstream>
#include <string>

#ifdef _WIN32
#include <chrono>
#endif

using namespace std;

struct TX_TYPE {
    string transfer;
    string exchange;
    string stake;
    string reward;
};

extern struct TX_TYPE tx;
time_t timeSinceEpoch();

#endif // _CONFIG_H_