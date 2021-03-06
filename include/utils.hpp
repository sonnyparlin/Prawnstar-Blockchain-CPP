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
#include <vector>
#include <random>
#include <chrono>

/*!
 * Namespace utils
 */
namespace utils {
    /*!
     * Signature struct
     * size_t signatureLength, std::string hexStringSignature
     */
    struct Signature {
        size_t len {}; /*!< signature length */
        std::string hex {}; /*!< hexSignature */
    };

    std::vector<std::string> get_ip_and_port_from_config();
    std::string get_master_node_ip();
    int get_master_node_port();
    void startUtils();
    std::string hash(const std::string&);
    int verifySignature(const std::string&,
                        const std::string&,
                        size_t&,
                        const std::string&);
    int getPort(char *portString);
    std::vector<std::string> split(std::string &str, const char *seperator);
    std::string generateAddress(const std::string &str);
    std::string get_uuid();
    inline uint32_t lcg_rand(uint32_t *state);
    std::string gen_random_str(int len);
    time_t timeSinceEpoch();
    int processArgs(int, char**);

    static std::vector<std::string> ip_port_vec {};
}

#endif // _UTILS_H_