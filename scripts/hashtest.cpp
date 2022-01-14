#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/hex.h>
#include <iostream>

int main (int argc, char* argv[])
{   
    using namespace CryptoPP;

    std::string msg = "Yoda said, Do or do not. There is no try.";
    std::string digest;

    SHA256 hash;
    StringSource s(msg, true, 
        new HashFilter(hash, new HexEncoder(new StringSink(digest))));

    std::cout << digest << std::endl;


    return 0; 
}
