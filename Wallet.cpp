#include "Wallet.hpp"

/*!
 *
 * @param node
 * @param filename
 *
 * Wallet constructor for wallets on disk.
 */
Wallet::Wallet(Node *node, const char *filename) {
    fromKey(filename);
    node->blockchain->accountModel.addAccount(
            address,
            walletPublicKey,
            walletPrivateKey);
}

/*!
 *
 * @param _address
 * @param node
 *
 * Wallet constructor that finds wallet by address.
 */
Wallet::Wallet(const char *_address, Node *node) {
    address = _address;
    walletPublicKey = node->blockchain->accountModel.addressToPublicKey[address];
    walletPrivateKey = node->blockchain->accountModel.addressToPrivateKey[address];
}

/*!
 *
 * @param node
 *
 * Generate a new wallet (elliptical curve key pair)
 */
Wallet::Wallet(Node *node) {
    genKeyPair();
    node->blockchain->accountModel.addAccount(address, walletPublicKey, walletPrivateKey);
}

Wallet::Wallet()=default;
Wallet::~Wallet()=default;

/*!
 *
 * @param file
 *
 * Method for opening wallet from file
 */
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
    BIO_write( bo, mKey,(int)strlen(mKey));
    EVP_PKEY* pkey = nullptr;
    PEM_read_bio_PrivateKey( bo, &pkey, nullptr, nullptr );
    BIO_free(bo);

    // PUBLIC KEY
    BIO *bpu = BIO_new(BIO_s_mem());
    char publicKeyString[PUBUFFSIZE];
    
    if (!PEM_write_bio_PUBKEY(bpu, pkey)) {
        std::cerr << "error generating public key: " << errno << std::endl;
        return;
    }

    n=BIO_read(bpu, (void *)publicKeyString, (int)sizeof(publicKeyString));
    if (n < 0) {
        std::cerr << "Error generating keypair: " << errno << std::endl;
        return;
    }

    walletPublicKey = (char *)publicKeyString;

    BIO_free(bpu);
    EVP_PKEY_free(pkey);

    address = "pv1" + generateAddress(walletPublicKey);
}

/*!
 * Method for generating an elliptic curve key pair using openssl.
 */
void Wallet::genKeyPair() {
    const char *curve = "secp256k1";
    int PRBUFFSIZE=255;
    int PUBUFFSIZE=174;
    int n;

    /* Generate the key pair */
    EVP_PKEY *pkey = EVP_EC_gen(curve);
    if (pkey == nullptr) {
        std::cerr << "Error generating the ECC key." << std::endl;
        return;
    }

    // PRIVATE KEY
    char privateKeyString[PRBUFFSIZE];
    BIO *bp = BIO_new(BIO_s_mem());
    const EVP_CIPHER *cipher = EVP_get_cipherbyname(curve);

    if (!PEM_write_bio_PrivateKey(bp, pkey, cipher, nullptr, 0, nullptr, nullptr))
        std::cerr << "Error generating keypair." << std::endl;

    n=BIO_read(bp, (void *)privateKeyString, (int)sizeof(privateKeyString));
    if (n < 0)
        std::cerr << "Error generating keypair." << std::endl;
    
    walletPrivateKey = (char *)privateKeyString;
    BIO_free(bp);

    // PUBLIC KEY
    BIO *bpu = BIO_new(BIO_s_mem());
    char publicKeyString[PUBUFFSIZE];
    
    if (!PEM_write_bio_PUBKEY(bpu, pkey))
        std::cerr << "!error generating public key" << std::endl;

    n=BIO_read(bpu, (void *)publicKeyString, (int)sizeof(publicKeyString));
    if (n < 0)
        std::cerr << "Error generating keypair." << std::endl;
    
    walletPublicKey = (char *)publicKeyString;
    
    BIO_free(bpu);
    EVP_PKEY_free(pkey);

    address = "pv1" + generateAddress(walletPublicKey);
}

/*!
 *
 * @param str
 * @return
 *
 * The public crypto address is a SHA1
 * hex encoded string of the walletPublicKey.
 */
