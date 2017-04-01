// @brief 非メンバ関数のテスト
#include <algorithm>
#include <array>

#include "gtest/gtest.h"

#include "Move.h"
#include "MoveList.h"
#include "Board.h"

using namespace std;

namespace realcore
{

class BoardTest
: public ::testing::Test
{
public:

};

TEST_F(BoardTest, IsEqualTest)
{
  Board board_1, board_2;

  EXPECT_TRUE(IsEqual(board_1, board_1));
  EXPECT_TRUE(IsEqual(board_1, board_2));

  board_1.MakeMove(kMoveHH);
  EXPECT_TRUE(IsEqual(board_1, board_1));
  EXPECT_FALSE(IsEqual(board_1, board_2));
  
  board_2.MakeMove(kMoveHH);
  EXPECT_TRUE(IsEqual(board_1, board_2));
}

TEST_F(BoardTest, CopyTest)
{
  Board board_1, board_2;

  board_1.MakeMove(kMoveHH);
  EXPECT_FALSE(IsEqual(board_1, board_2));

  Copy(board_1, &board_2);
  EXPECT_TRUE(IsEqual(board_1, board_2));
}

}   // namespace realcore
