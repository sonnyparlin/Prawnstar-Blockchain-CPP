#ifndef _LOT_H_
#define _LOT_H_
#include <vector>
#include <string>
#include <iostream>

/*!
 * Lot class, used for creating lottery tickets for the proof of stake system.
 */
class Lot {
public:
    explicit Lot(std::string); // Constructor
    ~Lot(); // Destructor

    // std::string lotHash() const;
    std::string publicKeyString{}; /*!< publicKeyString used for identifying Lot ownership */
};

#endif // _LOT_H_
