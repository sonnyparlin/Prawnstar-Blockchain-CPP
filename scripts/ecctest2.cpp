#include <iostream>
#include <cryptopp/cryptlib.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/oids.h>
#include <cryptopp/base64.h>
#include <cryptopp/sha.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/base32.h>
#include <cryptopp/zdeflate.h>
#include <iostream>
#include <sstream>
#include <random>

using namespace CryptoPP;
std::string walletPrivateKey;
std::string walletPublicKey;

std::string hash(std::string msg)
{   
    using namespace CryptoPP;
    std::string digest{""};

    SHA256 hash;
    StringSource s(msg, true, 
        new HashFilter(hash, new HexEncoder(new StringSink(digest))));

    return digest; 
}

void genKeyPair(const bool useFile) {
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
        key_p << std::hex << x1 << std::endl;
        walletPrivateKey = key_p.str();
    } else {
        privateKey.Initialize( prng, ASN1::secp256r1());
        const Integer& x1 = privateKey.GetPrivateExponent();
        std::stringstream key_p;
        key_p << std::hex << x1 << std::endl;
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
}

bool verifySignature(std::string message, std::string encoded_signature, std::string walletPublicKey) {

    // Hash the data to be signed.
    std::string hashedData = hash(message);

    HexDecoder publicKeyDecoder;
    publicKeyDecoder.Put((CryptoPP::byte*)&walletPublicKey[0], walletPublicKey.size());
    publicKeyDecoder.MessageEnd();

    ECP::Point q;
    size_t len = publicKeyDecoder.MaxRetrievable();
    q.identity = false;
    q.x.Decode(publicKeyDecoder, len/2);
    q.y.Decode(publicKeyDecoder, len/2);

    ECDSA<ECP, SHA256>::PublicKey publicKey;
    publicKey.Initialize( ASN1::secp256r1(), q );
    
    // Decode hex message (the block in a json string format)
    HexDecoder message_decoder;
    message_decoder.Put((CryptoPP::byte*)&encoded_signature[0], encoded_signature.size() );
    message_decoder.MessageEnd();
    
    // Set up a decoded variable to copy the bytes into
    std::string decoded_signature;
    word64 size = message_decoder.MaxRetrievable();
    if(size && size <= SIZE_MAX) {
        // set the size of the string to match the incoming bytes
        decoded_signature.resize(size);
        // Create a byte string by copying the incoming bytes
        message_decoder.Get((CryptoPP::byte*)&decoded_signature[0], decoded_signature.size());
    }

    // Set up public key as verifier
    ECDSA<ECP, SHA256>::Verifier verifier(publicKey);
    // Now that we've decoded our public key and our message, 
    // let's verify the message.
    bool verifyResult = verifier.VerifyMessage( 
        (const CryptoPP::byte*)&hashedData[0], 
        hashedData.size(), 
        (const CryptoPP::byte*)&decoded_signature[0],
        decoded_signature.size() 
    );
    
    // Verification failure?
    if( !verifyResult ) {
        std::cout << "Failed to verify signature on message" << std::endl;
    } else {
        std::cout << "It worked! All good!" << std::endl;
    }

    return verifyResult;
}

std::string sign(std::string strContents)
{	
    // Hash the data to be signed.
    std::string message = hash(strContents);
    
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

    std::string encoded;
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

int main(int argc, const char* argv[]) 
{
    using namespace CryptoPP;
    
    std::string message = "This is my message to verify";
    genKeyPair(false);
    std::cout << walletPrivateKey << std::endl;
    std::cout << walletPublicKey << std::endl;
    std::string encodedSignature = sign(message);
    verifySignature(message, encodedSignature, walletPublicKey);
    
    return 0;
}