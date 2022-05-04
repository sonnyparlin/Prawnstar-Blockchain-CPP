#ifndef _LOT_H_
#define _LOT_H_
#include <vector>
#include <string>
#include <iostream>

class Lot {
public:
    Lot();
    Lot(std::string, int, std::string); // Constructor
    ~Lot(); // Destructor

    // std::string lotHash() const;
    std::string publicKeyString{}; /*!< publicKeyString used for identifying Lot ownership */
    int iteration{}; /*!< Iteration variable */
    std::string lastBlockHash{}; /*!< LastBlockHash */
};

#endif // _LOT_H_
