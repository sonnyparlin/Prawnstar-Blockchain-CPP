#include "Wallet.hpp"
#include "Block.hpp"
//#include <cryptopp/pem.h>

using namespace CryptoPP;

Wallet::Wallet() {
    genKeyPair();
}

Wallet::~Wallet() {
}

void Wallet::genKeyPair() {
	// Generate keypair
	AutoSeededRandomPool rng;
	InvertibleRSAFunction params;
	params.Initialize(rng, 2048);

    // Save private key to Wallet::private_key
    Base64Encoder privkeysink(new StringSink(private_key));
	params.DEREncode(privkeysink);
	privkeysink.MessageEnd();
	 
	// Save public key to Wallet::public_key
	RSAFunction pubkey(params);
	Base64Encoder pubkeysink(new StringSink(public_key));
	pubkey.DEREncode(pubkeysink);
	pubkeysink.MessageEnd();
    address = "pv1" + generateAddress();
}

std::string Wallet::generateAddress() {

    // The public crypto address is a SHA1 
    // hex encoded string of the public_key.
    // It will be stored in the AccountModel
    // as a key in a C++ map. 
    // map["address"] = instance_of_accountModel
    // giving us the public key, which we can then 
    // re-hash  and compare the resulting string to 
    // the address as a way of verifying authenticity.

    using namespace CryptoPP;
    std::string hexdigest{""};
    SHA1 hash;
    StringSource s(public_key, true, 
        new HashFilter(hash, new HexEncoder(new StringSink(hexdigest), false)));

    return hexdigest;
}

std::string Wallet::sign(std::string strContents)
{	
    // Hash the data to be signed.
    std::string hashedData = utils::hash(strContents);
    //std::cout << hashedData << std::endl;

	//Read private key
	CryptoPP::ByteQueue bytes;
	StringSource str(private_key, true, new Base64Decoder);
	str.TransferTo(bytes);
	bytes.MessageEnd();
	RSA::PrivateKey privateKey;
	privateKey.Load(bytes);

	//Sign message
    AutoSeededRandomPool rng;
	RSASSA_PKCS1v15_SHA_Signer signer(privateKey);

    size_t length = signer.MaxSignatureLength();
    SecByteBlock signature(length);

	length = signer.SignMessage(rng, (const CryptoPP::byte*) hashedData.c_str(), hashedData.length(), signature);
    signature.resize(length);

    std::string strsig;
    std::string encoded;
    strsig.resize(signature.size());  // Make room for elements
    std::memcpy(&strsig[0], &signature[0], strsig.size());

    HexEncoder encoder;
    encoder.Put((CryptoPP::byte*)&strsig[0], strsig.size());
    encoder.MessageEnd();

    word64 size = encoder.MaxRetrievable();
    if(size)
    {
        encoded.resize(size);		
        encoder.Get((CryptoPP::byte*)&encoded[0], encoded.size());
    }

    return encoded;
}

Transaction Wallet::create_transaction(std::string receiver, double amount, std::string type) {
    Transaction transaction(address, receiver, amount, type);
    std::string signature = sign(transaction.payload());
    transaction.sign(signature);
    return transaction;
}

Block Wallet::create_block(vector<Transaction> transactions, std::string last_hash, unsigned long long block_count) {
    Block block(transactions, last_hash, block_count);
    block.forger_address = address;
    std::string signature = sign(block.payload());
    block.sign(signature);
    block.hash = utils::hash(block.payload());
    return block;
}