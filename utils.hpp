#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <cryptopp/cryptlib.h>
//#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/oids.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/base32.h>
#include <cryptopp/zdeflate.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>
#include <random>
#include "Wallet.hpp"

namespace utils {
    std::string hash(std::string dataStr);
    bool verify_signature(std::string dataStr, std::string signature, std::string publicKey);
    unsigned char random_char();
    std::string generate_uuid_hex(int len);
}

#endif // _UTILS_H_