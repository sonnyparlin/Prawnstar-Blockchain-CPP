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
            //std::cout << "All good!" << std::endl;
        }

        return verifyResult;
    }

    unsigned char random_char() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        return static_cast<unsigned char>(dis(gen));
    }

    char asciitolower(char in) {
        if (in <= 'Z' && in >= 'A')
            return in - ('Z' - 'z');
        return in;
    }

    std::string uuid_gen() {
        boost::uuids::uuid uuid = boost::uuids::random_generator()();
        return to_string(uuid);
    }

    std::string gen_random_str(const int len) {
        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
        std::string tmp_s;
        tmp_s.reserve(len);

        srand (time(NULL));
        for (int i = 0; i < len; ++i) {
            tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
        }
        
        //std::cout << tmp_s << std::endl;
        return tmp_s;
    }

    static const long hextable[] = { 
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1, 0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,10,11,12,13,14,15,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
    };

    /** 
     * @brief convert a hexidecimal string to a signed long
     * will not produce or process negative numbers except 
     * to signal error.
     * 
     * @param hex without decoration, case insensitive. 
     * 
     * @return -1 on error, or result (max (sizeof(long)*8)-1 bits)
     */
    int hexToInt(const char *hex) {
        int ret = 0; 
        while (*hex && ret >= 0) {
            ret = (ret << 4) | hextable[(unsigned char)*hex++];
        }
        if (ret < 0) ret *= -1;
        return ret; 
    }

    int ABS(int z) {
        if (z < 0) z *= -1;
        return z;
    }
}