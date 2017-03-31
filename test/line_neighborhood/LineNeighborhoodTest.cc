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
    BitBoard bit_board;
    LineNeighborhood<1> line_neighborhood(kMoveHH, bit_board);

    const StateBit expect_bit = (GetStateBit("OOOXXXXXX") << 32ULL) | GetStateBit("OOOXXXXXX");

    for(auto bit_state : line_neighborhood.local_bit_board_){
      EXPECT_EQ(expect_bit, bit_state);
    }

    EXPECT_EQ(kMoveHH, line_neighborhood.move_);
  }

  void SetCenterStateTest()
  {
    BitBoard bit_board;

    {
      // 黒石設定
      LineNeighborhood<1> line_neighborhood(kMoveHH, bit_board);
      line_neighborhood.SetCenterState<kBlackStone>();
      
      const StateBit expect_bit = (GetStateBit("OBOXXXXXX") << 32ULL) | GetStateBit("OBOXXXXXX");

      for(auto bit_state : line_neighborhood.local_bit_board_){
        EXPECT_EQ(expect_bit, bit_state);
      }
    }
    {
      // 白石設定
      LineNeighborhood<1> line_neighborhood(kMoveHH, bit_board);
      line_neighborhood.SetCenterState<kWhiteStone>();
      
      const StateBit expect_bit = (GetStateBit("OWOXXXXXX") << 32ULL) | GetStateBit("OWOXXXXXX");

      for(auto bit_state : line_neighborhood.local_bit_board_){
        EXPECT_EQ(expect_bit, bit_state);
      }
    }
  }

  void GetBoardDirectionTest()
  {
    BitBoard bit_board;
    LineNeighborhood<1> line_neighborhood(kMoveHH, bit_board);
    
    {
      // 横方向
      constexpr BoardDirection expect_direction = kLateralDirection;
      constexpr size_t index = 0;

      for(size_t bit_index=0; bit_index<32; ++bit_index){
        const BoardDirection board_direction = line_neighborhood.GetBoardDirection(index, bit_index);
        EXPECT_EQ(expect_direction, board_direction);
      }
    }
    {
      // 縦方向
      constexpr BoardDirection expect_direction = kVerticalDirection;
      constexpr size_t index = 0;

      for(size_t bit_index=32; bit_index<64; ++bit_index){
        const BoardDirection board_direction = line_neighborhood.GetBoardDirection(index, bit_index);
        EXPECT_EQ(expect_direction, board_direction);
      }
    }
    {
      // 左斜め方向
      constexpr BoardDirection expect_direction = kLeftDiagonalDirection;
      constexpr size_t index = 1;

      for(size_t bit_index=0; bit_index<32; ++bit_index){
        const BoardDirection board_direction = line_neighborhood.GetBoardDirection(index, bit_index);
        EXPECT_EQ(expect_direction, board_direction);
      }
    }
    {
      // 右斜め方向
      constexpr BoardDirection expect_direction = kRightDiagonalDirection;
      constexpr size_t index = 1;

      for(size_t bit_index=32; bit_index<64; ++bit_index){
        const BoardDirection board_direction = line_neighborhood.GetBoardDirection(index, bit_index);
        EXPECT_EQ(expect_direction, board_direction);
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

TEST_F(LineNeighborhoodTest, GetBoardDirectionTest)
{
  GetBoardDirectionTest();
}

}   // namespace realcore
