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
  void DefaultConstructorTest(){
    const auto &move_list = GetAllInBoardMove();
    const Board board;

    for(const auto move : move_list)
    {
      const PositionState state = board.GetState(move);
      EXPECT_EQ(kOpenPosition, state);
    }

    EXPECT_EQ(kOverBoard, board.GetState(kInvalidMove));
    EXPECT_EQ(kOverBoard, board.GetState(kNullMove));
    EXPECT_EQ(kOverBoard, board.GetState(kDeclareEndGame));
    EXPECT_EQ(kOverBoard, board.GetState(kUndefinedMove01));
    EXPECT_EQ(kOverBoard, board.GetState(kUndefinedMove28));
  }

  void GetSetStateTest(){
    Board board;
    const auto &all_move_list = GetAllMove();

    // 黒石設定
    for(auto set_move : all_move_list){
      board.SetState<kBlackStone>(set_move);

      for(auto get_move : all_move_list){
        PositionState check_state = kOverBoard;

        if(IsInBoardMove(get_move)){
          check_state = set_move == get_move ? kBlackStone : kOpenPosition;
        }

        EXPECT_EQ(check_state, board.GetState(get_move));
      }

      board.SetState<kOpenPosition>(set_move);
    }

    // 白石設定
    for(auto set_move : all_move_list){
      board.SetState<kWhiteStone>(set_move);

      for(auto get_move : all_move_list){
        PositionState check_state = kOverBoard;

        if(IsInBoardMove(get_move)){
          check_state = set_move == get_move ? kWhiteStone : kOpenPosition;
        }

        EXPECT_EQ(check_state, board.GetState(get_move));
      }

      board.SetState<kOpenPosition>(set_move);
    }
  }

  void GetBitBoardIndexListTest()
  {
    Board board;
    {
      constexpr Cordinate x = 1, y = 1;
      array<size_t, kBoardDirectionNum> index_list;
      board.GetBitBoardIndexList(x, y, &index_list);

      EXPECT_EQ(0, index_list[kLateralDirection]);
      EXPECT_EQ(8, index_list[kVerticalDirection]);
      EXPECT_EQ(16, index_list[kLeftDiagonalDirection]);
      EXPECT_EQ(31, index_list[kRightDiagonalDirection]);
    }
    {
      constexpr Cordinate x = 8, y = 9;
      array<size_t, kBoardDirectionNum> index_list;
      board.GetBitBoardIndexList(x, y, &index_list);

      EXPECT_EQ(4, index_list[kLateralDirection]);
      EXPECT_EQ(12, index_list[kVerticalDirection]);
      EXPECT_EQ(23, index_list[kLeftDiagonalDirection]);
      EXPECT_EQ(31, index_list[kRightDiagonalDirection]);
    }      
    {
      constexpr Cordinate x = 15, y = 10;
      array<size_t, kBoardDirectionNum> index_list;
      board.GetBitBoardIndexList(x, y, &index_list);

      EXPECT_EQ(5, index_list[kLateralDirection]);
      EXPECT_EQ(15, index_list[kVerticalDirection]);
      EXPECT_EQ(19, index_list[kLeftDiagonalDirection]);
      EXPECT_EQ(28, index_list[kRightDiagonalDirection]);
    }      
  }

  void GetBitBoardShiftListTest(){
    Board board;
    {
      constexpr Cordinate x = 1, y = 1;
      array<size_t, kBoardDirectionNum> shift_list;
      board.GetBitBoardShiftList(x, y, &shift_list);

      EXPECT_EQ(34, shift_list[kLateralDirection]);
      EXPECT_EQ(34, shift_list[kVerticalDirection]);
      EXPECT_EQ(0, shift_list[kLeftDiagonalDirection]);
      EXPECT_EQ(0, shift_list[kRightDiagonalDirection]);
    }
    {
      constexpr Cordinate x = 8, y = 9;
      array<size_t, kBoardDirectionNum> shift_list;
      board.GetBitBoardShiftList(x, y, &shift_list);

      EXPECT_EQ(48, shift_list[kLateralDirection]);
      EXPECT_EQ(18, shift_list[kVerticalDirection]);
      EXPECT_EQ(48, shift_list[kLeftDiagonalDirection]);
      EXPECT_EQ(48, shift_list[kRightDiagonalDirection]);
    }
    {
      constexpr Cordinate x = 15, y = 10;
      array<size_t, kBoardDirectionNum> shift_list;
      board.GetBitBoardShiftList(x, y, &shift_list);

      EXPECT_EQ(30, shift_list[kLateralDirection]);
      EXPECT_EQ(52, shift_list[kVerticalDirection]);
      EXPECT_EQ(50, shift_list[kLeftDiagonalDirection]);
      EXPECT_EQ(50, shift_list[kRightDiagonalDirection]);
    }    
  }

  void GetBitBoardIndexTest(){
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

  void GetBitBoardShiftTest(){
    const Board board;

    for(size_t i=0; i<1024; i++){
      const size_t shift_val = 2 * (i % 32);
      EXPECT_EQ(shift_val, board.GetBitBoardShift(i));
    }
  }
};

TEST_F(BoardTest, DefaultConstructorTest){
  DefaultConstructorTest();
}

TEST_F(BoardTest, GetSetStateTest)
{
  GetSetStateTest();
}

TEST_F(BoardTest, GetBitBoardIndexTest){
  GetBitBoardIndexTest();
}

TEST_F(BoardTest, GetBitBoardShiftTest){
  GetBitBoardShiftTest();
}

TEST_F(BoardTest, GetBitBoardIndexListTest){
  GetBitBoardIndexListTest();
}

TEST_F(BoardTest, GetBitBoardShiftListTest){
  GetBitBoardShiftListTest();
}

TEST_F(BoardTest, IsInBoardTest)
{
  EXPECT_FALSE(IsInBoard(1, 0));
  EXPECT_FALSE(IsInBoard(0, 1));
  EXPECT_TRUE(IsInBoard(1, 1));
  EXPECT_TRUE(IsInBoard(15, 15));
}

}   // namespace realcore
