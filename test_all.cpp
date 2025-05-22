#include <gtest/gtest.h>

#include "player.hpp"

// Example test case
TEST(Player, Fields)
{
    hyper_block::player p;
    p.set_nickname("Player1");
    EXPECT_EQ(p.get_nickname(), "Player1");

    boost::numeric::ublas::vector<double> position(3);
    position[0] = 1.0;
    position[1] = 2.0;
    position[2] = 3.0;
    p.set_position(position);
    EXPECT_EQ(p.get_position()[0], 1.0);
    EXPECT_EQ(p.get_position()[1], 2.0);
    EXPECT_EQ(p.get_position()[2], 3.0);
    EXPECT_EQ(p.get_position().size(), 3);

    p.set_facing_right(true);
    EXPECT_TRUE(p.is_facing_right());

    p.set_moving(true);
    EXPECT_TRUE(p.is_moving());
    p.set_moving(false);
    EXPECT_FALSE(p.is_moving());
    p.set_facing_right(false);
    EXPECT_FALSE(p.is_facing_right());
}

int
main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
