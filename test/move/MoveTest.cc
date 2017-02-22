#include "gtest/gtest.h"

#include "Move.h"

using namespace realcore;

TEST(MoveTest, GetAllMove)
{
  const auto &all_move_list = GetAllMove();

  EXPECT_EQ(kMoveNum, all_move_list.size());
  EXPECT_EQ(kInvalidMove, all_move_list[0]);
  EXPECT_EQ(kNullMove, all_move_list[1]);
  EXPECT_EQ(kMoveOO, all_move_list[kMoveNum - 1]);
}

TEST(MoveTest, GetAllValidMove)
{
  const auto &all_valid_move_list = GetAllValidMove();

  EXPECT_EQ(kValidMoveNum, all_valid_move_list.size());
  EXPECT_EQ(kMoveAA, all_valid_move_list[0]);
  EXPECT_EQ(kMoveBA, all_valid_move_list[1]);
  EXPECT_EQ(kNullMove, all_valid_move_list[kValidMoveNum - 1]);
}

TEST(MoveTest, GetAllInBoardMove)
{
  const auto &all_in_board_move_list = GetAllInBoardMove();

  EXPECT_EQ(kInBoardMoveNum, all_in_board_move_list.size());
  EXPECT_EQ(kMoveAA, all_in_board_move_list[0]);
  EXPECT_EQ(kMoveBA, all_in_board_move_list[1]);
  EXPECT_EQ(kMoveOO, all_in_board_move_list[kInBoardMoveNum - 1]);
}

TEST(MoveTest, IsInvalidMove)
{
  const auto &all_move_list = GetAllMove();

  for (auto move : all_move_list)
  {
    if (move == kInvalidMove)
    {
      EXPECT_TRUE(IsInvalidMove(move));
    }
    else
    {
      EXPECT_FALSE(IsInvalidMove(move));
    }
  }
}

TEST(MoveTest, IsNullMove)
{
  const auto &all_move_list = GetAllMove();

  for (auto move : all_move_list)
  {
    if (move == kNullMove)
    {
      EXPECT_TRUE(IsNullMove(move));
    }
    else
    {
      EXPECT_FALSE(IsNullMove(move));
    }
  }
}

TEST(MoveTest, IsInBoardMove)
{
  const auto &all_in_board_move_list = GetAllInBoardMove();

  for (auto move : all_in_board_move_list)
  {
    EXPECT_TRUE(IsInBoardMove(move));
  }

  EXPECT_FALSE(IsInBoardMove(kInvalidMove));
  EXPECT_FALSE(IsInBoardMove(kNullMove));
}

TEST(MoveTest, IsValidMove)
{
  const auto &all_in_board_move_list = GetAllInBoardMove();

  for (auto move : all_in_board_move_list)
  {
    EXPECT_TRUE(IsValidMove(move));
  }

  EXPECT_FALSE(IsValidMove(kInvalidMove));
  EXPECT_TRUE(IsValidMove(kNullMove));
}

TEST(MoveTest, GetMoveCordinate)
{
  {
    constexpr MovePosition move = kMoveAB;
    Cordinate x = 0, y = 0;

    GetMoveCordinate(move, &x, &y);

    EXPECT_EQ(1, x);
    EXPECT_EQ(2, y);
  }
  {
    constexpr MovePosition move = kMoveEO;
    Cordinate x = 0, y = 0;

    GetMoveCordinate(move, &x, &y);

    EXPECT_EQ(5, x);
    EXPECT_EQ(15, y);
  }
}

TEST(MoveTest, GetMove)
{
  {
    constexpr Cordinate x = 1, y = 2;
    const MovePosition move = GetMove(x, y);
    EXPECT_EQ(kMoveAB, move);
  }
  {
    constexpr Cordinate x = 15, y = 14;
    const MovePosition move = GetMove(x, y);
    EXPECT_EQ(kMoveON, move);
  }
}