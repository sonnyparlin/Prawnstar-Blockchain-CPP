#include "utils.hpp"

namespace utils {

    std::string hash(std::string msg)
    {   
        using namespace CryptoPP;
        std::string digest{""};

        SHA256 hash;
        StringSource s(msg, true, 
            new HashFilter(hash, new HexEncoder(new StringSink(digest))));

        return digest; 
    }

    using namespace CryptoPP;
    bool verify_signature(std::string dataStr, CryptoPP::SecByteBlock signature, std::string publicKey)
    {
        // Hash the data to be signed.
        std::string hashedData = utils::hash(dataStr);

        //Read public key
        CryptoPP::ByteQueue bytes;
	    StringSource str(publicKey, true, new Base64Decoder);
        str.TransferTo(bytes);
        bytes.MessageEnd();
        RSA::PublicKey pubKey;
        pubKey.Load(bytes);

        // Verifier object
        RSASSA_PKCS1v15_SHA_Verifier verifier(pubKey);

        // Verify
        bool result = verifier.VerifyMessage((const CryptoPP::byte*)hashedData.c_str(), hashedData.length(), signature, signature.size());

        // Result
        if(true == result) {
            std::cout << "Signature on message verified" << std::endl;
            return true;
        } else {
            std::cout << "Message verification failed" << std::endl;
            return false;
        }
    }
}