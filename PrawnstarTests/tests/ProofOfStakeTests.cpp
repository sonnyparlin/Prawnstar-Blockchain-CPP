#include <gtest/gtest.h>
#include "Node.hpp"
#include "ProofOfStake.hpp"

#define GTEST_COUT std::cout << "[   INFO   ] "

namespace MasterTestSpace {
    class ProofOfStakeTests : public ::testing::Test {

    protected:
        char *arguments[5] = {
                (char *) "appname",
                (char *) "127.0.0.1",
                (char *) "10001",
                (char *) "8001",
                nullptr
        };

        Node *node = new Node(arguments);

        ProofOfStakeTests()=default;

        ~ProofOfStakeTests() override = default;

        void SetUp() override {
        }

        void TearDown() override {}

    };

    TEST_F(ProofOfStakeTests, StakerLotteryTest)
    {
        ProofOfStake pos(node);
        // pos.update("bob", 1);
        pos.update("alice", 50);
        pos.update("phil", 100);
        pos.update("jimmy", 100000);

        int bobWins {0};
        int aliceWins {0};
        int philWins {0};
        int jimmyWins {0};

        std::string forger;
        int loopTimes = 1000;

        for (int i = 0; i < loopTimes; i++) {
            forger = pos.forger(utils::gen_random_str(i));

            if (forger == node->nodeWallet->walletPublicKey)
                bobWins += 1;
            else if (forger == "alice")
                aliceWins += 1;
            else if (forger == "phil")
                philWins += 1;
            else if (forger == "jimmy")
                jimmyWins += 1;
        }

        EXPECT_TRUE(bobWins < aliceWins);
        EXPECT_TRUE(aliceWins < philWins);
        EXPECT_TRUE(philWins < jimmyWins);
        EXPECT_EQ(bobWins+aliceWins+philWins+jimmyWins, loopTimes);

        delete node;
    }

    TEST_F(ProofOfStakeTests, StakerLotterySanityTest)
    {
        ProofOfStake pos(node);
        int bobWins {0};

        std::string forger;
        int loopTimes = 578;

        for (int i = 0; i < loopTimes; i++) {
            forger = pos.forger(utils::gen_random_str(i));

            if (forger == node->nodeWallet->walletPublicKey)
                bobWins += 1;
        }

        EXPECT_EQ(bobWins, loopTimes);
        delete node;
    }

    TEST_F(ProofOfStakeTests, AddStakeTest) {
        std::string sender = "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac"; // exchange
        std::string receiver = node->nodeWallet->address; // node wallet
        std::string type = "EXCHANGE"; // type
        double amount = 100000;
        Wallet senderWallet(sender.c_str(), node);
        EXPECT_EQ(senderWallet.address, "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac");
        Transaction tx = senderWallet.createTransaction(receiver, amount, type);
        EXPECT_TRUE(node->handleTransaction(tx));

        std::string type2 = "STAKE";
        Wallet senderWallet2(receiver.c_str(), node);
        Transaction tx2 = senderWallet2.createTransaction(receiver, amount, type2);
        EXPECT_TRUE(node->handleTransaction(tx2));
        EXPECT_EQ(node->blockchain->blocks.size(), 3);
        EXPECT_EQ(node->proofOfStake->getStake(node->nodeWallet->walletPublicKey), 100001);

        // Since I am the only forger, I should get back my own node's public key
        EXPECT_EQ(node->blockchain->nextForger(),node->nodeWallet->walletPublicKey);
    }
}