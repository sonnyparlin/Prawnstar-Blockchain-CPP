#include "p2putils.hpp"

namespace p2putils {

void logit(std::string message) {
    std::cout << message << std::endl;
}

bool isValidIpAddress(const char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}

int createSocket() {
    int opted=1;
    int obj_server;
    if (( obj_server = socket ( AF_INET, SOCK_STREAM, 0)) == 0)
    {
        p2putils::logit ( "Opening of Socket Failed !");
        exit ( EXIT_FAILURE);
    }
    
    if ( setsockopt(obj_server, SOL_SOCKET, SO_REUSEADDR,
    &opted, sizeof ( opted )))
    {
        p2putils::logit ( "Can't set the socket" );
        exit ( EXIT_FAILURE );
    }
    return obj_server;
}

bool Bind(int server, struct sockaddr_in address, int PORT) {
    address.sin_port = htons( PORT );

    if (bind(server, ( struct sockaddr * )&address,
    sizeof(address))<0)
    {
        p2putils::logit ( "Binding of socket failed !" );
        exit(EXIT_FAILURE);
    }
    
    if (listen ( server, 3) < 0)
    {
        p2putils::logit ( "Can't listen from the server !");
        exit(EXIT_FAILURE);
    }
    return 1;
}

int setOutgoingNodeConnection(const char *ipaddress) {
    int outgoingSocket;

    if (!p2putils::isValidIpAddress(ipaddress)) {
        std::cout << "Invalid master ip address" << std::endl;
        return -1;
    }

    struct sockaddr_in cli_addr;
    cli_addr.sin_family = AF_INET;
    cli_addr.sin_addr.s_addr = INADDR_ANY;    
    inet_pton(AF_INET, ipaddress, &(cli_addr));
    if (( outgoingSocket = socket (AF_INET, SOCK_STREAM, 0 )) < 0)
    {
        printf ( "Socket creation error !" );
        return -1;
    }

    cli_addr.sin_port = htons(10001);
    // Converting IPv4 and IPv6 addresses from text to binary form
    if(inet_pton ( AF_INET, ipaddress, &cli_addr.sin_addr)<=0)
    {
        printf ( "\nInvalid address ! This IP Address is not supported !\n" );
        return -1;
    }
    
    if ( connect( outgoingSocket, (struct sockaddr *)&cli_addr, sizeof(cli_addr )) < 0)
    {
        printf ( "Connection Failed : Can't establish a connection over this socket !" );
        return -1;
    }
    
    return outgoingSocket;
}

std::string encodeObject(std::string objectToEnocde) {
    std::string encoded;
    CryptoPP::HexEncoder encoder;
    encoder.Put((CryptoPP::byte*)&objectToEnocde[0], objectToEnocde.size());
    encoder.MessageEnd();

    CryptoPP::word64 size = encoder.MaxRetrievable();
    if(size)
    {
        encoded.resize(size);		
        encoder.Get((CryptoPP::byte*)&encoded[0], encoded.size());
    }

    return encoded;
}

std::string decodeObject(std::string encodedObject) {
    // Decode hex message (the block in a json string format)
    CryptoPP::HexDecoder messageDecoder;
    messageDecoder.Put((CryptoPP::byte*)&encodedObject[0], encodedObject.size() );
    messageDecoder.MessageEnd();
    
    // Set up a decoded variable to copy the bytes into
    std::string decodedObject;
    CryptoPP::word64 size = messageDecoder.MaxRetrievable();
    if(size && size <= SIZE_MAX) {
        // set the size of the string to match the incoming bytes
        decodedObject.resize(size);
        // Create a byte string by copying the incoming bytes
        messageDecoder.Get((CryptoPP::byte*)&decodedObject[0], decodedObject.size());
    }

    return decodedObject;
}

} // namespace