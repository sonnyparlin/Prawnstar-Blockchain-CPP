#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <random>
#include <sstream>
#include <string>

using namespace std;

struct TX_TYPE {
    string transfer;
    string exchange;
    string stake;
    string reward;
};

extern struct TX_TYPE tx;
time_t time_since_epoch();

#endif // _CONFIG_H_