std::string Wallet::generateAddress(const std::string &str) {

    // The public crypto address is a SHA1
    // hex encoded string of the walletPublicKey.

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1((unsigned char *)str.c_str(), str.length(), hash);
    std::stringstream ss;
    for(unsigned char i : hash) {ss << std::hex << (int)i;}
    return ss.str();
}

/*!
 *
 * @param str
 * @return utils::Signature
 *
 * Creates a signature by signing the passed in data (str).
 */
utils::Signature Wallet::sign(const std::string &str) const
{
    /* str should be a sha256 hash */
    auto *md = (unsigned char *)str.c_str();
   
    /* create private key from private key string */
    const char *mKey = walletPrivateKey.c_str();
    BIO* bo = BIO_new( BIO_s_mem() );
    BIO_write( bo, mKey,(int)strlen(mKey));
    EVP_PKEY* pkey = nullptr;
    PEM_read_bio_PrivateKey( bo, &pkey, nullptr, nullptr );
    BIO_free(bo);

    /* initialize our signature context object */
    EVP_PKEY_CTX *ctx;
    ctx = EVP_PKEY_CTX_new(pkey, nullptr /* no engine */);
    if (!ctx)
        std::cerr << "error creating ctx" << std::endl;


    if (EVP_PKEY_sign_init(ctx) <= 0)
        std::cerr << "error initializing sig ctx" << std::endl;

    /* set the context digest type to sha256 */
    if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0)
        std::cerr << "error setting signature md" << std::endl;

    /* Determine buffer length */
    size_t mdlen = 32; // SHA256_DIGEST_LENGTH
    size_t siglen {0};
    if (EVP_PKEY_sign(ctx, nullptr, &siglen, md, mdlen) <= 0)
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
    // mysig.hex = OPENSSL_buf2hexstr(sig, siglen);

    /* the not so easy way */
    char st[256];
    size_t strlen;
    OPENSSL_buf2hexstr_ex(st, 256, &strlen,
                           sig, siglen, '\0');

    OPENSSL_free(sig);
    
    /* assign the result to signature structure */
    mysig.hex = st;
    mysig.len = siglen;
    // std::cout << mysig.hex << "\n" << std::endl;

    /* free memory */
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);

    /* return signature struct */
    return mysig;
}

/*!
 *
 * @param receiverAddress
 * @param amount
 * @param type
 * @return Transaction
 *
 * This is where transactions originate. Right now there are no checks and balances
 * to verify a user's identity. This will change with the implementation of a user
 * interface.
 */
Transaction Wallet::createTransaction(std::string receiverAddress, double amount, std::string type) const {
    Transaction transaction(address,
                            std::move(receiverAddress),
                            amount,
                            std::move(type));

    // AUTHENTICATE HERE

    try {
        auto signature = sign(utils::hash(transaction.payload()));
        // std::cout << "sig size: " << signature.len << std::endl;
        transaction.senderPublicKey = walletPublicKey;
        transaction.sign(signature);
    } catch(std::exception& e) {
        std::cerr << "Error signing transaction: " << e.what() << std::endl;
    }
    return transaction;
}

/*!
 *
 * @param transactions
 * @param lastHash
 * @param blockCount
 * @return Block
 *
 * This is where blocks are generated for the entire blockchain. Since this is a
 * proof of stake blockchain, transactions will always be the created by a forger
 * determined by the prof of stake algorithm. Called from Blockchain::createBlock()
 * inside of Node.cpp.
 */
Block Wallet::createBlock(std::vector<Transaction> transactions,
                          std::string lastHash,
                          long blockCount) const {
    Block block(std::move(transactions), std::move(lastHash), blockCount);
    block.hash = utils::hash(block.payload());
    block.forgerAddress = address;
    try {
        auto signature = sign(utils::hash(block.payload()));
        block.sign(signature);
    } catch(std::exception& e) {
        std::cerr << "Error signing transaction: " << e.what() << std::endl;
    }

    return block;
}

/*!
 *
 * @return std::string
 *
 * Stringified json representation of wallet.
 */
std::string Wallet::toJson() {
    nlohmann::json j;

    j["wallet"]["address"] = address;
    j["wallet"]["publickey"] = walletPublicKey;
    j["wallet"]["privatekey"] = walletPrivateKey;

    return j.dump();
}