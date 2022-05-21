#include "utils.hpp"

using namespace utils;

/*!
 *
 * @param str
 * @return std::string
 *
 * SHA256 hash function.
 */
std::string utils::hash(const std::string &str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)str.c_str(), str.length(), hash);
    std::stringstream ss;
    for(unsigned char i : hash) {ss << std::hex << (int)i;}
    return ss.str();
}

/*!
 *
 * @return std::string
 *
 * UUID generator, found on stack overflow.
 */
std::string utils::get_uuid() {
    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::uniform_int_distribution<int> dist(0, 15);

    const char *v = "0123456789abcdef";
    const bool dash[] = {
        false, false, false, false, true, false, true, false, true, false, true, false, false, false, false, false
    };

    std::string res;
    for (bool i : dash) {
        if (i) res += "-";
        res += v[dist(rng)];
        res += v[dist(rng)];
    }
    return res;
}

/*!
 *
 * @param message
 * @param signature
 * @param signatureSize
 * @param publicKeyString
 * @return int
 *
 * Verify message with signature of signatureSize against the publicKeyString.
 * Function uses OpenSSL 3.
 */
int utils::verifySignature(const std::string &message,
                    const std::string &signature,
                    size_t &signatureSize,
                    const std::string &publicKeyString) {
    /* the easy way to translate your hex string back into your buffer */
    // long len;
    // unsigned char *sig = OPENSSL_hexstr2buf(signature.hexsig, &len);

    /* the not so easy way */
    unsigned char buf[256];
    const char *st = signature.c_str();
    size_t buflen;
    int rv = OPENSSL_hexstr2buf_ex(buf, 256, &buflen, st, '\0');
    if (rv == 0)
        return 0;

    /* generate the public key using the public key string */
    const char *mKey = publicKeyString.c_str();
    BIO* bo = BIO_new( BIO_s_mem() );
    BIO_write( bo, mKey,(int)strlen(mKey));
    EVP_PKEY* pkey = nullptr;
    PEM_read_bio_PUBKEY( bo, &pkey, nullptr, nullptr );
    /* free memory */
    BIO_free(bo);

    /* create the key context */
    EVP_PKEY_CTX *ctx;
    ctx = EVP_PKEY_CTX_new(pkey, nullptr /* no engine */);
    if (!ctx)
        std::cerr << "ctx error" << std::endl;

    /* initialize the context */
    if (EVP_PKEY_verify_init(ctx) <= 0)
        std::cerr << "ctx verify error" << std::endl;

    /* set the message digest type to sha256 */
    if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0)
        std::cerr << "verify signature error" << std::endl;

    /* Perform operation */
    auto *md = (unsigned char *)message.c_str();
    size_t mdlen = 32;
    auto siglen = (size_t)signatureSize;
    // std::cout << "v sig size: " << siglen << std::endl;
    int ret = EVP_PKEY_verify(ctx, buf, siglen, md, mdlen);

    /* free memory and return the result */
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);
    return ret;
}

/*!
 *
 * @param str
 * @param seperator
 * @return std::vector<std::string>
 *
 * string split function, returns a vector of strings split by seperator
 */
std::vector<std::string> utils::split(std::string &str, const char *seperator)
{
    char *token = strtok((char *)str.c_str(), seperator);

    std::vector<std::string> vec;
    while (token != nullptr)
    {
        vec.emplace_back(token);
        token = strtok(nullptr, ":");
    }

    return vec;
}

/*!
 *
 * @param portString
 * @return int
 *
 * getPort() takes a string and does some checking to see if it can be a valid port.
 * It converts the string to an int.
 */
int utils::getPort(char *portString) {
    char* p;
    errno = 0; // not 'int errno', because the '#include' already defined it
    auto port = (int)strtol(portString, &p, 10);

    if (*p != '\0' || errno != 0) {
        std::cout << "Invalid null or ernno not 0" << std::endl;
        return -1; // In main(), returning non-zero means failure
    }

    if (port < INT_MIN || port > INT_MAX) {
        std::cout << "Invalid port" << std::endl;
        return -1;
    }

    return port;
}

/*!
 *
 * @param str
 * @return std::string
 *
 * Generates a crypto address from a public key by hashing the
 * public key with a SHA1 hash. SHA1 is shorter than SHA256
 * and therefore better suited for a public crypto address.
 */
std::string utils::generateAddress(const std::string &str) {

    // The public crypto address is a SHA1
    // hex encoded string of the walletPublicKey.

    unsigned char hash[20];
    SHA1((unsigned char *)str.c_str(), str.length(), hash);
    std::stringstream ss;
    for(unsigned char i : hash) {ss << std::hex << (int)i;}
    return ss.str();
}

/*!
 *
 * @return vector
 *
 * Gets the IP and PORT of this node from config.json.
 */
std::vector<std::string> utils::get_ip_and_port_from_config() {
    try {
        std::ifstream config_file("../config.json");
        nlohmann::json j;
        config_file >> j;
        config_file.close();

        std::vector<std::string> config;
        config.push_back(j["master_server_ip"]);
        config.push_back(j["master_server_port"]);

        return config;
    } catch (std::exception &e) {
        std::cerr << "Couldn't open config file " << e.what() << std::endl;
        exit(0);
    }
}

/*!
 *
 * @param state
 * @return uint32_t
 *
 * A linear congruential generator (LCG) is an algorithm that
 * yields a sequence of pseudo-randomized numbers calculated
 * with a discontinuous piecewise linear equation. The method
 * represents one of the oldest and best-known pseudorandom
 * number generator algorithms.
 */
inline uint32_t utils::lcg_rand(uint32_t *state)
{
    return *state = (uint64_t)*state * 279470273u % 0xfffffffb;
}

/*!
 *
 * @param len
 * @return std::string
 *
 * Function used for testing purposes.
 */
std::string utils::gen_random_str(int len) {
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    uint32_t seed = 937162211; // seed our random number generator
    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[lcg_rand(&seed) % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

/*!
 * Grabs config info and puts it into ip_port_vec.
 */
void utils::startUtils()
{
    ip_port_vec = get_ip_and_port_from_config();
}

/*!
 *
 * @return string
 *
 * Getter for master node's ip
 */
std::string utils::get_master_node_ip() {
    std::string ip = ip_port_vec.at(0);
    return ip;
}

/*!
 *
 * @return int
 *
 * Getter for master node's port.
 */
int utils::get_master_node_port() {
    auto port = (int)stol(ip_port_vec.at(1));
    return port;
}

/*!
 *
 * @return time_t
 *
 * Return current time in unix format.
 */
time_t utils::timeSinceEpoch()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t( now );
}