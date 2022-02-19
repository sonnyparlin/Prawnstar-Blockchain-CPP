#include <iostream>
#include <cryptopp/cryptlib.h>
#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/oids.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <cryptopp/base32.h>
#include <cryptopp/zdeflate.h>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;
using namespace CryptoPP;

int main(int argc, char *argv[]) {
    AutoSeededRandomPool prng;
    ECDSA<ECP, SHA256>::PrivateKey privateKey;
    ECDSA<ECP, SHA256>::PublicKey publicKey;
    privateKey.Initialize( prng, ASN1::secp256k1());
    const Integer& x1 = privateKey.GetPrivateExponent();
    char *argument = argv[1];
    std::string privateName = argument;
    privateName += "Private.ec.der";
    FileSink fs(privateName.c_str(), true /*binary*/ );
    std::cout << privateName.c_str() << std::endl;
    privateKey.Save( fs );
    
    privateKey.MakePublicKey( publicKey );
    const ECP::Point& q = publicKey.GetPublicElement();
    std::string pubName = argv[1];
    pubName += "Public.ec.der";
    std::cout << pubName.c_str() << std::endl;
    FileSink fspublic(pubName.c_str(), true /*binary*/ );
    publicKey.Save( fspublic );
}