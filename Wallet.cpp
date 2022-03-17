#include "Wallet.hpp"
#include "Block.hpp"
#include <nlohmann/json.hpp>

Wallet::Wallet(Node *node, bool usefile, const char *filename) {
    fromKey(filename);
    node->accountModel->addAccount(address, walletPublicKey, walletPrivateKey);
}

Wallet::Wallet(const char *_address, Node *node) {
    address = _address;
    walletPublicKey = node->accountModel->addressToPublicKey[address];
    walletPrivateKey = node->accountModel->addressToPrivateKey[address];
}

Wallet::Wallet(Node *node) {
    genKeyPair();
    node->accountModel->addAccount(address, walletPublicKey, walletPrivateKey);
}

Wallet::~Wallet(){
    // std::cout << "wallet destructor called " << std::endl;
}

void Wallet::fromKey(const char *file) {
    int PUBUFFSIZE=174;
    int n;

    std::string key;
    std::ifstream t(file);
    std::stringstream buffer;
    buffer << t.rdbuf();

    walletPrivateKey = buffer.str();    
    
    const char *mKey = walletPrivateKey.c_str();
    BIO* bo = BIO_new( BIO_s_mem() );
    BIO_write( bo, mKey,strlen(mKey));
    EVP_PKEY* pkey = nullptr;
    PEM_read_bio_PrivateKey( bo, &pkey, 0, 0 );
    BIO_free(bo);

    // PUBLIC KEY
    BIO *bpu = BIO_new(BIO_s_mem());
    char publicKeyString[PUBUFFSIZE];
    
    if (!PEM_write_bio_PUBKEY(bpu, pkey))
        std::cerr << "error generating public key" << std::endl;

    n=BIO_read(bpu, (void *)publicKeyString, sizeof(publicKeyString));
    if (n < 0)
        std::cerr << "Error generating keypair." << std::endl;
    
    walletPublicKey = publicKeyString;
    
    BIO_free(bpu);
    EVP_PKEY_free(pkey);

    address = "pv1" + generateAddress(walletPublicKey);
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
    
    walletPrivateKey = privateKeyString;
    BIO_free(bp);

    // PUBLIC KEY
    BIO *bpu = BIO_new(BIO_s_mem());
    char publicKeyString[PUBUFFSIZE];
    
    if (!PEM_write_bio_PUBKEY(bpu, pkey))
        std::cerr << "error generating public key" << std::endl;

    n=BIO_read(bpu, (void *)publicKeyString, sizeof(publicKeyString));
    if (n < 0)
        std::cerr << "Error generating keypair." << std::endl;
    
    walletPublicKey = publicKeyString;
    
    BIO_free(bpu);
    EVP_PKEY_free(pkey);

    address = "pv1" + generateAddress(walletPublicKey);
}

std::string Wallet::generateAddress(const std::string str) {

    // The public crypto address is a SHA1 
    // hex encoded string of the walletPublicKey.

    unsigned char hash[20];
    SHA1((unsigned char *)str.c_str(), str.length(), hash);
    std::stringstream ss;
    for(int i = 0; i < 20; i++) {ss << std::hex << (int)hash[i];}
    return ss.str();
}

utils::Signature Wallet::sign(std::string str)
{	
    /* str should be a sha256 hash */
    unsigned char *md = (unsigned char *)str.c_str();
   
    /* create private key from private key string */
    const char *mKey = walletPrivateKey.c_str();
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

    /* Determine buffer length */
    size_t mdlen = 32;
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

    utils::Signature mysig;
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
    // std::cout << mysig.hexsig << "\n" << std::endl;

    /* free memory */
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);

    /* return signature struct */
    return mysig;
}

/*!
This is where transactions originate. Right now there are no checks and balances to verify a user's identity. This will change with the implementation of a user interface.
*/
Transaction Wallet::createTransaction(std::string receiverAddress, double amount, std::string type) {
    Transaction transaction(address, receiverAddress, amount, type);

    // AUTHENTICATE HERE

    utils::Signature signature = sign(transaction.payload());
    transaction.senderPublicKey = walletPublicKey;
    transaction.sign(signature.hexsig);
    return transaction;
}

/*!
This is where blocks are generated for the entire blockchain. Since this is a proof of stake blockchain, transactions will always be the created by a forger determined by the prof of stake algorithm. Called from Blockchain::createBlock() inside of Node.cpp.
*/
Block Wallet::createBlock(vector<Transaction> transactions, std::string lastHash, unsigned long long blockCount) {
    Block block(transactions, lastHash, blockCount);
    block.hash = utils::hash(block.payload());
    block._id = block.hash;
    block.forgerAddress = address;
    utils::Signature signature = sign(block.payload());
    block.sign(signature.hexsig);
    return block;
}

std::string Wallet::toJson() {
    nlohmann::json j;

    j["wallet"]["address"] = address;
    j["wallet"]["publickey"] = walletPublicKey;
    j["wallet"]["privatekey"] = walletPrivateKey;

    return j.dump();
}