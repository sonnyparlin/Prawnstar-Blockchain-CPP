#ifndef _LOT_H_
#define _LOT_H_
#include <vector>
#include <string>
#include <iostream>

class Lot {
public:
    std::string publicKeyString;
    int iteration {0};
    std::string lastBlockHash;

    Lot();
    Lot(std::string &pubString, int i, std::string &lBH); // Constructor
    ~Lot(); // Destructor

    std::string lotHash() const;
};

#endif // _LOT_H_
