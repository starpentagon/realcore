#include <array>

#include "gtest/gtest.h"

#include "Move.h"
#include "Board.h"

using namespace std;

namespace realcore
{

class BoardTest
: public ::testing::Test
{
public:
  void DefaultConstructor(){
    const auto &move_list = GetAllInBoardMove();
    const Board board;

    for(const auto move : move_list)
    {
      const PositionState state = board.GetState(move);
      EXPECT_EQ(kOpenPosition, state);
    }

    EXPECT_EQ(kOverBoard, board.GetState(kInvalidMove));
    EXPECT_EQ(kOverBoard, board.GetState(kNullMove));
    EXPECT_EQ(kOverBoard, board.GetState(kUndefinedMove01));
    EXPECT_EQ(kOverBoard, board.GetState(kUndefinedMove29));
  }

  void GetReadBoardPosition(){
    // 無効な手、Pass、未定義は読込専用BoardPositionになるかチェック
    const auto &move_list = GetAllMove();
    const Board board;

    for(auto move : move_list){
      if(IsInBoardMove(move)){
        continue;
      }

      EXPECT_EQ(224, board.GetReadBoardPosition(move, kLateralDirection));
      EXPECT_EQ(480, board.GetReadBoardPosition(move, kVerticalDirection));
      EXPECT_EQ(704, board.GetReadBoardPosition(move, kLeftDiagonalDirection));
      EXPECT_EQ(960, board.GetReadBoardPosition(move, kRightDiagonalDirection));
    }

    // 盤内の手
    {
      constexpr MovePosition move = kMoveAA;
      EXPECT_EQ(31, board.GetReadBoardPosition(move, kLateralDirection));
      EXPECT_EQ(287, board.GetReadBoardPosition(move, kVerticalDirection));
      EXPECT_EQ(543, board.GetReadBoardPosition(move, kLeftDiagonalDirection));
      EXPECT_EQ(1023, board.GetReadBoardPosition(move, kRightDiagonalDirection));
    }
    {
      constexpr MovePosition move = kMoveHI;
      EXPECT_EQ(152, board.GetReadBoardPosition(move, kLateralDirection));
      EXPECT_EQ(359, board.GetReadBoardPosition(move, kVerticalDirection));
      EXPECT_EQ(744, board.GetReadBoardPosition(move, kLeftDiagonalDirection));
      EXPECT_EQ(1000, board.GetReadBoardPosition(move, kRightDiagonalDirection));
    }
    {
      constexpr MovePosition move = kMoveOO;
      EXPECT_EQ(241, board.GetReadBoardPosition(move, kLateralDirection));
      EXPECT_EQ(497, board.GetReadBoardPosition(move, kVerticalDirection));
      EXPECT_EQ(721, board.GetReadBoardPosition(move, kLeftDiagonalDirection));
      EXPECT_EQ(1009, board.GetReadBoardPosition(move, kRightDiagonalDirection));
    }
  }

  void GetWriteBoardPosition(){
    // 無効な手、Pass、未定義は読込専用BoardPositionになるかチェック
    const auto &move_list = GetAllMove();
    const Board board;

    for(auto move : move_list){
      if(IsInBoardMove(move)){
        continue;
      }

      EXPECT_EQ(225, board.GetWriteBoardPosition(move, kLateralDirection));
      EXPECT_EQ(481, board.GetWriteBoardPosition(move, kVerticalDirection));
      EXPECT_EQ(736, board.GetWriteBoardPosition(move, kLeftDiagonalDirection));
      EXPECT_EQ(992, board.GetWriteBoardPosition(move, kRightDiagonalDirection));
    }

    // 盤内の手
    {
      constexpr MovePosition move = kMoveAA;
      EXPECT_EQ(31, board.GetWriteBoardPosition(move, kLateralDirection));
      EXPECT_EQ(287, board.GetWriteBoardPosition(move, kVerticalDirection));
      EXPECT_EQ(543, board.GetWriteBoardPosition(move, kLeftDiagonalDirection));
      EXPECT_EQ(1023, board.GetWriteBoardPosition(move, kRightDiagonalDirection));
    }
    {
      constexpr MovePosition move = kMoveHI;
      EXPECT_EQ(152, board.GetWriteBoardPosition(move, kLateralDirection));
      EXPECT_EQ(359, board.GetWriteBoardPosition(move, kVerticalDirection));
      EXPECT_EQ(744, board.GetWriteBoardPosition(move, kLeftDiagonalDirection));
      EXPECT_EQ(1000, board.GetWriteBoardPosition(move, kRightDiagonalDirection));
    }
    {
      constexpr MovePosition move = kMoveOO;
      EXPECT_EQ(241, board.GetWriteBoardPosition(move, kLateralDirection));
      EXPECT_EQ(497, board.GetWriteBoardPosition(move, kVerticalDirection));
      EXPECT_EQ(721, board.GetWriteBoardPosition(move, kLeftDiagonalDirection));
      EXPECT_EQ(1009, board.GetWriteBoardPosition(move, kRightDiagonalDirection));
    }
  }

  void GetBitBoardIndex(){
     const Board board;

     // 横方向
     EXPECT_EQ(0, board.GetBitBoardIndex(16));
     EXPECT_EQ(1, board.GetBitBoardIndex(48));
     EXPECT_EQ(2, board.GetBitBoardIndex(80));
     EXPECT_EQ(3, board.GetBitBoardIndex(112));
     EXPECT_EQ(4, board.GetBitBoardIndex(144));
     EXPECT_EQ(5, board.GetBitBoardIndex(176));
     EXPECT_EQ(6, board.GetBitBoardIndex(208));
     EXPECT_EQ(7, board.GetBitBoardIndex(240));

     // 縦方向
     EXPECT_EQ(8, board.GetBitBoardIndex(287));
     EXPECT_EQ(9, board.GetBitBoardIndex(319));
     EXPECT_EQ(10, board.GetBitBoardIndex(351));
     EXPECT_EQ(11, board.GetBitBoardIndex(383));
     EXPECT_EQ(12, board.GetBitBoardIndex(415));
     EXPECT_EQ(13, board.GetBitBoardIndex(447));
     EXPECT_EQ(14, board.GetBitBoardIndex(479));
     EXPECT_EQ(15, board.GetBitBoardIndex(511));

     // 左斜め方向
     EXPECT_EQ(16, board.GetBitBoardIndex(543));
     EXPECT_EQ(17, board.GetBitBoardIndex(575));
     EXPECT_EQ(18, board.GetBitBoardIndex(607));
     EXPECT_EQ(19, board.GetBitBoardIndex(639));
     EXPECT_EQ(20, board.GetBitBoardIndex(671));
     EXPECT_EQ(21, board.GetBitBoardIndex(703));
     EXPECT_EQ(22, board.GetBitBoardIndex(735));
     EXPECT_EQ(23, board.GetBitBoardIndex(767));

     // 右斜め方向
     EXPECT_EQ(24, board.GetBitBoardIndex(797));
     EXPECT_EQ(25, board.GetBitBoardIndex(827));
     EXPECT_EQ(26, board.GetBitBoardIndex(857));
     EXPECT_EQ(27, board.GetBitBoardIndex(887));
     EXPECT_EQ(28, board.GetBitBoardIndex(917));
     EXPECT_EQ(29, board.GetBitBoardIndex(947));
     EXPECT_EQ(30, board.GetBitBoardIndex(977));
     EXPECT_EQ(31, board.GetBitBoardIndex(1023));
  }

  void GetBitBoardShift(){
    const Board board;

    for(size_t i=0; i<1024; i++){
      const size_t shift_val = 2 * (i % 32);
      EXPECT_EQ(shift_val, board.GetBitBoardShift(i));
    }
  }
};

TEST_F(BoardTest, DefaultConstructor){
  DefaultConstructor();
}

TEST_F(BoardTest, GetSetState)
{
  // todo 盤内の全点についてテスト & bit_board_の中身もテストする
  Board board;
  
  // 盤内
  EXPECT_EQ(kOpenPosition, board.GetState(kMoveAB));
  EXPECT_EQ(kOpenPosition, board.GetState(kMoveAC));
  
  board.SetState(kMoveAB, kBlackStone);
  board.SetState(kMoveAC, kWhiteStone);

  EXPECT_EQ(kBlackStone, board.GetState(kMoveAB));
  EXPECT_EQ(kWhiteStone, board.GetState(kMoveAC));

  board.SetState(kMoveAB, kOpenPosition);
  board.SetState(kMoveAC, kOpenPosition);

  EXPECT_EQ(kOpenPosition, board.GetState(kMoveAB));
  EXPECT_EQ(kOpenPosition, board.GetState(kMoveAC));

  // 無効手, Pass, 未定義: SetStateは反映されず常にkOverBoardが返ってくる
  const array<MovePosition, 4> move_list{{
    kInvalidMove, kNullMove, kUndefinedMove01, kUndefinedMove29
  }};

  for(const auto move : move_list){
    EXPECT_EQ(kOverBoard, board.GetState(move));

    board.SetState(move, kBlackStone);
    EXPECT_EQ(kOverBoard, board.GetState(move));

    board.SetState(move, kWhiteStone);
    EXPECT_EQ(kOverBoard, board.GetState(move));

    board.SetState(move, kOpenPosition);
    EXPECT_EQ(kOverBoard, board.GetState(move));
  }
}

TEST_F(BoardTest, GetReadBoardPosition){
  GetReadBoardPosition();
}

TEST_F(BoardTest, GetWriteBoardPosition){
  GetWriteBoardPosition();
}

TEST_F(BoardTest, GetBitBoardIndex){
  GetBitBoardIndex();
}

TEST_F(BoardTest, GetBitBoardShift){
  GetBitBoardShift();
}

TEST_F(BoardTest, IsInBoard)
{
  EXPECT_FALSE(IsInBoard(1, 0));
  EXPECT_FALSE(IsInBoard(0, 1));
  EXPECT_TRUE(IsInBoard(1, 1));
  EXPECT_TRUE(IsInBoard(15, 15));
  EXPECT_FALSE(IsInBoard(15, 16));
  EXPECT_FALSE(IsInBoard(16, 15));
}

}   // namespace realcore
