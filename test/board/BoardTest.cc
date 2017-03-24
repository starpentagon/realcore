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
};

TEST_F(BoardTest, IsInBoardTest)
{
  // 境界値テスト
  EXPECT_FALSE(IsInBoard(1, 0));
  EXPECT_FALSE(IsInBoard(0, 1));
  EXPECT_TRUE(IsInBoard(1, 1));
  EXPECT_TRUE(IsInBoard(15, 15));
}

TEST_F(BoardTest, GetBoardDirectionTest)
{
  // 境界値テスト
  EXPECT_EQ(kLateralDirection, GetBoardDirection(0));
  EXPECT_EQ(kLateralDirection, GetBoardDirection(255));
  EXPECT_EQ(kVerticalDirection, GetBoardDirection(256));
  EXPECT_EQ(kVerticalDirection, GetBoardDirection(511));
  EXPECT_EQ(kLeftDiagonalDirection, GetBoardDirection(512));
  EXPECT_EQ(kLeftDiagonalDirection, GetBoardDirection(767));
  EXPECT_EQ(kRightDiagonalDirection, GetBoardDirection(768));
  EXPECT_EQ(kRightDiagonalDirection, GetBoardDirection(1023));
}

TEST_F(BoardTest, GetBoardCordinateTest)
{
  Cordinate x = 0, y = 0;
  
  {
    // 左上
    constexpr Cordinate expect_x = 1, expect_y = 1;

    GetBoardCordinate(17, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);

    GetBoardCordinate(273, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);

    GetBoardCordinate(512, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);

    GetBoardCordinate(992, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);
  }
  {
    // 右上
    constexpr Cordinate expect_x = 15, expect_y = 1;

    GetBoardCordinate(31, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);

    GetBoardCordinate(497, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);

    GetBoardCordinate(736, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);

    GetBoardCordinate(768, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);
  }
  {
    // 左下
    constexpr Cordinate expect_x = 1, expect_y = 15;

    GetBoardCordinate(241, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);

    GetBoardCordinate(287, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);

    GetBoardCordinate(750, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);

    GetBoardCordinate(974, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);
  }
  {
    // 右下
    constexpr Cordinate expect_x = 15, expect_y = 15;

    GetBoardCordinate(255, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);

    GetBoardCordinate(511, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);

    GetBoardCordinate(718, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);

    GetBoardCordinate(1006, &x, &y);
    EXPECT_EQ(expect_x, x);
    EXPECT_EQ(expect_y, y);
  }
}

TEST_F(BoardTest, GetBoardPositionTest)
{
  for(size_t index=0; index<32; index++)
  {
    for(size_t shift=0; shift<64; shift+=2)
    {
      const BoardPosition expect_board_position = 32 * index + shift / 2;
      const BoardPosition board_position = GetBoardPosition(index, shift);

      EXPECT_EQ(expect_board_position, board_position);
    }
  }
}

TEST_F(BoardTest, GetBoardMoveTest)
{
  // move -> board_positionの対応表を作る
  array< vector<BoardPosition>, kMoveNum> move_board_position_list;

  for(const auto move : GetAllInBoardMove()){
    Cordinate x = 0, y = 0;
    GetMoveCordinate(move, &x, &y);

    array<size_t, kBoardDirectionNum> index_list;
    GetBitBoardIndexList(x, y, &index_list);

    array<size_t, kBoardDirectionNum> shift_list;
    GetBitBoardShiftList(x, y, &shift_list);

    for(size_t i=0; i<kBoardDirectionNum; i++)
    {
      const BoardPosition board_position = GetBoardPosition(index_list[i], shift_list[i]);
      move_board_position_list[move].push_back(board_position);
    }
  }

  for(BoardPosition board_position=0; board_position<1024; ++board_position)
  {
    const MovePosition move = GetBoardMove(board_position);

    if(IsInBoardMove(move)){
      const vector<BoardPosition> &board_position_list = move_board_position_list[move];
      vector<BoardPosition>::const_iterator find_it = find(board_position_list.cbegin(), board_position_list.cend(), board_position);

      EXPECT_TRUE(find_it != board_position_list.end());
    }else{
      EXPECT_EQ(move, kInvalidMove);
    }
  }
}

