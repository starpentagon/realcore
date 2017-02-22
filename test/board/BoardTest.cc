#include "gtest/gtest.h"

#include "Board.h"

namespace realcore
{

class BoardTest
: public ::testing::Test
{
public:
};

TEST_F(BoardTest, IsInBoard)
{
  EXPECT_FALSE(IsInBoard(1, 0));
  EXPECT_FALSE(IsInBoard(0, 1));
  EXPECT_TRUE(IsInBoard(1, 1));
  EXPECT_TRUE(IsInBoard(15, 15));
  EXPECT_FALSE(IsInBoard(15, 16));
  EXPECT_FALSE(IsInBoard(16, 15));
}

}
