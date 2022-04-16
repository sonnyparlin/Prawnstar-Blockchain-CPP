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
                NULL
        };

        Node *node = new Node(4, arguments);

        BlockchainTests() {
        }

        ~BlockchainTests() {
            delete node;
        }

        virtual void SetUp() {
        }

        virtual void TearDown() {}

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

        GTEST_COUT << "Executing 1000 transactions, please wait..." << std::endl;
        for (int i = 0; i < 1000; i++) {
            Transaction tx = node->aliceWallet->createTransaction(
                    "pv17ca8886e573b6749aeeb7b87387b8e01fcd5f42",
                    1,
                    "TRANSFER");
            EXPECT_TRUE(node->handleTransaction(tx));
        }

        double expected = 18905;
        double result = node->accountModel->getBalance(node->aliceWallet->address);
        result = roundf(result * 100) / 100;
        EXPECT_DOUBLE_EQ(expected, result);
    }

    TEST_F(BlockchainTests, ProofOfStakeTests) {
        std::string sender = "pv1403d478bfc4949c9c68af53bbaf8deb58c4eac"; // exchange
        std::string receiver = node->nodeWallet->address.c_str(); // node wallet
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

        // Since I am the only forger, i should get back my own node's public key
        EXPECT_EQ(node->blockchain->nextForger(),node->nodeWallet->walletPublicKey);
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
        tx2.signature = "bad signature:456"; // bad signature
        EXPECT_FALSE(node->handleTransaction(tx2));
        EXPECT_EQ(node->blockchain->blocks.size(), 2);
    }
}