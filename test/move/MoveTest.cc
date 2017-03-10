#include "gtest/gtest.h"

#include "Move.h"

using namespace realcore;

TEST(MoveTest, GetAllMoveTest)
{
  const auto &all_move_list = GetAllMove();

  EXPECT_EQ(kMoveNum, all_move_list.size());
  EXPECT_EQ(kNullMove, all_move_list[0]);
  EXPECT_EQ(kDeclareEndGame, all_move_list[1]);
  EXPECT_EQ(kInvalidMove, all_move_list[2]);
  EXPECT_EQ(kUndefinedMove01, all_move_list[3]);  
  EXPECT_EQ(kMoveAA, all_move_list[17]);
  EXPECT_EQ(kMoveOO, all_move_list[kMoveNum - 1]);
}

TEST(MoveTest, GetAllValidMoveTest)
{
  const auto &all_valid_move_list = GetAllValidMove();

  EXPECT_EQ(kValidMoveNum, all_valid_move_list.size());
  EXPECT_EQ(kMoveAA, all_valid_move_list[0]);
  EXPECT_EQ(kMoveBA, all_valid_move_list[1]);
  EXPECT_EQ(kMoveOO, all_valid_move_list[kValidMoveNum - 2]);
  EXPECT_EQ(kNullMove, all_valid_move_list[kValidMoveNum - 1]);
}

TEST(MoveTest, GetAllInBoardMoveTest)
{
  const auto &all_in_board_move_list = GetAllInBoardMove();

  EXPECT_EQ(kInBoardMoveNum, all_in_board_move_list.size());
  EXPECT_EQ(kMoveAA, all_in_board_move_list[0]);
  EXPECT_EQ(kMoveBA, all_in_board_move_list[1]);
  EXPECT_EQ(kMoveOO, all_in_board_move_list[kInBoardMoveNum - 1]);
}

TEST(MoveTest, IsInvalidMoveTest)
{
  const auto &all_move_list = GetAllMove();

  // 全数テスト
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

TEST(MoveTest, IsNullMoveTest)
{
  const auto &all_move_list = GetAllMove();

  // 全数テスト
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

TEST(MoveTest, IsInBoardMoveTest)
{
  const auto &all_in_board_move_list = GetAllInBoardMove();

  // 全数テスト
  for (auto move : all_in_board_move_list)
  {
    EXPECT_TRUE(IsInBoardMove(move));
  }

  EXPECT_FALSE(IsInBoardMove(kInvalidMove));
  EXPECT_FALSE(IsInBoardMove(kDeclareEndGame));
  EXPECT_FALSE(IsInBoardMove(kUndefinedMove01));
  EXPECT_FALSE(IsInBoardMove(kUndefinedMove28));
  EXPECT_FALSE(IsInBoardMove(kNullMove));
}

TEST(MoveTest, IsValidMoveTest)
{
  const auto &all_in_board_move_list = GetAllInBoardMove();

  // 全数テスト
  for (auto move : all_in_board_move_list)
  {
    EXPECT_TRUE(IsValidMove(move));
  }

  EXPECT_FALSE(IsValidMove(kInvalidMove));
  EXPECT_FALSE(IsValidMove(kDeclareEndGame));
  EXPECT_FALSE(IsValidMove(kUndefinedMove01));
  EXPECT_FALSE(IsValidMove(kUndefinedMove28));
  EXPECT_TRUE(IsValidMove(kNullMove));
}

TEST(MoveTest, GetMoveCordinateTest)
{
  {
    // 左端
    constexpr MovePosition move = kMoveAB;
    Cordinate x = 0, y = 0;

    GetMoveCordinate(move, &x, &y);

    EXPECT_EQ(1, x);
    EXPECT_EQ(2, y);
  }
  {
    // 右端
    constexpr MovePosition move = kMoveOE;
    Cordinate x = 0, y = 0;

    GetMoveCordinate(move, &x, &y);

    EXPECT_EQ(15, x);
    EXPECT_EQ(5, y);
  }
  {
    // 上端
    constexpr MovePosition move = kMoveBA;
    Cordinate x = 0, y = 0;

    GetMoveCordinate(move, &x, &y);

    EXPECT_EQ(2, x);
    EXPECT_EQ(1, y);
  }
  {
    // 下端
    constexpr MovePosition move = kMoveEO;
    Cordinate x = 0, y = 0;

    GetMoveCordinate(move, &x, &y);

    EXPECT_EQ(5, x);
    EXPECT_EQ(15, y);
  }
}

TEST(MoveTest, GetMoveTest)
{
  {
    // 左端
    constexpr Cordinate x = 1, y = 2;
    const MovePosition move = GetMove(x, y);
    EXPECT_EQ(kMoveAB, move);
  }
  {
    // 右端
    constexpr Cordinate x = 15, y = 14;
    const MovePosition move = GetMove(x, y);
    EXPECT_EQ(kMoveON, move);
  }
  {
    // 上端
    constexpr Cordinate x = 2, y = 1;
    const MovePosition move = GetMove(x, y);
    EXPECT_EQ(kMoveBA, move);
  }
  {
    // 下端
    constexpr Cordinate x = 2, y = 15;
    const MovePosition move = GetMove(x, y);
    EXPECT_EQ(kMoveBO, move);
  }
}