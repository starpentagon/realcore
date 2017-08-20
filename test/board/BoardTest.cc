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

TEST_F(BoardTest, MakeNonTerminateNormalSequenceTest)
{
  vector<string> test_sequence_list;
    
  test_sequence_list.emplace_back("gghhhgghhigiigffgffghfgjhjifjfjijggeefgkeghehkkgkfdfkhdggdhlgllfhdlg");

  for(const auto &sequence_str : test_sequence_list){
    MoveList move_list(sequence_str);
    ASSERT_FALSE(IsNonTerminateNormalSequence(move_list));

    MoveList modified_list;
    const auto is_modified = MakeNonTerminateNormalSequence(move_list, &modified_list);
    ASSERT_TRUE(is_modified);

    BitBoard before_board(move_list), after_board(modified_list);
    ASSERT_TRUE(before_board == after_board);

    ASSERT_TRUE(IsNonTerminateNormalSequence(modified_list));
  }
}

}   // namespace realcore
