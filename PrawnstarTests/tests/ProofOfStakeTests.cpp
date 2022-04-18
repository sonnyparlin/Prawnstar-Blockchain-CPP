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

        Node *node = new Node(4, arguments);

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
        pos.update("alice", 500);
        pos.update("phil", 5000);
        pos.update("jimmy", 50000);

        int bobWins {0};
        int aliceWins {0};
        int philWins {0};
        int jimmyWins {0};

        std::string forger;
        int loopTimes = 100;

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
}