TEST_F(BoardTest, GetBitBoardIndexListTest)
{
  {
    // 左上の境界
    constexpr Cordinate x = 1, y = 1;
    array<size_t, kBoardDirectionNum> index_list;
    GetBitBoardIndexList(x, y, &index_list);

    EXPECT_EQ(0, index_list[kLateralDirection]);
    EXPECT_EQ(8, index_list[kVerticalDirection]);
    EXPECT_EQ(16, index_list[kLeftDiagonalDirection]);
    EXPECT_EQ(31, index_list[kRightDiagonalDirection]);
  }
  {
    // 盤内
    constexpr Cordinate x = 8, y = 9;
    array<size_t, kBoardDirectionNum> index_list;
    GetBitBoardIndexList(x, y, &index_list);

    EXPECT_EQ(4, index_list[kLateralDirection]);
    EXPECT_EQ(12, index_list[kVerticalDirection]);
    EXPECT_EQ(23, index_list[kLeftDiagonalDirection]);
    EXPECT_EQ(31, index_list[kRightDiagonalDirection]);
  }      
  {
    // 右下の境界
    constexpr Cordinate x = 15, y = 15;
    array<size_t, kBoardDirectionNum> index_list;
    GetBitBoardIndexList(x, y, &index_list);

    EXPECT_EQ(7, index_list[kLateralDirection]);
    EXPECT_EQ(15, index_list[kVerticalDirection]);
    EXPECT_EQ(22, index_list[kLeftDiagonalDirection]);
    EXPECT_EQ(31, index_list[kRightDiagonalDirection]);
  }      
}

TEST_F(BoardTest, GetBitBoardShiftListTest){
  {
    // 左上の境界
    constexpr Cordinate x = 1, y = 1;
    array<size_t, kBoardDirectionNum> shift_list;
    GetBitBoardShiftList(x, y, &shift_list);

    EXPECT_EQ(34, shift_list[kLateralDirection]);
    EXPECT_EQ(34, shift_list[kVerticalDirection]);
    EXPECT_EQ(0, shift_list[kLeftDiagonalDirection]);
    EXPECT_EQ(0, shift_list[kRightDiagonalDirection]);
  }
  {
    // 盤内
    constexpr Cordinate x = 8, y = 9;
    array<size_t, kBoardDirectionNum> shift_list;
    GetBitBoardShiftList(x, y, &shift_list);

    EXPECT_EQ(48, shift_list[kLateralDirection]);
    EXPECT_EQ(18, shift_list[kVerticalDirection]);
    EXPECT_EQ(48, shift_list[kLeftDiagonalDirection]);
    EXPECT_EQ(48, shift_list[kRightDiagonalDirection]);
  }
  {
    // 右下の境界
    constexpr Cordinate x = 15, y = 15;
    array<size_t, kBoardDirectionNum> shift_list;
    GetBitBoardShiftList(x, y, &shift_list);

    EXPECT_EQ(62, shift_list[kLateralDirection]);
    EXPECT_EQ(62, shift_list[kVerticalDirection]);
    EXPECT_EQ(28, shift_list[kLeftDiagonalDirection]);
    EXPECT_EQ(28, shift_list[kRightDiagonalDirection]);
  }    
}

TEST_F(BoardTest, IsEqualTest)
{
  Board board_1, board_2;

  EXPECT_TRUE(IsEqual(board_1, board_1));
  EXPECT_TRUE(IsEqual(board_1, board_2));

  board_1.SetState<kBlackStone>(kMoveHH);
  EXPECT_TRUE(IsEqual(board_1, board_1));
  EXPECT_FALSE(IsEqual(board_1, board_2));
  
  board_2.SetState<kBlackStone>(kMoveHH);
  EXPECT_TRUE(IsEqual(board_1, board_2));
}

TEST_F(BoardTest, CopyTest)
{
  Board board_1, board_2;

  board_1.SetState<kBlackStone>(kMoveHH);
  EXPECT_FALSE(IsEqual(board_1, board_2));

  Copy(board_1, &board_2);
  EXPECT_TRUE(IsEqual(board_1, board_2));
}

}   // namespace realcore
