#include <gtest/gtest.h>
#include <interprete.hh>

// Un ejemplo simple
TEST(SumaTest, Positivos)
{
    EXPECT_EQ(2 + 2, 4);
}

TEST(SumaTest, Negativos)
{
    EXPECT_EQ(-2 + -2, -4);
}

// El punto de entrada para Google Test
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
