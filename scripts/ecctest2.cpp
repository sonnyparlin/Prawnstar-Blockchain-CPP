#include <iostream>
#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/oids.h>
#include <cryptopp/base64.h>

int main(int argc, const char* argv[]) 
{
    using namespace CryptoPP;
    
    AutoSeededRandomPool prng;

    ECDSA<ECP, SHA256>::PrivateKey k1;
    k1.Initialize( prng, ASN1::secp256r1() );
    ECDSA<ECP, SHA256>::Signer signer(k1);

    std::string message = "Do or do not. There is no try.";
    size_t siglen = signer.MaxSignatureLength();
    std::string signature(siglen, 0x00);

    siglen = signer.SignMessage( prng, (const byte*)&message[0], message.size(), (byte*)&signature[0] );
    signature.resize(siglen);

    ECDSA<ECP, SHA256>::PublicKey publicKey;
    k1.MakePublicKey(publicKey);
    ECDSA<ECP, SHA256>::Verifier verifier(publicKey);

    bool result = verifier.VerifyMessage( (const byte*)&message[0], message.size(), (const byte*)&signature[0], signature.size() );
    
    // Verification failure?
    if( !result ) {
        std::cout << "Failed to verify signature on message" << std::endl;
    } else {
        std::cout << "All good!" << std::endl;
    }
    
    return 0;
}