// @brief メンバ関数のテスト
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
  void DefaultConstructorTest(){
    const Board board;
    const BitBoard bit_board;
    const MoveList move_list;

    EXPECT_TRUE(board.bit_board_ == bit_board);
    EXPECT_TRUE(board.move_list_ == move_list);
  }

  void CopyConstructorTest(){
    Board board_1;
    board_1.MakeMove(kMoveHH);

    const Board board_2(board_1);
    EXPECT_TRUE(board_1.bit_board_ == board_2.bit_board_);
    EXPECT_TRUE(board_1.move_list_ == board_2.move_list_);
  }

  void MoveListConstructorTest(){
    const MoveList board_move_list("hhhg");
    const Board board(board_move_list);

    Board board_expect;
    board_expect.MakeMove(kMoveHH);
    board_expect.MakeMove(kMoveHG);

    EXPECT_TRUE(board.bit_board_ == board_expect.bit_board_);
    EXPECT_TRUE(board.move_list_ == board_expect.move_list_);
  }
};

TEST_F(BoardTest, DefaultConstructorTest){
  DefaultConstructorTest();
}

TEST_F(BoardTest, CopyConstructorTest){
  CopyConstructorTest();
}

TEST_F(BoardTest, MoveListConstructorTest){
  MoveListConstructorTest();
}

TEST_F(BoardTest, CompareOperTest){
  Board board_1, board_2;

  EXPECT_TRUE(board_1 == board_1);
  EXPECT_TRUE(board_1 == board_2);

  board_1.MakeMove(kMoveHH);
  EXPECT_TRUE(board_1 == board_1);
  EXPECT_TRUE(board_1 != board_2);
  
  board_2.MakeMove(kMoveHH);
  EXPECT_TRUE(board_1 == board_2);
}

TEST_F(BoardTest, AssignOperTest){
  Board board_1, board_2;

  board_1.MakeMove(kMoveHH);
  EXPECT_TRUE(board_1 != board_2);

  board_2 = board_1;
  EXPECT_TRUE(board_1 == board_2);
}
}
