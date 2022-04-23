#include <gtest/gtest.h>
#include "Node.hpp"

#define GTEST_COUT std::cout << "[   INFO   ] "

namespace MasterTestSpace {
    class BlockchainTests : public ::testing::Test {

    protected:
        char *arguments[5] = {
                (char *) "appname",
                (char *) "127.0.0.1",
                (char *) "10001",
                (char *) "8001",
                nullptr
        };

        Node *node = new Node(arguments);

        BlockchainTests()=default;

        ~BlockchainTests() override {
            delete node;
        }

        void SetUp() override {
        }

        void TearDown() override {}

    };

    TEST_F(BlockchainTests, BlockchainHasGenesis) {
        EXPECT_EQ(node->blockchain->blocks.size(), 1);
    }

    TEST_F(BlockchainTests, BlockchainHasGenesisNodeStake) {
        EXPECT_EQ(node->proofOfStake->stakers.size(), 1);
    }

    TEST_F(BlockchainTests, NodeWalletAddress) {
        EXPECT_EQ(node->nodeWallet->address, "pv171e654b041c51d978becc5cf65b0e2f48ff7");
    }

    TEST_F(BlockchainTests, TransactionTests) {
        std::string sender = "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac"; // exchange
        std::string receiver = "pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7"; // alice
        std::string type = "EXCHANGE"; // extract type
        double amount = 20000;

        Wallet senderWallet(sender.c_str(), node);
        EXPECT_EQ(senderWallet.address, "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac");
        Transaction tx = senderWallet.createTransaction(receiver, amount, type);
        EXPECT_EQ(tx.senderAddress, "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac");
        EXPECT_EQ(tx.receiverAddress, "pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7");
        EXPECT_TRUE(node->handleTransaction(tx));
        EXPECT_EQ(node->blockchain->blocks.size(), 2);
        EXPECT_EQ(node->accountModel->getBalance(node->aliceWallet->address), 20000 - (20000 * 0.005));
        EXPECT_EQ(node->accountModel->getBalance(node->nodeWallet->address), (20000 * 0.005));
    }

    TEST_F(BlockchainTests, TransactionStressTest) {
        std::string sender = "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac"; // exchange
        std::string receiver = "pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7"; // alice
        std::string type = "EXCHANGE"; // extract type
        double amount = 20000;

        Wallet senderWallet(sender.c_str(), node);
        EXPECT_EQ(senderWallet.address, "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac");
        Transaction tx = senderWallet.createTransaction(receiver, amount, type);
        EXPECT_EQ(tx.senderAddress, "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac");
        EXPECT_EQ(tx.receiverAddress, "pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7");
        EXPECT_TRUE(node->handleTransaction(tx));
        EXPECT_EQ(node->blockchain->blocks.size(), 2);
        EXPECT_EQ(node->accountModel->getBalance(node->aliceWallet->address), 20000 - (20000 * 0.005));
        EXPECT_EQ(node->accountModel->getBalance(node->nodeWallet->address), (20000 * 0.005));

        GTEST_COUT << "Executing 1000 transactions, please wait..." << std::endl;
        for (int i = 0; i < 1000; i++) {
            Transaction txi = node->aliceWallet->createTransaction(
                    "pv17ca8886e573b6749aeeb7b87387b8e01fcd5f42",
                    1,
                    "TRANSFER");
            EXPECT_TRUE(node->handleTransaction(txi));
        }

        double expected = 18905;
        auto result = node->accountModel->getBalance(node->aliceWallet->address);
        result = roundf((float)result * 100) / 100;
        EXPECT_DOUBLE_EQ(expected, result);
    }

    TEST_F(BlockchainTests, TransactionNotCovered) {
        // No exchange transaction first, so we should fail
        std::string sender = "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac"; // alice
        std::string receiver = "pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7"; // bob
        std::string type = "TRANSFER"; // type
        double amount = 1;
        Wallet senderWallet(sender.c_str(), node);
        Transaction tx = senderWallet.createTransaction(receiver, amount, type);
        EXPECT_FALSE(node->handleTransaction(tx));
        EXPECT_EQ(node->blockchain->blocks.size(), 1);
    }

    TEST_F(BlockchainTests, IncorrectSignature) {
        std::string sender = "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac"; // exchange
        std::string receiver = "pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7"; // alice
        std::string type = "EXCHANGE"; // extract type
        double amount = 20000;

        // Send tokens to Alice
        Wallet senderWallet(sender.c_str(), node);
        EXPECT_EQ(senderWallet.address, "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac");
        Transaction tx = senderWallet.createTransaction(receiver, amount, type);
        EXPECT_EQ(tx.senderAddress, "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac");
        EXPECT_EQ(tx.receiverAddress, "pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7");
        EXPECT_TRUE(node->handleTransaction(tx));
        EXPECT_EQ(node->blockchain->blocks.size(), 2);

        // Alice attempts to send to bob
        std::string sender2 = "pv137a7ea711dd4a12c97e4391ad4bade5b353b1d7"; // alice
        std::string receiver2 = "pv17ca8886e573b6749aeeb7b87387b8e01fcd5f42"; // bob
        std::string type2 = "TRANSFER"; // type
        double amount2 = 1;
        Wallet senderWallet2(sender2.c_str(), node);
        Transaction tx2 = senderWallet2.createTransaction(receiver2, amount2, type2);
        // Evil fake signature
        tx2.signature = "304602210095013C67B56C42925DFB8846EB2076CBA5C391CA52E698623C6D754908BB956F0221009EE4F3158BBDAA450F9EEEA251A63C6B871D5D95FE1B2FD8BCF4EC898C3A4FDC:72"; // bad signature
        EXPECT_FALSE(node->handleTransaction(tx2));
        EXPECT_EQ(node->blockchain->blocks.size(), 2);
    }
}