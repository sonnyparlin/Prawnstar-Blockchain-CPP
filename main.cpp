#include <iostream>
#include <typeinfo>
#include "Node.hpp"
#include "ProofOfStake.hpp"
#include "Lot.hpp"

int main(int argc, char *argv[]) {
    ProofOfStake pos;
    pos.update("bob", 15);
    pos.update("alice", 100);
    pos.update("phil", 45);
    pos.update("jimmy", 22);

    int bobWins {0};
    int aliceWins {0};
    int philWins {0};
    int jimmyWins {0};

    std::string forger;

    for (int i = 0; i < 100; i++) {
        forger = pos.forger(utils::gen_random_str(i));    

        if (forger == "bob")
            bobWins += 1;
        else if (forger == "alice")
            aliceWins += 1;
        else if (forger == "phil")
            philWins += 1;
        else if (forger == "jimmy")
            jimmyWins += 1;
    }

    std::cout << "bob wins: " << bobWins << std::endl;
    std::cout << "alice wins: " << aliceWins << std::endl;
    std::cout << "phil wins: " << philWins << std::endl;
    std::cout << "jimmy wins: " << jimmyWins << std::endl;

    // Node *node = Node::createNode(argc, argv);
    // node->startServers(argc, argv);
    // delete node;
	return 0;
}