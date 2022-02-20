#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <iostream>
#include <string>
#include <sstream>
#include <ostream>
#include <stdio.h>

class Wallet {
public:
    std::string publicKey;
    std::string privateKey;
    EVP_PKEY *pkey;

    Wallet();
    ~Wallet();

    void genKeyPair();
    const char * sign(std::string);
    bool verify(std::string, const char *);
};

Wallet::Wallet() {
    genKeyPair();
}
Wallet::~Wallet(){}

void Wallet::genKeyPair() {
    char *bp;
    size_t size;
    FILE *stream;

    const EVP_CIPHER *cipher = EVP_get_cipherbyname("secp256k1");
    pkey = EVP_EC_gen("secp256k1");
    if (pkey == NULL) {
        std::cerr << "Error generating the ECC key." << std::endl;
        return;
    }

    stream = open_memstream (&bp, &size);
    PEM_write_PUBKEY(stream, pkey);
    fflush (stream);
    publicKey = bp;
    fclose (stream);

    stream = open_memstream (&bp, &size);
    PEM_write_PrivateKey(stream, pkey, cipher, NULL, 0, NULL, NULL);
    fflush (stream);
    privateKey = bp;
    fclose (stream);

}

const char * Wallet::sign(std::string str) {
    EVP_PKEY_CTX *ctx;
    /* md is a SHA-256 digest in this example. */
    unsigned char *sig;
    size_t siglen;
    size_t mdlen = 32;
    unsigned char *md = (unsigned char *)str.c_str();

    ctx = EVP_PKEY_CTX_new(pkey, NULL /* no engine */);
    if (!ctx)
        std::cerr << "error creating ctx " << std::endl;

    if (EVP_PKEY_sign_init(ctx) <= 0)
        std::cerr << "error initializing sig ctx" << std::endl;

    if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0)
        std::cerr << "error setting signature md" << std::endl;

    /* Determine buffer length */
    if (EVP_PKEY_sign(ctx, NULL, &siglen, md, mdlen) <= 0)
        std::cerr << "error determining buffer length" << std::endl;

    sig = (unsigned char *)OPENSSL_malloc(siglen);

    if (!sig)
        std::cerr << "malloc failure" << std::endl;

    if (EVP_PKEY_sign(ctx, sig, &siglen, md, mdlen) <= 0) {
        std::cerr << "error creating signature" << std::endl;
        return nullptr;
    }

    static char out[64];
    size_t hexlen;
    //return OPENSSL_buf2hexstr(sig, sizeof(sig));
    OPENSSL_buf2hexstr_ex(out, sizeof(out), &hexlen, sig, sizeof(sig), '\0');
    return out;
}

bool Wallet::verify(std::string str, const char *signature) {
    EVP_PKEY_CTX *ctx;
    size_t mdlen = 32;
    size_t siglen = sizeof(signature);
    unsigned char *sig = (unsigned char *)signature;
    unsigned char *md = (unsigned char *)str.c_str();

    ctx = EVP_PKEY_CTX_new(pkey, NULL /* no engine */);
    if (!ctx)
        std::cerr << "ctx error" << std::endl;

    if (EVP_PKEY_verify_init(ctx) <= 0)
        std::cerr << "ctx verify error" << std::endl;

    if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0)
        std::cerr << "verify signature error" << std::endl;

    /* Perform operation */
    int ret = EVP_PKEY_verify(ctx, sig, siglen, md, mdlen);
    if (ret) {
        return true;
    } else {
        return false;
    }
}

std::string sha256(const std::string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)str.c_str(), str.length(), hash);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {ss << std::hex << (int)hash[i];}
    return ss.str();
}

int main() {
    Wallet wallet;
    std::cout << wallet.publicKey << std::endl;
    std::cout << wallet.privateKey << std::endl;

    const std::string data = "message to sign is a very long message that might have lots and lots of characters in it.";
    std::string hash = sha256(data);
    const char * signature = wallet.sign(hash);

    std::cout << "signature: " << signature << std::endl;
    bool ret = wallet.verify(hash, signature);
    std::cout << "hash: " << hash << " ";

    if (ret)
        std::cout << "Verified!" << std::endl;
    else
        std::cout << "Not verified." << hash << std::endl;


    return 0;
}