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
    bool verify_signature(std::string dataStr, std::string encodedSignature, std::string publicKey)
    {
        // Hash the data to be signed.
        std::string hashedData = utils::hash(dataStr);
        //std::cout << hashedData << std::endl;

        //Read public key
        CryptoPP::ByteQueue bytes;
	    StringSource str(publicKey, true, new Base64Decoder);
        str.TransferTo(bytes);
        bytes.MessageEnd();
        RSA::PublicKey pubKey;
        pubKey.Load(bytes);

        // Verifier object
        RSASSA_PKCS1v15_SHA_Verifier verifier(pubKey);

        std::string decodedSignature;
        HexDecoder decoder;
        decoder.Put( (byte*)encodedSignature.data(), encodedSignature.size() );
        decoder.MessageEnd();

        word64 size = decoder.MaxRetrievable();
        if(size && size <= SIZE_MAX)
        {
            decodedSignature.resize(size);		
            decoder.Get((byte*)&decodedSignature[0], decodedSignature.size());
        }

        SecByteBlock sigbyte(reinterpret_cast<const byte*>(&decodedSignature[0]), decodedSignature.size());

        // Verify
        bool result = verifier.VerifyMessage((const CryptoPP::byte*)hashedData.c_str(), hashedData.length(), sigbyte, sigbyte.size());

        // Result
        return result;
    }

    unsigned char random_char() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        return static_cast<unsigned char>(dis(gen));
    }

    // This is how we generate uuids and crypto addresses.
    std::string generate_uuid_hex(int len) {
        std::stringstream ss;
        for (auto i = 0; i < len; i++) {
            const auto rc = random_char();
            std::stringstream hexstream;
            hexstream << std::hex << int(rc);
            auto hex = hexstream.str();
            ss << (hex.length() < 2 ? '0' + hex : hex);
        }
        return ss.str();
    }
}