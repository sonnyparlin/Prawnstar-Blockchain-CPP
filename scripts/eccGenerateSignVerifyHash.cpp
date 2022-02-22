#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <ostream>
#include <stdio.h>

class Wallet {
public:
    std::string publicKey;
    std::string privateKey;
    std::string address;

    Wallet();
    ~Wallet();

    void genKeyPair();
    struct Signature sign(std::string);
    int verify(std::string, struct Signature, std::string);
};

Wallet::Wallet() {
    genKeyPair();
}
Wallet::~Wallet(){}

struct Signature {
    size_t _size;
    char *hexsig;
};

std::string sha256(const std::string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)str.c_str(), str.length(), hash);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {ss << std::hex << (int)hash[i];}
    return ss.str();
}

std::string sha1(const std::string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA1((unsigned char *)str.c_str(), str.length(), hash);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {ss << std::hex << (int)hash[i];}
    return ss.str();
}

void Wallet::genKeyPair() {
    char *bp;
    size_t size;
    FILE *stream;
    const char *curve = "secp256k1";

    const EVP_CIPHER *cipher = EVP_get_cipherbyname(curve);
    EVP_PKEY *pkey = EVP_EC_gen(curve);
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

    EVP_PKEY_free(pkey);

    address = "pv1" + sha1(publicKey);
}

struct Signature Wallet::sign(std::string str) {
    EVP_PKEY_CTX *ctx;
    unsigned char *sig;
    size_t siglen {0};
    size_t mdlen = 32;

    unsigned char *md = (unsigned char *)str.c_str();
    const char *mKey = privateKey.c_str();
    BIO* bo = BIO_new( BIO_s_mem() );
    BIO_write( bo, mKey,strlen(mKey));

    EVP_PKEY* pkey = 0;
    PEM_read_bio_PrivateKey( bo, &pkey, 0, 0 );
    BIO_free(bo);

    ctx = EVP_PKEY_CTX_new(pkey, NULL /* no engine */);
    if (!ctx)
        std::cerr << "error creating ctx" << std::endl;

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
    }

    EVP_PKEY_free(pkey);

    Signature mysig;
    mysig.hexsig = OPENSSL_buf2hexstr(sig, siglen);
    mysig._size = siglen;
    std::cout << "Signature converted to hex: " << std::endl;
    std::cout << mysig.hexsig << "\n" << std::endl;
    return mysig;
}

int Wallet::verify(std::string str, Signature signature, std::string publicKeyString) {
    EVP_PKEY_CTX *ctx;
    size_t mdlen = 32;
    size_t siglen = signature._size;
    long len;
    unsigned char *sig = OPENSSL_hexstr2buf(signature.hexsig, &len);
    unsigned char *md = (unsigned char *)str.c_str();

    const char *mKey = publicKeyString.c_str();
    BIO* bo = BIO_new( BIO_s_mem() );
    BIO_write( bo, mKey,strlen(mKey));

    EVP_PKEY* pkey = 0;
    PEM_read_bio_PUBKEY( bo, &pkey, 0, 0 );
    BIO_free(bo);

    ctx = EVP_PKEY_CTX_new(pkey, NULL /* no engine */);
    if (!ctx)
        std::cerr << "ctx error" << std::endl;

    if (EVP_PKEY_verify_init(ctx) <= 0)
        std::cerr << "ctx verify error" << std::endl;

    if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0)
        std::cerr << "verify signature error" << std::endl;

    EVP_PKEY_free(pkey);    

    /* Perform operation */
    int ret = EVP_PKEY_verify(ctx, sig, siglen, md, mdlen);
    return ret;
}

int main() {
    Wallet wallet;
    // std::cout << wallet.publicKey << std::endl;
    // std::cout << wallet.privateKey << std::endl;
    // std::cout << wallet.address << std::endl;
    // std::cout << "=======================================" << std::endl;

    const std::string data = "This is my wonderful message.";

    std::string hash = sha256(data);
    std::cout << "\n\n\ndata to be hashed: " << data << std::endl;
    std::cout << "hashed data to be signed: " << hash << "\n" << std::endl;
    Signature signature = wallet.sign(hash);

    std::cout << "Attempting to decode and verify signature: " << std::endl;
    int ret = wallet.verify(hash, signature, wallet.publicKey);

    if (ret == 1)
        std::cout << "Result: Verified!" << std::endl;
    else
        std::cout << "Result: Not verified: " << ret << std::endl;

    std::cout << "\n" << std::endl;

    return 0;
}