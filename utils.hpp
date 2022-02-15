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

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>

namespace utils {
    std::string hash(std::string dataStr);
    bool verifySignature(std::string dataStr, std::string signature, std::string publicKey);
    unsigned char random_char();
    std::string uuid_gen();
    char asciitolower(char in);
    std::string gen_random_str(const int len);
    int hexToInt(const char *hex);
    int ABS(int x);
}

#endif // _UTILS_H_