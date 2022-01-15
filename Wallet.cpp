#include "Wallet.hpp"
#include <cryptopp/pem.h>

using namespace CryptoPP;

Wallet::Wallet() {
    genKeyPair();
}

Wallet::~Wallet() {
}

void Wallet::genKeyPair() {
	// InvertibleRSAFunction is used directly only because the private key
	// won't actually be used to perform any cryptographic operation;
	// otherwise, an appropriate typedef'ed type from rsa.h would have been used.
	AutoSeededRandomPool rng;
	InvertibleRSAFunction privkey;
	privkey.Initialize(rng, 2048);

    Base64Encoder privkeysink(new StringSink(private_key));
	privkey.DEREncode(privkeysink);
	privkeysink.MessageEnd();
    //std::cout << "pr: " << private_key << std::endl;
	 
	// Suppose we want to store the public key separately,
	// possibly because we will be sending the public key to a third party.
	RSAFunction pubkey(privkey);
	
	Base64Encoder pubkeysink(new StringSink(public_key));
	pubkey.DEREncode(pubkeysink);
	pubkeysink.MessageEnd();
    //std::cout << "pu: "<< public_key << std::endl;
}

CryptoPP::SecByteBlock Wallet::sign(std::string strContents)
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
    
    // put decoded signature in variable and return it.
    // std::string sig;
    // Base64Encoder sigsink(new StringSink(sig));
	// sigsink.Put(signature, signature.size());
    // privateKey.DEREncode(sigsink);

    return signature;
}