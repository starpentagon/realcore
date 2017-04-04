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

TEST_F(BitBoardTest, GetBoardDirectionTest)
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

TEST_F(BitBoardTest, GetBoardCordinateTest)
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

TEST_F(BitBoardTest, GetBoardPositionTest)
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

TEST_F(BitBoardTest, GetBoardMoveTest)
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
      EXPECT_EQ(board_position, GetBoardPosition(move, static_cast<BoardDirection>(i)));
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

TEST_F(BitBoardTest, GetBitBoardIndexListTest)
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

    EXPECT_EQ(0, GetBitBoardIndex(x, y, kLateralDirection));
    EXPECT_EQ(8, GetBitBoardIndex(x, y, kVerticalDirection));
    EXPECT_EQ(16, GetBitBoardIndex(x, y, kLeftDiagonalDirection));
    EXPECT_EQ(31, GetBitBoardIndex(x, y, kRightDiagonalDirection));
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

    EXPECT_EQ(4, GetBitBoardIndex(x, y, kLateralDirection));
    EXPECT_EQ(12, GetBitBoardIndex(x, y, kVerticalDirection));
    EXPECT_EQ(23, GetBitBoardIndex(x, y, kLeftDiagonalDirection));
    EXPECT_EQ(31, GetBitBoardIndex(x, y, kRightDiagonalDirection));
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

    EXPECT_EQ(7, GetBitBoardIndex(x, y, kLateralDirection));
    EXPECT_EQ(15, GetBitBoardIndex(x, y, kVerticalDirection));
    EXPECT_EQ(22, GetBitBoardIndex(x, y, kLeftDiagonalDirection));
    EXPECT_EQ(31, GetBitBoardIndex(x, y, kRightDiagonalDirection));
  }      
}

TEST_F(BitBoardTest, GetBitBoardShiftListTest){
  {
    // 左上の境界
    constexpr Cordinate x = 1, y = 1;
    array<size_t, kBoardDirectionNum> shift_list;
    GetBitBoardShiftList(x, y, &shift_list);

    EXPECT_EQ(34, shift_list[kLateralDirection]);
    EXPECT_EQ(34, shift_list[kVerticalDirection]);
    EXPECT_EQ(0, shift_list[kLeftDiagonalDirection]);
    EXPECT_EQ(0, shift_list[kRightDiagonalDirection]);

    EXPECT_EQ(34, GetBitBoardShift(x, y, kLateralDirection));
    EXPECT_EQ(34, GetBitBoardShift(x, y, kVerticalDirection));
    EXPECT_EQ(0, GetBitBoardShift(x, y, kLeftDiagonalDirection));
    EXPECT_EQ(0, GetBitBoardShift(x, y, kRightDiagonalDirection));
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

    EXPECT_EQ(48, GetBitBoardShift(x, y, kLateralDirection));
    EXPECT_EQ(18, GetBitBoardShift(x, y, kVerticalDirection));
    EXPECT_EQ(48, GetBitBoardShift(x, y, kLeftDiagonalDirection));
    EXPECT_EQ(48, GetBitBoardShift(x, y, kRightDiagonalDirection));
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

    EXPECT_EQ(62, GetBitBoardShift(x, y, kLateralDirection));
    EXPECT_EQ(62, GetBitBoardShift(x, y, kVerticalDirection));
    EXPECT_EQ(28, GetBitBoardShift(x, y, kLeftDiagonalDirection));
    EXPECT_EQ(28, GetBitBoardShift(x, y, kRightDiagonalDirection));
  }    
}

