#include "Wallet.hpp"
#include "Block.hpp"
#include <nlohmann/json.hpp>

using namespace CryptoPP;

Wallet::Wallet(bool forgerWallet, const char *filename, Node *node) {
    this->node = node;
    genKeyPair(filename);
    node->accountModel->addAccount(this->address, this->walletPublicKey, this->walletPrivateKey);
}

Wallet::Wallet(const char *filename, Node *node) {
    const char *p;
    p = strstr(filename, ".der");
    if (p) {
        genKeyPair(filename);
        node->accountModel->addAccount(this->address, this->walletPublicKey, this->walletPrivateKey);
    } else {
        address = filename;
        walletPublicKey = node->accountModel->addressToPublicKey[address];
        walletPrivateKey = node->accountModel->addressToPrivateKey[address];
    }
}

Wallet::Wallet(Node *node) {
    const char *filename = "none";
    genKeyPair(filename);
    node->accountModel->addAccount(this->address, this->walletPublicKey, this->walletPrivateKey);
}

Wallet::~Wallet(){}

void Wallet::genKeyPair(const char *filename) {
	// Generate keypair
    ECDSA<ECP, SHA256>::PrivateKey privateKey;
    ECDSA<ECP, SHA256>::PublicKey publicKey;
    AutoSeededRandomPool prng;

    if (strcmp(filename, "none") != 0) {
        FileSource fs( filename, true /*binary*/ );
        privateKey.Load( fs );
        bool result = privateKey.Validate( prng, 3 );
        if (!result)
            std::cout << "***Private key invalid for signing***" << std::endl; // throw exception
        
        const Integer& x1 = privateKey.GetPrivateExponent();
        std::stringstream key_p;
        key_p << std::hex << x1 << endl;
        walletPrivateKey = key_p.str();
    } else {
        privateKey.Initialize( prng, ASN1::secp256k1());
        const Integer& x1 = privateKey.GetPrivateExponent();
        std::stringstream key_p;
        key_p << std::hex << x1 << endl;
        walletPrivateKey = key_p.str();
    } 
    
    privateKey.MakePublicKey( publicKey );
    const ECP::Point& q = publicKey.GetPublicElement();
    const Integer& qx = q.x;
    const Integer& qy = q.y;

    std::stringstream key_x;
    std::stringstream key_y;
    key_x << std::hex << qx;
    key_y << std::hex << qy;
    const std::string walletPublicKey_x = key_x.str();
    const std::string walletPublicKey_y = key_y.str();
    walletPublicKey = walletPublicKey_x + walletPublicKey_y;

    address = "pv1" + generateAddress();
}

std::string Wallet::generateAddress() {

    // The public crypto address is a SHA1 
    // hex encoded string of the walletPublicKey.
    // It will be stored in the AccountModel
    // as a key in a C++ map. 
    // map["address"] = instance_of_accountModel
    // giving us the public key, which we can then 
    // re-hash  and compare the resulting string to 
    // the address as a way of verifying authenticity.

    using namespace CryptoPP;
    std::string hexdigest{""};
    SHA1 hash;
    StringSource s(walletPublicKey, true, 
        new HashFilter(hash, new HexEncoder(new StringSink(hexdigest), false)));

    return hexdigest;
}

std::string Wallet::sign(std::string strContents)
{	
    // Hash the data to be signed.
    std::string message = utils::hash(strContents);
    
    AutoSeededRandomPool prng;
    ECDSA<ECP, SHA256>::PrivateKey privateKey;
    HexDecoder decoder;
    decoder.Put((CryptoPP::byte*)&walletPrivateKey[0], walletPrivateKey.size());
    decoder.MessageEnd();
    Integer x;
    x.Decode(decoder, decoder.MaxRetrievable());
    privateKey.Initialize(ASN1::secp256k1(), x);

    bool result = privateKey.Validate( prng, 3 );
    if (!result)
        std::cout << "***Private key invalid for signing***" << std::endl; // throw exception

    ECDSA<ECP, SHA256>::Signer signer(privateKey);
    size_t siglen = signer.MaxSignatureLength();
    std::string signature(siglen, 0x00);
    siglen = signer.SignMessage(prng, (const CryptoPP::byte*)&message[0], message.size(), (CryptoPP::byte*)&signature[0]);
    signature.resize(siglen);

    string encoded;
    HexEncoder encoder;
    encoder.Put((CryptoPP::byte*)&signature[0], signature.size());
    encoder.MessageEnd();

    word64 size = encoder.MaxRetrievable();
    if(size)
    {
        encoded.resize(size);		
        encoder.Get((CryptoPP::byte*)&encoded[0], encoded.size());
    }

    return encoded;
}

Transaction Wallet::createTransaction(std::string receiverAddress, double amount, std::string type) {
    Transaction transaction(address, receiverAddress, amount, type);
    std::string signature = sign(transaction.payload());
    transaction.senderPublicKey = walletPublicKey;
    transaction.sign(signature);
    return transaction;
}

Block Wallet::createBlock(vector<Transaction> transactions, std::string lastHash, unsigned long long blockCount) {
    Block block(transactions, lastHash, blockCount);
    block.hash = utils::hash(block.payload());
    block.forgerAddress = address;
    std::string signature = sign(block.payload());
    block.sign(signature);
    return block;
}

std::string Wallet::toJson() {
    nlohmann::json j;

    j["wallet"]["address"] = address;
    j["wallet"]["publickey"] = walletPublicKey;
    j["wallet"]["privatekey"] = walletPrivateKey;

    return j.dump();
}