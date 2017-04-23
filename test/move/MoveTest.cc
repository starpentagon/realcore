#include "gtest/gtest.h"

#include "Move.h"

using namespace std;
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

TEST(MoveTest, GetSymmetricMoveTest)
{
  constexpr MovePosition move = kMoveBC;

  {
    // kIdenticalSymmetry
    const MovePosition symmetric_move = GetSymmetricMove(move, kIdenticalSymmetry);
    EXPECT_EQ(kMoveBC, symmetric_move);
  }
  {
    // kHorizontalSymmetry
    const MovePosition symmetric_move = GetSymmetricMove(move, kHorizontalSymmetry);
    EXPECT_EQ(kMoveNC, symmetric_move);
  }
  {
    // kVerticalSymmetry
    const MovePosition symmetric_move = GetSymmetricMove(move, kVerticalSymmetry);
    EXPECT_EQ(kMoveBM, symmetric_move);
  }
  {
    // kCentricSymmetry
    const MovePosition symmetric_move = GetSymmetricMove(move, kCentricSymmetry);
    EXPECT_EQ(kMoveNM, symmetric_move);
  }
  {
    // kDiagonalSymmetry1
    const MovePosition symmetric_move = GetSymmetricMove(move, kDiagonalSymmetry1);
    EXPECT_EQ(kMoveCB, symmetric_move);
  }
  {
    // kDiagonalSymmetry2
    const MovePosition symmetric_move = GetSymmetricMove(move, kDiagonalSymmetry2);
    EXPECT_EQ(kMoveMB, symmetric_move);
  }
  {
    // kDiagonalSymmetry3
    const MovePosition symmetric_move = GetSymmetricMove(move, kDiagonalSymmetry3);
    EXPECT_EQ(kMoveCN, symmetric_move);
  }
  {
    // kDiagonalSymmetry4
    const MovePosition symmetric_move = GetSymmetricMove(move, kDiagonalSymmetry4);
    EXPECT_EQ(kMoveMN, symmetric_move);
  }

  // 盤外
  for(const auto move : GetAllMove()){
    if(IsInBoardMove(move)){
      continue;
    }

    for(const auto symmetric : GetBoardSymmetry()){
      const MovePosition symmetric_move = GetSymmetricMove(move, symmetric);
      EXPECT_EQ(move, symmetric_move);
    }
  }
}

TEST(MoveTest, MoveStringTest)
{
  ASSERT_EQ("pp", MoveString(kNullMove));
  ASSERT_EQ("ao", MoveString(kMoveAO));
  ASSERT_EQ("hh", MoveString(kMoveHH));
  ASSERT_EQ("oa", MoveString(kMoveOA));
}

TEST(MoveTest, CalcBoardDistanceTest)
{
  ASSERT_EQ(0, CalcBoardDistance(kMoveHH, kMoveHH));
  ASSERT_EQ(1, CalcBoardDistance(kMoveHH, kMoveHG));
  ASSERT_EQ(14, CalcBoardDistance(kMoveOO, kMoveAA));
  ASSERT_EQ(17, CalcBoardDistance(kMoveHH, kMoveFI));
  ASSERT_EQ(119, CalcBoardDistance(kMoveHA, kMoveOO));
  ASSERT_EQ(225, CalcBoardDistance(kMoveHH, kNullMove));
}

TEST(MoveTest, AscendingSortTest)
{
  vector<MoveValue> move_value_list;

  move_value_list.emplace_back(kMoveHH, 1);
  move_value_list.emplace_back(kMoveHI, 3);
  move_value_list.emplace_back(kMoveHJ, 2);
  move_value_list.emplace_back(kMoveHK, 2);
  move_value_list.emplace_back(kMoveHL, 2);
  move_value_list.emplace_back(kMoveHM, 2);
  move_value_list.emplace_back(kMoveHN, 3);

  AscendingSort(&move_value_list);

  ASSERT_EQ(move_value_list[0].first, kMoveHH);
  ASSERT_EQ(move_value_list[1].first, kMoveHJ);
  ASSERT_EQ(move_value_list[2].first, kMoveHK);
  ASSERT_EQ(move_value_list[3].first, kMoveHL);
  ASSERT_EQ(move_value_list[4].first, kMoveHM);
  ASSERT_EQ(move_value_list[5].first, kMoveHI);
  ASSERT_EQ(move_value_list[6].first, kMoveHN);
}

TEST(MoveTest, DescendingSortTest)
{
  vector<MoveValue> move_value_list;

  move_value_list.emplace_back(kMoveHH, 1);
  move_value_list.emplace_back(kMoveHI, 3);
  move_value_list.emplace_back(kMoveHJ, 2);
  move_value_list.emplace_back(kMoveHK, 2);
  move_value_list.emplace_back(kMoveHL, 2);
  move_value_list.emplace_back(kMoveHM, 2);
  move_value_list.emplace_back(kMoveHN, 3);

  DescendingSort(&move_value_list);

  ASSERT_EQ(move_value_list[0].first, kMoveHI);
  ASSERT_EQ(move_value_list[1].first, kMoveHN);
  ASSERT_EQ(move_value_list[2].first, kMoveHJ);
  ASSERT_EQ(move_value_list[3].first, kMoveHK);
  ASSERT_EQ(move_value_list[4].first, kMoveHL);
  ASSERT_EQ(move_value_list[5].first, kMoveHM);
  ASSERT_EQ(move_value_list[6].first, kMoveHH);
}
