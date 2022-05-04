#ifndef _LOT_H_
#define _LOT_H_
#include <vector>
#include <string>
#include <iostream>

class Lot {
public:
    Lot();
    Lot(std::string pubString, int i, std::string lBH); // Constructor
    ~Lot(); // Destructor

    // std::string lotHash() const;
    std::string publicKeyString{};
    int iteration{};
    std::string lastBlockHash{};
};

#endif // _LOT_H_
