#include "utils.hpp"

namespace utils {

    std::string hash(std::string msg)
    {   
        using namespace CryptoPP;
        std::string digest{""};

        SHA256 hash;
        StringSource s(msg, true, 
            new HashFilter(hash, new HexEncoder(new StringSink(digest))));

        std::cout << msg << std::endl;
        return digest; 
    }

}