TEST_F(BitBoardTest, GetBitBoardIndexTest)
{
  // 各配列Indexの境界値テスト
  // 横方向
  EXPECT_EQ(0, GetBitBoardIndex(0));
  EXPECT_EQ(0, GetBitBoardIndex(31));
  EXPECT_EQ(1, GetBitBoardIndex(32));
  EXPECT_EQ(1, GetBitBoardIndex(63));
  EXPECT_EQ(2, GetBitBoardIndex(64));
  EXPECT_EQ(2, GetBitBoardIndex(95));
  EXPECT_EQ(3, GetBitBoardIndex(96));
  EXPECT_EQ(3, GetBitBoardIndex(127));
  EXPECT_EQ(4, GetBitBoardIndex(128));
  EXPECT_EQ(4, GetBitBoardIndex(159));
  EXPECT_EQ(5, GetBitBoardIndex(160));
  EXPECT_EQ(5, GetBitBoardIndex(191));
  EXPECT_EQ(6, GetBitBoardIndex(192));
  EXPECT_EQ(6, GetBitBoardIndex(223));
  EXPECT_EQ(7, GetBitBoardIndex(224));
  EXPECT_EQ(7, GetBitBoardIndex(255));

  // 縦方向
  EXPECT_EQ(8, GetBitBoardIndex(256));
  EXPECT_EQ(8, GetBitBoardIndex(287));
  EXPECT_EQ(9, GetBitBoardIndex(288));
  EXPECT_EQ(9, GetBitBoardIndex(319));
  EXPECT_EQ(10, GetBitBoardIndex(320));
  EXPECT_EQ(10, GetBitBoardIndex(351));
  EXPECT_EQ(11, GetBitBoardIndex(352));
  EXPECT_EQ(11, GetBitBoardIndex(383));
  EXPECT_EQ(12, GetBitBoardIndex(384));
  EXPECT_EQ(12, GetBitBoardIndex(415));
  EXPECT_EQ(13, GetBitBoardIndex(416));
  EXPECT_EQ(13, GetBitBoardIndex(447));
  EXPECT_EQ(14, GetBitBoardIndex(448));
  EXPECT_EQ(14, GetBitBoardIndex(479));
  EXPECT_EQ(15, GetBitBoardIndex(480));
  EXPECT_EQ(15, GetBitBoardIndex(511));

  // 左斜め方向
  EXPECT_EQ(16, GetBitBoardIndex(512));
  EXPECT_EQ(16, GetBitBoardIndex(543));
  EXPECT_EQ(17, GetBitBoardIndex(544));
  EXPECT_EQ(17, GetBitBoardIndex(575));
  EXPECT_EQ(18, GetBitBoardIndex(576));
  EXPECT_EQ(18, GetBitBoardIndex(607));
  EXPECT_EQ(19, GetBitBoardIndex(608));
  EXPECT_EQ(19, GetBitBoardIndex(639));
  EXPECT_EQ(20, GetBitBoardIndex(640));
  EXPECT_EQ(20, GetBitBoardIndex(671));
  EXPECT_EQ(21, GetBitBoardIndex(672));
  EXPECT_EQ(21, GetBitBoardIndex(703));
  EXPECT_EQ(22, GetBitBoardIndex(704));
  EXPECT_EQ(22, GetBitBoardIndex(735));
  EXPECT_EQ(23, GetBitBoardIndex(736));
  EXPECT_EQ(23, GetBitBoardIndex(767));

  // 右斜め方向
  EXPECT_EQ(24, GetBitBoardIndex(768));
  EXPECT_EQ(24, GetBitBoardIndex(799));
  EXPECT_EQ(25, GetBitBoardIndex(800));
  EXPECT_EQ(25, GetBitBoardIndex(831));
  EXPECT_EQ(26, GetBitBoardIndex(832));
  EXPECT_EQ(26, GetBitBoardIndex(863));
  EXPECT_EQ(27, GetBitBoardIndex(864));
  EXPECT_EQ(27, GetBitBoardIndex(895));
  EXPECT_EQ(28, GetBitBoardIndex(896));
  EXPECT_EQ(28, GetBitBoardIndex(927));
  EXPECT_EQ(29, GetBitBoardIndex(928));
  EXPECT_EQ(29, GetBitBoardIndex(959));
  EXPECT_EQ(30, GetBitBoardIndex(960));
  EXPECT_EQ(30, GetBitBoardIndex(991));
  EXPECT_EQ(31, GetBitBoardIndex(992));
  EXPECT_EQ(31, GetBitBoardIndex(1023));
}

TEST_F(BitBoardTest, GetBitBoardShiftTest)
{
  // シフト量の全数テスト
  const BitBoard bit_board;

  for(size_t i=0; i<1024; i++){
    const size_t shift_val = 2 * (i % 32);
    EXPECT_EQ(shift_val, GetBitBoardShift(i));
  }
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
