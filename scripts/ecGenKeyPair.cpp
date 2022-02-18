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

int main() {
    AutoSeededRandomPool prng;
    ECDSA<ECP, SHA256>::PrivateKey privateKey;
    ECDSA<ECP, SHA256>::PublicKey publicKey;
    privateKey.Initialize( prng, ASN1::secp256r1());
    const Integer& x1 = privateKey.GetPrivateExponent();
    FileSink fs( "private.ec.der", true /*binary*/ );
    privateKey.Save( fs );
    
    privateKey.MakePublicKey( publicKey );
    const ECP::Point& q = publicKey.GetPublicElement();
    FileSink fspublic( "public.ec.der", true /*binary*/ );
    publicKey.Save( fspublic );
}