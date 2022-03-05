#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>
#include <random>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>

namespace utils {
    struct Signature {
        size_t _size;
        std::string hexsig;
    };

    static const char * MASTER_NODE_IP = "127.0.0.1";
    static int MASTER_NODE_PORT=10001;
    std::string hash(std::string dataStr);
    bool verifySignature(std::string dataStr, std::string, std::string publicKey);
    unsigned char random_char();
    std::string uuid_gen();
    char asciitolower(char in);
    std::string gen_random_str(const int len);
    int hexToInt(const char *hex);
    int ABS(int x);
    int getPort(char *portString);
    std::vector<std::string> split(std::string str, const char *seperator);
    std::string generateAddress(const std::string str);
}

#endif // _UTILS_H_