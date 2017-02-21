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

TEST_F(MoveListTest, CalcInitialReserveSizeTest)
{
  CalcInitialReserveSizeTest();
}

}   // namespace realcore


