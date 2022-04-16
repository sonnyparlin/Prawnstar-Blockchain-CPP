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
    std::string hexsig;
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
    const char *curve = "secp256k1";
    int PRBUFFSIZE=255;
    int PUBUFFSIZE=174;
    int n;

    /* Generate the key pair */
    EVP_PKEY *pkey = EVP_EC_gen(curve);
    if (pkey == NULL) {
        std::cerr << "Error generating the ECC key." << std::endl;
        return;
    }

    // PRIVATE KEY
    char privateKeyString[PRBUFFSIZE];
    BIO *bp = BIO_new(BIO_s_mem());
    const EVP_CIPHER *cipher = EVP_get_cipherbyname(curve);

    if (!PEM_write_bio_PrivateKey(bp, pkey, cipher, NULL, 0, 0, NULL))
        std::cerr << "Error generating keypair." << std::endl;

    n=BIO_read(bp, (void *)privateKeyString, sizeof(privateKeyString));
    if (n < 0)
        std::cerr << "Error generating keypair." << std::endl;
    
    privateKey = privateKeyString;
    BIO_free(bp);

    // PUBLIC KEY
    BIO *bpu = BIO_new(BIO_s_mem());
    char publicKeyString[PUBUFFSIZE];
    
    if (!PEM_write_bio_PUBKEY(bpu, pkey))
        std::cerr << "error generating public key" << std::endl;

    n=BIO_read(bpu, (void *)publicKeyString, sizeof(publicKeyString));
    if (n < 0)
        std::cerr << "Error generating keypair." << std::endl;
    
    publicKey = publicKeyString;
    
    BIO_free(bpu);
    EVP_PKEY_free(pkey);

    address = "pv1" + sha1(publicKey);
}

struct Signature Wallet::sign(std::string str) {
    /* str should be a sha256 hash */
    unsigned char *md = (unsigned char *)str.c_str();
   
    /* create private key from private key string */
    const char *mKey = privateKey.c_str();
    BIO* bo = BIO_new( BIO_s_mem() );
    BIO_write( bo, mKey,strlen(mKey));
    EVP_PKEY* pkey = 0;
    PEM_read_bio_PrivateKey( bo, &pkey, 0, 0 );
    BIO_free(bo);

    /* initialize our signature context object */
    EVP_PKEY_CTX *ctx;
    ctx = EVP_PKEY_CTX_new(pkey, NULL /* no engine */);
    if (!ctx)
        std::cerr << "error creating ctx" << std::endl;

    if (EVP_PKEY_sign_init(ctx) <= 0)
        std::cerr << "error initializing sig ctx" << std::endl;

    /* set the context digest type to sha256 */
    if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0)
        std::cerr << "error setting signature md" << std::endl;

    /* 
    mdlen is always 32, I don't know why, 
    see https://www.openssl.org/docs/manmaster/man3/EVP_PKEY_sign.html
    */
    size_t mdlen = 32;

    /* Determine buffer length */
    size_t siglen {0};
    if (EVP_PKEY_sign(ctx, NULL, &siglen, md, mdlen) <= 0)
        std::cerr << "error determining buffer length" << std::endl;

    /* allocate memory for the signature */
    unsigned char *sig;
    sig = (unsigned char *)OPENSSL_malloc(siglen);

    if (!sig)
        std::cerr << "malloc failure" << std::endl;

    if (EVP_PKEY_sign(ctx, sig, &siglen, md, mdlen) <= 0) {
        std::cerr << "error creating signature" << std::endl;
    }

    Signature mysig;
    /* the easy way to translate your signature to a hex string */
    // mysig.hexsig = OPENSSL_buf2hexstr(sig, siglen);

    /* the not so easy way */
    char st[256];
    size_t strlen;
    OPENSSL_buf2hexstr_ex(st, 256, &strlen,
                           sig, siglen, '\0');
    
    /* assign the result to signature structure */
    mysig.hexsig = st;
    mysig._size = siglen;
    std::cout << "Signature converted to hex: " << std::endl;
    std::cout << mysig.hexsig << "\n" << std::endl;

    /* free memory */
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);

    /* return signature struct */
    return mysig;
}

int Wallet::verify(std::string str, Signature signature, std::string publicKeyString) {

    /* the easy way to translate your hex string back into your buffer */
    // long len;
    // unsigned char *sig = OPENSSL_hexstr2buf(signature.hexsig, &len);

    /* the not so easy way */
    unsigned char buf[256];
    const char *st = signature.hexsig.c_str();
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
    if (!ctx)
        std::cerr << "ctx error" << std::endl;

    /* initialize the context */
    if (EVP_PKEY_verify_init(ctx) <= 0)
        std::cerr << "ctx verify error" << std::endl;

    /* set the message digest type to sha256 */
    if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0)
        std::cerr << "verify signature error" << std::endl; 

    /* Perform operation */
    unsigned char *md = (unsigned char *)str.c_str();
    size_t mdlen = 32;
    size_t siglen = signature._size;
    int ret = EVP_PKEY_verify(ctx, buf, siglen, md, mdlen);

    /* free memory and return the result */
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);
    return ret;
}

int main() {
    Wallet wallet;
    std::cout << wallet.publicKey << std::endl;
    std::cout << wallet.privateKey << std::endl;
    std::cout << wallet.address << std::endl;
    std::cout << "=======================================" << std::endl;

    const std::string data = "This is my wonderful message.";

    std::string hash = sha256(data);
    std::cout << "\n\n\ndata to be hashed: " << data << std::endl;
    std::cout << "hashed data to be signed: " << hash << "\n" << std::endl;
    Signature signature = wallet.sign(hash);

    std::cout << "Attempting to decode and verify signature: " << std::endl;
    signature.hexsig = "fart"; // evil stuff
    int ret = wallet.verify(hash, signature, wallet.publicKey);

    if (ret == 1)
        std::cout << "Result: Verified!" << std::endl;
    else
        std::cout << "Result: Not verified: " << ret << std::endl;

    std::cout << "\n" << std::endl;

    return 0;
}
