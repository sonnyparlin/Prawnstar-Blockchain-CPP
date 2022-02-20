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
    std::string address;

    Wallet();
    ~Wallet();

    void genKeyPair();
    const char * sign(std::string);
    bool verify(std::string, const char *, std::string);
};

Wallet::Wallet() {
    genKeyPair();
}
Wallet::~Wallet(){}

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

    const EVP_CIPHER *cipher = EVP_get_cipherbyname("secp256k1");
    EVP_PKEY *pkey = EVP_EC_gen("secp256k1");
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

const char * Wallet::sign(std::string str) {
    EVP_PKEY_CTX *ctx;
    /* md is a SHA-256 digest in this example. */
    unsigned char *sig;
    size_t siglen;
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

    EVP_PKEY_free(pkey);

    // had to trick OPENSSL_buf2hexstr_ex into creating 64 byte long strings
    // by changing the out buffer to out[66] and adding +24 to the sizeof(sig).
    // This seems to create consistent 64 byte hex strings. 
    static char out[66];
    size_t hexlen;
    //return OPENSSL_buf2hexstr(sig, sizeof(sig));
    OPENSSL_buf2hexstr_ex(out, sizeof(out), &hexlen, sig, sizeof(sig)+24, '\0');
    return out;
}

bool Wallet::verify(std::string str, const char *signature, std::string publicKeyString) {
    EVP_PKEY_CTX *ctx;
    size_t mdlen = 32;
    size_t siglen = sizeof(signature);
    unsigned char *sig = (unsigned char *)signature;
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
    if (ret) {
        return true;
    } else {
        return false;
    }
}

int main() {
    Wallet wallet;
    std::cout << wallet.publicKey << std::endl;
    std::cout << wallet.privateKey << std::endl;
    std::cout << wallet.address << std::endl;
    std::cout << "=======================================" << std::endl;

    const std::string data = "message to sign";

    std::string hash = sha256(data);
    std::cout << "signing hashed data with hash: " << std::endl << hash << "\n" << std::endl;
    const char * signature = wallet.sign(hash);

    std::cout << "using signature: " << std::endl << signature << "\n" << std::endl;
    bool ret = wallet.verify(hash, signature, wallet.publicKey);

    if (ret)
        std::cout << "Result: Verified!" << std::endl;
    else
        std::cout << "Result: Not verified." << hash << std::endl;

    return 0;
}