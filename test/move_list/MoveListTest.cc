#include <array>

#include "gtest/gtest.h"

#include "Move.h"
#include "MoveList.h"

using namespace std;

namespace realcore
{
class MoveListTest
: public ::testing::Test
{
public:
  void DefaultConstructorTest(){
    MoveList move_list;
    
    EXPECT_TRUE(move_list.move_list_.empty());

    static const int initial_reserve_size = move_list.CalcInitialReserveSize(0);
    EXPECT_EQ(initial_reserve_size, move_list.move_list_.capacity());
  }

  void ReserveInitialTest()
  {
    MoveList move_list;

    constexpr size_t initial_size = 8;
    move_list.ReserveInitial(initial_size);
    static const int initial_reserve_size = move_list.CalcInitialReserveSize(initial_size);

    EXPECT_EQ(initial_reserve_size, move_list.move_list_.capacity());
  }

  void CalcInitialReserveSizeTest()
  {
    // 境界値テスト
    MoveList move_list;
    const array<size_t, 8> val_list{{0, 1, 2, 3, 4, 5, 6, 7}};
    
    for(const size_t val : val_list){
      EXPECT_EQ(24, move_list.CalcInitialReserveSize(val));
    }

    for(const size_t val : val_list){
      EXPECT_EQ(32, move_list.CalcInitialReserveSize(8 + val));
    }
  }
};

TEST_F(MoveListTest, DefaultConstructorTest)
{
  DefaultConstructorTest();
}

TEST_F(MoveListTest, StringConstructorTest)
{
  MoveList move_list("aabb");
  MoveList expect_list;

  expect_list += kMoveAA;
  expect_list += kMoveBB;

  EXPECT_TRUE(move_list == expect_list);
}

TEST_F(MoveListTest, ReserveInitialTest)
{
  ReserveInitialTest();
}

TEST_F(MoveListTest, CopyConstructorMoveListTest)
{
  MoveList move_list;
  move_list += kMoveAA;
  move_list += kMoveAB;

  // MoveListのコピーをテストする
  MoveList copied_list(move_list);

  EXPECT_TRUE(move_list == copied_list);
}

TEST_F(MoveListTest, CopyConstructorMovePositionTest)
{
  MoveList move_list;
  move_list += kMoveAA;

  // MovePositionのコピーをテストする
  MoveList copied_list(kMoveAA);

  EXPECT_TRUE(move_list == copied_list);  
}

TEST_F(MoveListTest, ConcatOperTest)
{
  MoveList move_list;

  // MovePosition版
  move_list += kNullMove;

  EXPECT_EQ(1, move_list.size());
  EXPECT_EQ(kNullMove, move_list[0]);

  // MoveList版
  move_list += move_list;

  EXPECT_EQ(2, move_list.size());
  EXPECT_EQ(kNullMove, move_list[0]);
  EXPECT_EQ(kNullMove, move_list[1]);
}

TEST_F(MoveListTest, AssignOperTest)
{
  MoveList list_1, list_2;

  list_1 += kMoveAA;

  EXPECT_TRUE(list_1 != list_2);

  // MoveList版
  list_2 = list_1;
  EXPECT_TRUE(list_1 == list_2);

  // MovePosition版
  MoveList list_3;
  list_3 = kMoveAA;

  EXPECT_TRUE(list_1 == list_3);
}

TEST_F(MoveListTest, CompareOperTest)
{
  MoveList list_1, list_2;

  EXPECT_TRUE(list_1 == list_2);
  EXPECT_FALSE(list_1 != list_2);

  list_1 += kMoveAA;
  EXPECT_FALSE(list_1 == list_2);
  EXPECT_TRUE(list_1 != list_2);

  list_2 += kMoveAA;
  EXPECT_TRUE(list_1 == list_2);
  EXPECT_FALSE(list_1 != list_2);
}

TEST_F(MoveListTest, SubscriptOperTest)
{
  MoveList move_list;

  // 右辺値
  move_list += kMoveAA;
  EXPECT_EQ(kMoveAA, move_list[0]);

  // 左辺値
  move_list[0] = kMoveAB;
  EXPECT_EQ(kMoveAB, move_list[0]);
}

TEST_F(MoveListTest, DecrementOperTest)
{
  MoveList move_list;

  move_list += kMoveAA;
  EXPECT_FALSE(move_list.empty());

  --move_list;
  EXPECT_TRUE(move_list.empty());
}

TEST_F(MoveListTest, GetLastMoveTest)
{
  MoveList move_list;
  EXPECT_EQ(kInvalidMove, move_list.GetLastMove());

  move_list += kMoveAA;
  EXPECT_EQ(kMoveAA, move_list.GetLastMove());

  move_list += kMoveAB;
  EXPECT_EQ(kMoveAB, move_list.GetLastMove());

  --move_list;
  EXPECT_EQ(kMoveAA, move_list.GetLastMove());

  --move_list;
  EXPECT_EQ(kInvalidMove, move_list.GetLastMove());
}

TEST_F(MoveListTest, CalcInitialReserveSizeTest)
{
  CalcInitialReserveSizeTest();
}

TEST_F(MoveListTest, sizeTest)
{
  MoveList move_list;
  EXPECT_EQ(0, move_list.size());

  move_list += kNullMove;
  EXPECT_EQ(1, move_list.size());
}

TEST_F(MoveListTest, clearTest)
{
  MoveList move_list;

  move_list += kNullMove;
  move_list.clear();

  MoveList empty;
  EXPECT_TRUE(move_list == empty);  
}

TEST_F(MoveListTest, emptyTest)
{
  MoveList move_list;
  EXPECT_TRUE(move_list.empty());

  move_list += kNullMove;
  EXPECT_FALSE(move_list.empty());

  move_list.clear();
  EXPECT_TRUE(move_list.empty());
}

TEST_F(MoveListTest, strTest)
{
  MoveList move_list;

  EXPECT_EQ("", move_list.str());

  move_list += kMoveAB;
  EXPECT_EQ("ab", move_list.str());

  move_list += kMoveHH;
  EXPECT_EQ("abhh", move_list.str());

  move_list += kNullMove;
  EXPECT_EQ("abhhpp", move_list.str());
}

TEST_F(MoveListTest, GetMoveListTest)
{
  {
    // 空文字のケースをテスト
    MoveList move_list;
    bool is_generate = GetMoveList("", &move_list);
    EXPECT_TRUE(is_generate);

    MoveList check;
    EXPECT_TRUE(move_list == check);
  }
  {
    // 1手のみのテスト
    MoveList move_list;
    bool is_generate = GetMoveList("aa", &move_list);
    EXPECT_TRUE(is_generate);

    MoveList check;
    check += kMoveAA;
    EXPECT_TRUE(move_list == check);
  }
  {
    // Passを含む場合をテスト
    MoveList move_list;
    bool is_generate = GetMoveList("abhhpp", &move_list);
    EXPECT_TRUE(is_generate);

    MoveList check;
    check += kMoveAB;
    check += kMoveHH;
    check += kNullMove;
    EXPECT_TRUE(move_list == check);
  }
  {
    // 異常系(奇数文字数)
    MoveList move_list;
    bool is_generate = GetMoveList("abh", &move_list);  
    EXPECT_FALSE(is_generate);
    EXPECT_TRUE(move_list.empty());
  }
  {
    // 異常系(定義外文字)
    MoveList move_list;
    bool is_generate = GetMoveList("abaz", &move_list);  
    EXPECT_FALSE(is_generate);
    EXPECT_TRUE(move_list.empty());
  }
}

TEST_F(MoveListTest, GetSymmetricMoveListTest)
{
  MoveList move_list("bchhpp");

  {
    // kIdenticalSymmetry
    MoveList symmetric_move_list, expect_list("bchhpp");
    GetSymmetricMoveList(move_list, kIdenticalSymmetry, &symmetric_move_list);
    EXPECT_TRUE(symmetric_move_list == expect_list);
  }
  {
    // kHorizontalSymmetry
    MoveList symmetric_move_list, expect_list("nchhpp");
    GetSymmetricMoveList(move_list, kHorizontalSymmetry, &symmetric_move_list);
    EXPECT_TRUE(symmetric_move_list == expect_list);
  }
  {
    // kVerticalSymmetry
    MoveList symmetric_move_list, expect_list("bmhhpp");
    GetSymmetricMoveList(move_list, kVerticalSymmetry, &symmetric_move_list);
    EXPECT_TRUE(symmetric_move_list == expect_list);
  }
  {
    // kCentricSymmetry
    MoveList symmetric_move_list, expect_list("nmhhpp");
    GetSymmetricMoveList(move_list, kCentricSymmetry, &symmetric_move_list);
    EXPECT_TRUE(symmetric_move_list == expect_list);
  }
  {
    // kDiagonalSymmetry1
    MoveList symmetric_move_list, expect_list("cbhhpp");
    GetSymmetricMoveList(move_list, kDiagonalSymmetry1, &symmetric_move_list);
    EXPECT_TRUE(symmetric_move_list == expect_list);
  }
  {
    // kDiagonalSymmetry2
    MoveList symmetric_move_list, expect_list("mbhhpp");
    GetSymmetricMoveList(move_list, kDiagonalSymmetry2, &symmetric_move_list);
    EXPECT_TRUE(symmetric_move_list == expect_list);
  }
  {
    // kDiagonalSymmetry3
    MoveList symmetric_move_list, expect_list("cnhhpp");
    GetSymmetricMoveList(move_list, kDiagonalSymmetry3, &symmetric_move_list);
    EXPECT_TRUE(symmetric_move_list == expect_list);
  }
  {
    // kDiagonalSymmetry4
    MoveList symmetric_move_list, expect_list("mnhhpp");
    GetSymmetricMoveList(move_list, kDiagonalSymmetry4, &symmetric_move_list);
    EXPECT_TRUE(symmetric_move_list == expect_list);
  }
}

TEST_F(MoveListTest, IsBlackTurnTest)
{
  MoveList move_list;
  EXPECT_TRUE(move_list.IsBlackTurn());

  move_list += kMoveHH;
  EXPECT_FALSE(move_list.IsBlackTurn());
}

TEST_F(MoveListTest, GetMoveBitSetListTest)
{
  MoveBitSet move_bit_set;

  move_bit_set.set(kMoveAA);
  move_bit_set.set(kMoveHH);
  move_bit_set.set(kMoveOO);
  move_bit_set.set(kNullMove);

  MoveList move_list;
  GetMoveList(move_bit_set, &move_list);

  ASSERT_EQ(4, move_list.size());
  EXPECT_EQ(kNullMove, move_list[0]);
  EXPECT_EQ(kMoveAA, move_list[1]);
  EXPECT_EQ(kMoveHH, move_list[2]);
  EXPECT_EQ(kMoveOO, move_list[3]);
}

TEST_F(MoveListTest, GetOpenMoveTest)
{
  MoveList move_list;
  move_list += kMoveHH;

  MoveList open_move_list;
  move_list.GetOpenMove(&open_move_list);

  ASSERT_EQ(kInBoardMoveNum - 1, open_move_list.size());

  for(const auto move : GetAllInBoardMove()){
    auto find_it = find(open_move_list.begin(), open_move_list.end(), move);

    if(move == kMoveHH){
      ASSERT_TRUE(find_it == open_move_list.end());
    }else{
      ASSERT_FALSE(find_it == open_move_list.end());
    }
  }
}

TEST_F(MoveListTest, GetOpenMoveForbiddenTest)
{
  MoveList move_list;
  move_list += kMoveHH;

  MoveList open_move_list;
  MoveBitSet forbidden_bit;
  forbidden_bit.set(kMoveHG);

  move_list.GetOpenMove(forbidden_bit, &open_move_list);

  ASSERT_EQ(kInBoardMoveNum - 2, open_move_list.size());

  for(const auto move : GetAllInBoardMove()){
    auto find_it = find(open_move_list.begin(), open_move_list.end(), move);

    if(move == kMoveHH || move == kMoveHG){
      ASSERT_TRUE(find_it == open_move_list.end());
    }else{
      ASSERT_FALSE(find_it == open_move_list.end());
    }
  }
}

TEST_F(MoveListTest, GetPossibleMoveTest)
{
  MoveList move_list;
  move_list += kMoveHH;

  MoveList possible_move_list;
  move_list.GetPossibleMove(&possible_move_list);

  ASSERT_EQ(kInBoardMoveNum, possible_move_list.size());

  for(const auto move : GetAllInBoardMove()){
    auto find_it = find(possible_move_list.begin(), possible_move_list.end(), move);

    if(move == kMoveHH){
      ASSERT_TRUE(find_it == possible_move_list.end());
    }else{
      ASSERT_FALSE(find_it == possible_move_list.end());
    }
  }

  auto find_it = find(possible_move_list.begin(), possible_move_list.end(), kNullMove);
  ASSERT_FALSE(find_it == possible_move_list.end());
}

TEST_F(MoveListTest, GetPossibleMoveForbiddenTest)
{
  MoveList move_list;
  move_list += kMoveHH;

  MoveList possible_move_list;
  MoveBitSet forbidden_bit;
  forbidden_bit.set(kMoveHG);

  move_list.GetPossibleMove(forbidden_bit, &possible_move_list);

  ASSERT_EQ(kInBoardMoveNum - 1, possible_move_list.size());

  for(const auto move : GetAllInBoardMove()){
    auto find_it = find(possible_move_list.begin(), possible_move_list.end(), move);

    if(move == kMoveHH || move == kMoveHG){
      ASSERT_TRUE(find_it == possible_move_list.end());
    }else{
      ASSERT_FALSE(find_it == possible_move_list.end());
    }
  }

  auto find_it = find(possible_move_list.begin(), possible_move_list.end(), kNullMove);
  ASSERT_FALSE(find_it == possible_move_list.end());
}

TEST_F(MoveListTest, GetInBoardMoveBitSetTest)
{
  const auto& move_bit_set = GetInBoardMoveBitSet();

  for(const auto move : GetAllMove()){
    if(IsInBoardMove(move)){
      ASSERT_TRUE(move_bit_set[move]);
    }else{
      ASSERT_FALSE(move_bit_set[move]);
    }
  }
}

TEST_F(MoveListTest, SortByNearMoveTest)
{
  MoveList test_list;
  
  test_list += kMoveHH;
  test_list += kMoveAA;
  test_list += kMoveOO;

  SortByNearMove(kMoveBB, &test_list);

  ASSERT_EQ(kMoveAA, test_list[0]);
  ASSERT_EQ(kMoveHH, test_list[1]);
  ASSERT_EQ(kMoveOO, test_list[2]);
}
}   // namespace realcore


