#include "gtest/gtest.h"

#include "Move.h"
#include "LineNeighborhood.h"

using namespace std;

namespace realcore
{
class LineNeighborhoodTest
: public ::testing::Test
{
public:
  void DefaultConstructorTest(){
    Board board;
    LineNeighborhood<1> line_neighborhood(kMoveHH, board);

    const StateBit expect_bit = (GetStateBit("OOOXXXXXX") << 32ULL) | GetStateBit("OOOXXXXXX");

    for(auto bit_state : line_neighborhood.local_bit_board_){
      EXPECT_EQ(expect_bit, bit_state);
    }
  }

  void SetCenterStateTest()
  {
    Board board;

    {
      // 黒石設定
      LineNeighborhood<1> line_neighborhood(kMoveHH, board);
      line_neighborhood.SetCenterState<kBlackStone>();
      
      const StateBit expect_bit = (GetStateBit("OBOXXXXXX") << 32ULL) | GetStateBit("OBOXXXXXX");

      for(auto bit_state : line_neighborhood.local_bit_board_){
        EXPECT_EQ(expect_bit, bit_state);
      }
    }
    {
      // 白石設定
      LineNeighborhood<1> line_neighborhood(kMoveHH, board);
      line_neighborhood.SetCenterState<kWhiteStone>();
      
      const StateBit expect_bit = (GetStateBit("OWOXXXXXX") << 32ULL) | GetStateBit("OWOXXXXXX");

      for(auto bit_state : line_neighborhood.local_bit_board_){
        EXPECT_EQ(expect_bit, bit_state);
      }
    }
  }
};

TEST_F(LineNeighborhoodTest, DefaultConstructorTest)
{
  DefaultConstructorTest();
}

TEST_F(LineNeighborhoodTest, SetCenterStateTest)
{
  SetCenterStateTest();
}

}   // namespace realcore
