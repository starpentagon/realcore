#include "gtest/gtest.h"

#include "MoveList.h"
#include "OpenState.h"
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

  void GetBoardPositionListTest()
  {
    BitBoard bit_board;
    LineNeighborhood<7> line_neighborhood(kMoveHH, bit_board);
    LocalBitBoard bit_list{{0}};

    bit_list[0] = (0b1ULL << 0) | (0b01ULL << 34);
    bit_list[1] = (0b1ULL << 2) | (0b01ULL << 36);

    vector<BoardPosition> board_position_list;
    line_neighborhood.GetBoardPositionList(bit_list, &board_position_list);

    ASSERT_EQ(4, board_position_list.size());

    ASSERT_EQ(129, board_position_list[0]);
    ASSERT_EQ(386, board_position_list[1]);
    ASSERT_EQ(737, board_position_list[2]);
    ASSERT_EQ(994, board_position_list[3]);
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

TEST_F(LineNeighborhoodTest, GetBoardPositionListTest)
{
  GetBoardPositionListTest();
}

TEST_F(LineNeighborhoodTest, GetOpenStateOverlineTest)
{
  // kNextOverline
  //   A B C D E F G H I J K L M N O 
  // A + --------------------------+ A 
  // B | . . . . . . . . . . . . . | B 
  // C | . . . . . . . . . . . . . | C 
  // D | . . * . . . . . . . * . . | D 
  // E | . . . . . . . . . . . . . | E 
  // F | . . . . . . . . . . . . . | F 
  // G | . . . . . . o . . . . o . | G 
  // H | . . . . . o x x x . x x o | H 
  // I | . . . . . . . . . . . . . | I 
  // J | . . . . . . . . . . . . . | J 
  // K | . . . . . . . . . . . . . | K 
  // L | . . * . . . . . . . * . . | L 
  // M | . . . . . . . . . . . . . | M 
  // N | . . . . . . . . . . . . . | N 
  // O + --------------------------+ O 
  //   A B C D E F G H I J K L M N O 
  constexpr OpenStatePattern kPattern = kNextOverline;
  constexpr PlayerTurn P = GetPatternPlayerTurn(kPattern);
  BitBoard bit_board(MoveList("hhhgihghmhnhlhmgjh"));
  constexpr MovePosition move = kMoveJH;
  LineNeighborhood<kOpenStateNeighborhoodSize> line_neighborhood(move, bit_board);

  vector< OpenState<kPattern> > open_state_list, expect_list;
  line_neighborhood.GetOpenState<kPattern, P>(&open_state_list);

  const BoardDirection direction = kLateralDirection;
  const BoardPosition open_position = GetBoardPosition(kMoveKH, direction);
  const BoardPosition pattern_position = GetBoardPosition(kMoveIH, direction);
  OpenState<kPattern> expect(open_position, pattern_position);

  ASSERT_EQ(1, open_state_list.size());
  EXPECT_TRUE(expect == open_state_list[0]);
}

TEST_F(LineNeighborhoodTest, GetOpenStateOpenFourBlackTest)
{
  // kNextOpenFourBlack
  //   A B C D E F G H I J K L M N O 
  // A + --------------------------+ A 
  // B | . . . . . . . . . . . . . | B 
  // C | . . . . . . . . . . . . . | C 
  // D | . . * . . . . . . . * . . | D 
  // E | . . . . . . . . . . . . . | E 
  // F | . . . . . . . . . . . . . | F 
  // G | . . . . . . o o . . . . . | G 
  // H | . . . . . . x x x . . . . | H 
  // I | . . . . . . . . . . . . . | I 
  // J | . . . . . . . . . . . . . | J 
  // K | . . . . . . . . . . . . . | K 
  // L | . . * . . . . . . . * . . | L 
  // M | . . . . . . . . . . . . . | M 
  // N | . . . . . . . . . . . . . | N 
  // O + --------------------------+ O 
  //   A B C D E F G H I J K L M N O 
  constexpr OpenStatePattern kPattern = kNextOpenFourBlack;
  constexpr PlayerTurn P = GetPatternPlayerTurn(kPattern);
  BitBoard bit_board(MoveList("hhhgihigjh"));
  constexpr MovePosition move = kMoveJH;
  LineNeighborhood<kOpenStateNeighborhoodSize> line_neighborhood(move, bit_board);

  vector< OpenState<kPattern> > open_state_list, expect_list;
  line_neighborhood.GetOpenState<kPattern, P>(&open_state_list);

  const BoardDirection direction = kLateralDirection;

  const BoardPosition open_position_1 = GetBoardPosition(kMoveGH, direction);
  const BoardPosition pattern_position_1 = GetBoardPosition(kMoveGH, direction);
  OpenState<kPattern> expect_1(open_position_1, pattern_position_1);

  const BoardPosition open_position_2 = GetBoardPosition(kMoveKH, direction);
  const BoardPosition pattern_position_2 = GetBoardPosition(kMoveKH, direction);
  OpenState<kPattern> expect_2(open_position_2, pattern_position_2);

  ASSERT_EQ(2, open_state_list.size());
  EXPECT_TRUE(expect_1 == open_state_list[0]);
  EXPECT_TRUE(expect_2 == open_state_list[1]);
}

}   // namespace realcore
