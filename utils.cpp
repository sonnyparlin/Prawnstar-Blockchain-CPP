#include "utils.hpp"

namespace utils {

    std::string hash(std::string str)
    {   
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256((unsigned char *)str.c_str(), str.length(), hash);
        std::stringstream ss;
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {ss << std::hex << (int)hash[i];}
        return ss.str();
    }

    std::string get_uuid() {
        static std::random_device dev;
        static std::mt19937 rng(dev());

        std::uniform_int_distribution<int> dist(0, 15);

        const char *v = "0123456789abcdef";
        const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

        std::string res;
        for (int i = 0; i < 16; i++) {
            if (dash[i]) res += "-";
            res += v[dist(rng)];
            res += v[dist(rng)];
        }
        return res;
    }

    int verifySignature(std::string message, std::string signature, std::string publicKeyString) {
        /* the easy way to translate your hex string back into your buffer */
        // long len;
        // unsigned char *sig = OPENSSL_hexstr2buf(signature.hexsig, &len);

        std::vector<std::string> sigParts = split(signature, ":");
        std::string str1 = ":";

        size_t found = signature.find(str1);
        if (found == std::string::npos)
            return 0;

        // std::cout << sigParts.at(0) << " : " << sigParts.at(1) << std::endl;

        /* the not so easy way */
        unsigned char buf[256];
        const char *st = sigParts.at(0).c_str();
        size_t buflen;
        int rv = OPENSSL_hexstr2buf_ex(buf, 256, &buflen, st, '\0');
        if (rv == 0)
            return 0;

        /* generate the public key using the public key string */
        const char *mKey = publicKeyString.c_str();
        BIO* bo = BIO_new( BIO_s_mem() );
        BIO_write( bo, mKey,strlen(mKey));
        EVP_PKEY* pkey = 0;
        PEM_read_bio_PUBKEY( bo, &pkey, 0, 0 );
        /* free memory */
        BIO_free(bo);

        /* create the key context */
        EVP_PKEY_CTX *ctx;
        ctx = EVP_PKEY_CTX_new(pkey, NULL /* no engine */);
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
        size_t siglen = (size_t)stoi(sigParts.at(1));
        // std::cout << "v sig size: " << siglen << std::endl;
        int ret = EVP_PKEY_verify(ctx, buf, siglen, md, mdlen);

        /* free memory and return the result */
        EVP_PKEY_free(pkey);
        EVP_PKEY_CTX_free(ctx);
        return ret;
    }

    std::vector<std::string> split(std::string str, const char *seperator)  
    {  
        char *token = strtok((char *)str.c_str(), seperator);

        std::vector<std::string> ipPortStrV;
        while (token != NULL)
        {
            ipPortStrV.push_back(token);
            token = strtok(NULL, ":");
        }

        return ipPortStrV;
    }

    char asciitolower(char in) {
        if (in <= 'Z' && in >= 'A')
            return in - ('Z' - 'z');
        return in;
    }

    int getPort(char *portString) {
        char* p;
        errno = 0; // not 'int errno', because the '#include' already defined it
        long port = strtol(portString, &p, 10);

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

    std::string generateAddress(const std::string str) {

        // The public crypto address is a SHA1 
        // hex encoded string of the walletPublicKey.

        unsigned char hash[20];
        SHA1((unsigned char *)str.c_str(), str.length(), hash);
        std::stringstream ss;
        for(int i = 0; i < 20; i++) {ss << std::hex << (int)hash[i];}
        return ss.str();
    }

    std::vector<std::string> get_ip_and_port_from_config() {
        std::ifstream config_file("../config.json");
        if (!config_file) // Check the state of the file
        {
            std::cerr << "Couldn't open config file\n";
            return {};
        }
        nlohmann::json j;
        config_file >> j;
        config_file.close();

        std::vector<std::string> config;
        config.push_back(j["master_server_ip"]);
        config.push_back(j["master_server_port"]);

        return config;
    }

    std::string gen_random_str(const int len) {
        static const char alphanum[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
        std::string tmp_s;
        tmp_s.reserve(len);

        for (int i = 0; i < len; ++i) {
            tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
        }

        return tmp_s;
    }
}