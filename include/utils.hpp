#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/ec.h>

#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>

namespace utils {
    struct Signature {
        size_t _size;
        std::string hexsig;
    };

    std::vector<std::string> get_ip_and_port_from_config();
    static std::vector<std::string> ip_port_vec = get_ip_and_port_from_config();
    static std::string master_ip = ip_port_vec.at(0);
    static const char *MASTER_NODE_IP = master_ip.c_str();
    const static int MASTER_NODE_PORT=stoi(ip_port_vec.at(1));
    const static int BLOCKTIMER = 300;
    std::string hash(const std::string&);
    int verifySignature(const std::string&,
                        const std::string&,
                        const std::string&);
    int getPort(char *portString);
    std::vector<std::string> split(std::string str, const char *seperator);
    std::string generateAddress(const std::string &str);
    std::string get_uuid();
    std::string gen_random_str(int len);
}

#endif // _UTILS_H_