#include "p2putils.hpp"

class P2PServer {
public:
    std::unordered_map<std::string, int> nodeMapList;

    P2PServer();
    ~P2PServer();


    int socketSendHandler(int );
    int processArgs(int argc, char **argv);
    int createServer ( int argc, char **argv );
    int setOutgoingNodeConnection(int sock);
    int socketReceiveHandler(int s);
    bool addToNodeMapList(std::string ipAddress, int PORT);
    std::unordered_map<std::string, int> getNodeMapList();
    bool nodeInMapList(std::string ipAddress);
    int initializeWithMasterNode(int sock);
};