#include <gtest/gtest.h>
#include "Blockchain.hpp"
#include "Node.hpp"

TEST(PrawnstarTests, BlockchainHasGenesis)
{
    Node *node;
    Blockchain bc(node);
    EXPECT_EQ(bc.blocks.size(), 1);
}

