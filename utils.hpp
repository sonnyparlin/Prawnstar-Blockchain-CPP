#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <cryptopp/cryptlib.h>
#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>

namespace utils {
    std::string hash(std::string dataStr);
    bool verify_signature(std::string dataStr, std::string signature, std::string publicKey);
}

#endif // _UTILS_H_