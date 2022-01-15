#include <iostream>
#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/oids.h>
#include <iomanip>

using namespace std;
using namespace CryptoPP;

int main() {
    AutoSeededRandomPool prng;
    ECDSA<ECP, SHA256>::PrivateKey privateKey;
    ECDSA<ECP, SHA256>::PublicKey publicKey;
    privateKey.Initialize( prng, ASN1::secp256r1());

    const Integer& x1 = privateKey.GetPrivateExponent();
    cout << "priv:  " << std::hex << x1 << endl;
    
    privateKey.MakePublicKey( publicKey );
    
    const ECP::Point& q = publicKey.GetPublicElement();
    const Integer& qx = q.x;
    const Integer& qy = q.y;
    cout << "pub x: " << std::hex << qx << endl;
    cout << "pub y: " << std::hex << qy << endl;
}