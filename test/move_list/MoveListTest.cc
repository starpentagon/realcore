#include <array>

#include "gtest/gtest.h"

#include "MoveList.h"

using namespace std;

namespace realcore
{
class MoveListTest
: public ::testing::Test
{
public:
  void DefaultConstructor(){
    MoveList move_list;
    
    EXPECT_TRUE(move_list.move_list_.empty());
    EXPECT_EQ(24, move_list.move_list_.capacity());
  }

  void CalcInitialReserveSizeTest()
  {
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

TEST_F(MoveListTest, DefaultConstructor)
{
  DefaultConstructor();
}

TEST_F(MoveListTest, CopyConstructorMoveList)
{
  MoveList move_list;
  move_list += kMoveAA;
  move_list += kMoveAB;

  MoveList copied_list(move_list);

  EXPECT_TRUE(move_list == copied_list);
}

TEST_F(MoveListTest, CopyConstructorMovePosition)
{
  MoveList move_list;
  move_list += kMoveAA;

  MoveList copied_list(kMoveAA);

  EXPECT_TRUE(move_list == copied_list);  
}

TEST_F(MoveListTest, ConcatOper)
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

TEST_F(MoveListTest, AssignOper)
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

TEST_F(MoveListTest, CompareOper)
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

TEST_F(MoveListTest, SubscriptOper)
{
  MoveList move_list;

  // 右辺値
  move_list += kMoveAA;
  EXPECT_EQ(kMoveAA, move_list[0]);

  // 左辺値
  move_list[0] = kMoveAB;
  EXPECT_EQ(kMoveAB, move_list[0]);
}

TEST_F(MoveListTest, CalcInitialReserveSizeTest)
{
  CalcInitialReserveSizeTest();
}

TEST_F(MoveListTest, size)
{
  MoveList move_list;
  EXPECT_EQ(0, move_list.size());

  move_list += kNullMove;
  EXPECT_EQ(1, move_list.size());
}

TEST_F(MoveListTest, clear)
{
  MoveList move_list;

  move_list += kNullMove;
  move_list.clear();

  MoveList empty;
  EXPECT_TRUE(move_list == empty);  
}

TEST_F(MoveListTest, empty)
{
  MoveList move_list;
  EXPECT_TRUE(move_list.empty());

  move_list += kNullMove;
  EXPECT_FALSE(move_list.empty());

  move_list.clear();
  EXPECT_TRUE(move_list.empty());
}

TEST_F(MoveListTest, str)
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

TEST_F(MoveListTest, GetMoveList)
{
  {
    MoveList move_list;
    bool is_generate = GetMoveList("", &move_list);
    EXPECT_TRUE(is_generate);

    MoveList check;
    EXPECT_TRUE(move_list == check);
  }
  {
    MoveList move_list;
    bool is_generate = GetMoveList("aa", &move_list);
    EXPECT_TRUE(is_generate);

    MoveList check;
    check += kMoveAA;
    EXPECT_TRUE(move_list == check);
  }
  {
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

}   // namespace realcore


