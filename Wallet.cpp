#include "Wallet.hpp"

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

std::string Wallet::sign(std::string strContents)
{	
	//Read private key
	CryptoPP::ByteQueue bytes;
	StringSource str(private_key, true, new Base64Decoder);
	str.TransferTo(bytes);
	bytes.MessageEnd();
	RSA::PrivateKey privateKey;
	privateKey.Load(bytes);

	//Sign message
    AutoSeededRandomPool rng;
	RSASSA_PKCS1v15_SHA_Signer privkey(privateKey);
	SecByteBlock sbbSignature(privkey.SignatureLength());
	privkey.SignMessage(
		rng,
		(CryptoPP::byte const*) strContents.data(),
		strContents.size(),
		sbbSignature);
    
    // put decoded signature in variable and return it.
    std::string signature;
    Base64Encoder sigsink(new StringSink(signature));
	sigsink.Put(sbbSignature, sbbSignature.size());
    privateKey.DEREncode(sigsink);

    return signature;
}
