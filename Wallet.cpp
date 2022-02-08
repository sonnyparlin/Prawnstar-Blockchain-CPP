#include "Wallet.hpp"
#include "Block.hpp"
//#include <cryptopp/pem.h>
#include "AccountModel.hpp"

using namespace CryptoPP;

Wallet::Wallet(const bool useFile) {
    genKeyPair(true);
}

Wallet::Wallet() {
    genKeyPair(false);
}

Wallet::Wallet(std::string address, AccountModel accountModel) {
    loadWalletFromAddress(address, accountModel);
}

Wallet::~Wallet() {
}

void Wallet::loadWalletFromAddress(const std::string& addr, AccountModel accountModel) {
    // Need to get this information from the AccountModel
    // Maybe when someone creates an account we add it to the
    // AccountModel immediately. Right now we generate a new wallet 
    // and insert the address but the publickey and privatekey are
    // generated on the fly, we want to load them instead.
    //genKeyPair(false);
    //address = addr;
    address = addr;
    walletPublicKey = accountModel.addressToPublicKey[addr];
    walletPrivateKey = accountModel.addressToPrivateKey[addr];
    //std::cout << "walletPublicKey: " << walletPublicKey << std::endl;
}

void Wallet::genKeyPair(const bool useFile) {
	// Generate keypair
    ECDSA<ECP, SHA256>::PrivateKey privateKey;
    ECDSA<ECP, SHA256>::PublicKey publicKey;
    AutoSeededRandomPool prng;

    if (useFile) {
        FileSource fs( "private.ec.der", true /*binary*/ );
        privateKey.Load( fs );
        bool result = privateKey.Validate( prng, 3 );
        if (!result)
            std::cout << "***Private key invalid for signing***" << std::endl; // throw exception
        
        const Integer& x1 = privateKey.GetPrivateExponent();
        std::stringstream key_p;
        key_p << std::hex << x1 << endl;
        walletPrivateKey = key_p.str();
    } else {
        privateKey.Initialize( prng, ASN1::secp256r1());
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

    //std::cout << "public: " + walletPublicKey << std::endl;
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
    privateKey.Initialize(ASN1::secp256r1(), x);

    // FileSource fs( "private.ec.der", true /*binary*/ );
    // privateKey.Load( fs );
    // bool result = privateKey.Validate( prng, 3 );
    // if (!result)
    //     std::cout << "Private key invalud for signing" << std::endl; // throw exception

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