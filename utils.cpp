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

    bool verifySignature(std::string message, std::string signature, std::string publicKeyString) {
        /* the easy way to translate your hex string back into your buffer */
        // long len;
        // unsigned char *sig = OPENSSL_hexstr2buf(signature.hexsig, &len);

        /* the not so easy way */
        unsigned char buf[256];
        const char *st = signature.c_str();
        size_t buflen;
        OPENSSL_hexstr2buf_ex(buf, 256, &buflen, st, '\0');

        /* generate the pubic key using the public key string */
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
        if (!ctx) {
            std::cerr << "ctx error" << std::endl;
            return false;
        }
        /* initialize the context */
        if (EVP_PKEY_verify_init(ctx) <= 0) {
            std::cerr << "ctx verify error" << std::endl;
            return false;
        }
        /* set the message digest type to sha256 */
        if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0) {
            std::cerr << "verify signature error" << std::endl;
            return false;
        }

        /* Perform operation */
        unsigned char *md = (unsigned char *)message.c_str();
        size_t mdlen = 32;
        size_t siglen = signature.length();
        int ret = EVP_PKEY_verify(ctx, buf, siglen, md, mdlen);

        /* free memory and return the result */
        EVP_PKEY_free(pkey);
        EVP_PKEY_CTX_free(ctx);
        return ret;
    }

    unsigned char random_char() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        return static_cast<unsigned char>(dis(gen));
    }

    char asciitolower(char in) {
        if (in <= 'Z' && in >= 'A')
            return in - ('Z' - 'z');
        return in;
    }

    std::string uuid_gen() {
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        return to_string(uuid);
    }

    std::string gen_random_str(const int len) {
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        std::string tmp_s;
        tmp_s.reserve(len);

        srand (time(NULL));
        for (int i = 0; i < len; ++i) {
            tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
        }
        
        //std::cout << tmp_s << std::endl;
        return tmp_s;
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
}