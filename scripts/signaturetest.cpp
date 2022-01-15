#include <cryptopp/cryptlib.h>
#include <cryptopp/rsa.h>
#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>

using namespace CryptoPP;
using namespace std;

int main() {
    AutoSeededRandomPool rng;

    InvertibleRSAFunction parameters;
    parameters.GenerateRandomWithKeySize(rng, 2048);

    RSA::PrivateKey privateKey(parameters);
    RSA::PublicKey publicKey(parameters);

    // Message
    string message = "Yoda said, Do or Do Not. There is no try.";

    // Signer object
    RSASSA_PKCS1v15_SHA_Signer signer(privateKey);

    // Create signature space
    size_t length = signer.MaxSignatureLength();
    SecByteBlock signature(length);

    // Sign message
    length = signer.SignMessage(rng, (const CryptoPP::byte*) message.c_str(),
        message.length(), signature);

    // Resize now we know the true size of the signature
    signature.resize(length);

    // Verifier object
    RSASSA_PKCS1v15_SHA_Verifier verifier(publicKey);

    // Verify
    bool result = verifier.VerifyMessage((const CryptoPP::byte*)message.c_str(),
        message.length(), signature, signature.size());

    // Result
    if(true == result) {
        cout << "Signature on message verified" << endl;
    } else {
        cout << "Message verification failed" << endl;
    }
}