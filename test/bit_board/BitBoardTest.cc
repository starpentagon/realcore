// @brief 非メンバ関数のテスト
#include <algorithm>
#include <array>

#include "gtest/gtest.h"

#include "Move.h"
#include "MoveList.h"
#include "BitBoard.h"

using namespace std;

namespace realcore
{

class BitBoardTest
: public ::testing::Test
{
};

TEST_F(BitBoardTest, IsInBitBoardTest)
{
  // 境界値テスト
  EXPECT_FALSE(IsInBoard(1, 0));
  EXPECT_FALSE(IsInBoard(0, 1));
  EXPECT_TRUE(IsInBoard(1, 1));
  EXPECT_TRUE(IsInBoard(15, 15));
}

TEST_F(BitBoardTest, IsEqualTest)
{
  BitBoard bit_board_1, bit_board_2;

  EXPECT_TRUE(IsEqual(bit_board_1, bit_board_1));
  EXPECT_TRUE(IsEqual(bit_board_1, bit_board_2));

  bit_board_1.SetState<kBlackStone>(kMoveHH);
  EXPECT_TRUE(IsEqual(bit_board_1, bit_board_1));
  EXPECT_FALSE(IsEqual(bit_board_1, bit_board_2));
  
  bit_board_2.SetState<kBlackStone>(kMoveHH);
  EXPECT_TRUE(IsEqual(bit_board_1, bit_board_2));
}

TEST_F(BitBoardTest, CopyTest)
{
  BitBoard bit_board_1, bit_board_2;

  bit_board_1.SetState<kBlackStone>(kMoveHH);
  EXPECT_FALSE(IsEqual(bit_board_1, bit_board_2));

  Copy(bit_board_1, &bit_board_2);
  EXPECT_TRUE(IsEqual(bit_board_1, bit_board_2));
}

}   // namespace realcore
