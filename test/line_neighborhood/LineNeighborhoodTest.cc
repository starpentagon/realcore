#include "gtest/gtest.h"

#include "MoveList.h"
#include "OpenState.h"
#include "LineNeighborhood.h"
#include "BitBoard.h"

using namespace std;

namespace realcore
{
class LineNeighborhoodTest
: public ::testing::Test
{
public:
  void DefaultConstructorTest(){
    constexpr size_t distance = 1;
    BitBoard bit_board;
    LineNeighborhood line_neighborhood(kMoveHH, distance, bit_board);

    const StateBit expect_bit = (GetStateBit("OOOXXXXXX") << 32ULL) | GetStateBit("OOOXXXXXX");

    for(auto bit_state : line_neighborhood.local_bit_board_){
      EXPECT_EQ(expect_bit, bit_state);
    }

    EXPECT_EQ(kMoveHH, line_neighborhood.move_);
  }

  void SetCenterStateTest()
  {
    BitBoard bit_board;
    constexpr size_t distance = 1;

    {
      // 黒石設定
      LineNeighborhood line_neighborhood(kMoveHH, distance, bit_board);
      line_neighborhood.SetCenterState<kBlackStone>();
      
      const StateBit expect_bit = (GetStateBit("OBOXXXXXX") << 32ULL) | GetStateBit("OBOXXXXXX");

      for(auto bit_state : line_neighborhood.local_bit_board_){
        EXPECT_EQ(expect_bit, bit_state);
      }
    }
    {
      // 白石設定
      LineNeighborhood line_neighborhood(kMoveHH, distance, bit_board);
      line_neighborhood.SetCenterState<kWhiteStone>();
      
      const StateBit expect_bit = (GetStateBit("OWOXXXXXX") << 32ULL) | GetStateBit("OWOXXXXXX");

      for(auto bit_state : line_neighborhood.local_bit_board_){
        EXPECT_EQ(expect_bit, bit_state);
      }
    }
  }

  void GetBoardDirectionTest()
  {
    constexpr size_t distance = 1;
    
    BitBoard bit_board;
    LineNeighborhood line_neighborhood(kMoveHH, distance, bit_board);
    
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
    constexpr size_t distance = 7;
    
    LineNeighborhood line_neighborhood(kMoveHH, distance, bit_board);
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
  LineNeighborhood line_neighborhood(move, kOpenStateNeighborhoodSize, bit_board);

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
  LineNeighborhood line_neighborhood(move, kOpenStateNeighborhoodSize, bit_board);

  vector< OpenState<kPattern> > open_state_list, expect_list;
  line_neighborhood.GetOpenState<kPattern, P>(&open_state_list);

  const BoardDirection direction = kLateralDirection;

  const BoardPosition open_position_1 = GetBoardPosition(kMoveGH, direction);
  const BoardPosition pattern_position_1 = GetBoardPosition(kMoveGH, direction);
  OpenState<kPattern> expect_1(open_position_1, pattern_position_1);

  const BoardPosition open_position_2 = GetBoardPosition(kMoveKH, direction);
  const BoardPosition pattern_position_2 = GetBoardPosition(kMoveHH, direction);
  OpenState<kPattern> expect_2(open_position_2, pattern_position_2);

  ASSERT_EQ(2, open_state_list.size());
  EXPECT_TRUE(expect_1 == open_state_list[0]);
  EXPECT_TRUE(expect_2 == open_state_list[1]);
}

TEST_F(LineNeighborhoodTest, GetOpenStateOpenFourWhiteTest)
{
  // kNextOpenFourWhite
  //   A B C D E F G H I J K L M N O 
  // A + --------------------------+ A 
  // B | . . . . . . . . . . . . . | B 
  // C | . . . . . . . . . . . . . | C 
  // D | . . * . . . . . . . * . . | D 
  // E | . . . . . . . . . . . . . | E 
  // F | . . . . . . . . . . . . . | F 
  // G | . . . . . . o o o . . . . | G 
  // H | . . . . . . x x x . . . . | H 
  // I | . . . . . . . . . . . . . | I 
  // J | . . . . . . . . . . . . . | J 
  // K | . . . . . . . . . . . . . | K 
  // L | . . * . . . . . . . * . . | L 
  // M | . . . . . . . . . . . . . | M 
  // N | . . . . . . . . . . . . . | N 
  // O + --------------------------+ O 
  //   A B C D E F G H I J K L M N O 
  constexpr OpenStatePattern kPattern = kNextOpenFourWhite;
  constexpr PlayerTurn P = GetPatternPlayerTurn(kPattern);
  BitBoard bit_board(MoveList("hhhgihigjhjg"));
  constexpr MovePosition move = kMoveJG;
  LineNeighborhood line_neighborhood(move, kOpenStateNeighborhoodSize, bit_board);

  vector< OpenState<kPattern> > open_state_list, expect_list;
  line_neighborhood.GetOpenState<kPattern, P>(&open_state_list);

  const BoardDirection direction = kLateralDirection;

  const BoardPosition open_position_1 = GetBoardPosition(kMoveGG, direction);
  const BoardPosition pattern_position_1 = GetBoardPosition(kMoveGG, direction);
  OpenState<kPattern> expect_1(open_position_1, pattern_position_1);

  const BoardPosition open_position_2 = GetBoardPosition(kMoveKG, direction);
  const BoardPosition pattern_position_2 = GetBoardPosition(kMoveHG, direction);
  OpenState<kPattern> expect_2(open_position_2, pattern_position_2);

  ASSERT_EQ(2, open_state_list.size());
  EXPECT_TRUE(expect_1 == open_state_list[0]);
  EXPECT_TRUE(expect_2 == open_state_list[1]);
}

TEST_F(LineNeighborhoodTest, GetOpenStateFourBlackTest)
{
  // kNextFourBlack
  //   A B C D E F G H I J K L M N O 
  // A + --------------------------+ A 
  // B | . . . . . . . . . . . . . | B 
  // C | . . . . . . . . . . . . . | C 
  // D | . . * . . . x . . . * . . | D 
  // E | . . . . . . . . . o . . . | E 
  // F | . . . . . . x . . . . . . | F 
  // G | . . . . . . . o . . . . . | G 
  // H | . . . . . . x . . . . . . | H 
  // I | . . . . . . . . . . . . . | I 
  // J | . . . . . . . . . . . . . | J 
  // K | . . . . . . . . . . . . . | K 
  // L | . . * . . . . . . . * . . | L 
  // M | . . . . . . . . . . . . . | M 
  // N | . . . . . . . . . . . . . | N 
  // O + --------------------------+ O 
  //   A B C D E F G H I J K L M N O 
  constexpr OpenStatePattern kPattern = kNextFourBlack;
  constexpr PlayerTurn P = GetPatternPlayerTurn(kPattern);
  BitBoard bit_board(MoveList("hhighfkehd"));
  constexpr MovePosition move = kMoveHD;
  LineNeighborhood line_neighborhood(move, kOpenStateNeighborhoodSize, bit_board);

  vector< OpenState<kPattern> > open_state_list, expect_list;
  line_neighborhood.GetOpenState<kPattern, P>(&open_state_list);

  const BoardDirection direction = kVerticalDirection;

  const BoardPosition open_position_1 = GetBoardPosition(kMoveHE, direction);
  const BoardPosition pattern_position_1 = GetBoardPosition(kMoveHD, direction);
  OpenState<kPattern> expect_1(open_position_1, pattern_position_1);

  GuardPositionList guard_1{{0}};
  guard_1[0] = GetBoardPosition(kMoveHG, direction);
  expect_1.SetGuardPositionList(guard_1);

  const BoardPosition open_position_2 = GetBoardPosition(kMoveHG, direction);
  const BoardPosition pattern_position_2 = GetBoardPosition(kMoveHD, direction);
  OpenState<kPattern> expect_2(open_position_2, pattern_position_2);

  GuardPositionList guard_2{{0}};
  guard_2[0] = GetBoardPosition(kMoveHE, direction);
  expect_2.SetGuardPositionList(guard_2);

  ASSERT_EQ(2, open_state_list.size());
  EXPECT_TRUE(expect_1 == open_state_list[0]);
  EXPECT_TRUE(expect_2 == open_state_list[1]);
}

TEST_F(LineNeighborhoodTest, GetOpenStateFourWhiteTest)
{
  // kNextFourWhite
  //   A B C D E F G H I J K L M N O 
  // A + --------------------------+ A 
  // B | . . . . . . . . . . . . . | B 
  // C | . . . . . . . . . . . o . | C 
  // D | . . * . . . x . . . * . . | D 
  // E | . . . . . . . . . o . . . | E 
  // F | . . . . . . x . . . . . . | F 
  // G | . . . . . . . o . . . . . | G 
  // H | . . . . . . x . . . . . . | H 
  // I | . . . . . . . . . . . . . | I 
  // J | . . . . . . . . . . . . . | J 
  // K | . . . . . . . . . . . . . | K 
  // L | . . * . . . . . . . * . . | L 
  // M | . . . . . . . . . . . . . | M 
  // N | . . . . . . . . . . . . . | N 
  // O + --------------------------+ O 
  //   A B C D E F G H I J K L M N O 
  constexpr OpenStatePattern kPattern = kNextFourWhite;
  constexpr PlayerTurn P = GetPatternPlayerTurn(kPattern);
  BitBoard bit_board(MoveList("hhighfkehdmc"));
  constexpr MovePosition move = kMoveMC;
  LineNeighborhood line_neighborhood(move, kOpenStateNeighborhoodSize, bit_board);

  vector< OpenState<kPattern> > open_state_list, expect_list;
  line_neighborhood.GetOpenState<kPattern, P>(&open_state_list);

  const BoardDirection direction = kLeftDiagonalDirection;

  const BoardPosition open_position_1 = GetBoardPosition(kMoveLD, direction);
  const BoardPosition pattern_position_1 = GetBoardPosition(kMoveMC, direction);
  OpenState<kPattern> expect_1(open_position_1, pattern_position_1);

  GuardPositionList guard_1{{0}};
  guard_1[0] = GetBoardPosition(kMoveJF, direction);
  expect_1.SetGuardPositionList(guard_1);

  const BoardPosition open_position_2 = GetBoardPosition(kMoveJF, direction);
  const BoardPosition pattern_position_2 = GetBoardPosition(kMoveMC, direction);
  OpenState<kPattern> expect_2(open_position_2, pattern_position_2);

  GuardPositionList guard_2{{0}};
  guard_2[0] = GetBoardPosition(kMoveLD, direction);
  expect_2.SetGuardPositionList(guard_2);

  ASSERT_EQ(2, open_state_list.size());
  EXPECT_TRUE(expect_1 == open_state_list[0]);
  EXPECT_TRUE(expect_2 == open_state_list[1]);
}

TEST_F(LineNeighborhoodTest, GetOpenStateSemiThreeBlackTest)
{
  // kNextSemiThreeBlack
  //   A B C D E F G H I J K L M N O 
  // A + --------------------------+ A 
  // B | . . . . . . . . . . . . . | B 
  // C | . . . . . . . . . . . . . | C 
  // D | . . * . . . . . . . * . . | D 
  // E | . . . . . . . . . . . . . | E 
  // F | . . . . x . . . . . . . . | F 
  // G | . . . . . . . o . . . . . | G 
  // H | . . . . . . x . . . . . . | H 
  // I | . . . . . . . . . . . . . | I 
  // J | . . . . . . . . . . . . . | J 
  // K | . . . . . . . . . . . . . | K 
  // L | . . * . . . . . . . * . . | L 
  // M | . . . . . . . . . . . . . | M 
  // N | . . . . . . . . . . . . . | N 
  // O + --------------------------+ O 
  //   A B C D E F G H I J K L M N O 
  constexpr OpenStatePattern kPattern = kNextSemiThreeBlack;
  constexpr PlayerTurn P = GetPatternPlayerTurn(kPattern);
  BitBoard bit_board(MoveList("hhigff"));
  constexpr MovePosition move = kMoveFF;
  LineNeighborhood line_neighborhood(move, kOpenStateNeighborhoodSize, bit_board);

  vector< OpenState<kPattern> > open_state_list, expect_list;
  line_neighborhood.GetOpenState<kPattern, P>(&open_state_list);

  const BoardDirection direction = kRightDiagonalDirection;

  // OpenState at kMoveEE by pattern kMoveEE
  const BoardPosition open_position_1 = GetBoardPosition(kMoveEE, direction);
  const BoardPosition pattern_position_1 = GetBoardPosition(kMoveEE, direction);
  OpenState<kPattern> expect_1(open_position_1, pattern_position_1);

  const auto check_1 = GetBoardPosition(kMoveGG, direction);
  expect_1.SetCheckPosition(check_1);

  GuardPositionList guard_1{{0}};
  guard_1[0] = GetBoardPosition(kMoveGG, direction);
  guard_1[1] = GetBoardPosition(kMoveDD, direction);
  guard_1[2] = GetBoardPosition(kMoveII, direction);
  expect_1.SetGuardPositionList(guard_1);

  // OpenState at kMoveGG by pattern kMoveEE
  const BoardPosition open_position_2 = GetBoardPosition(kMoveGG, direction);
  const BoardPosition pattern_position_2 = GetBoardPosition(kMoveEE, direction);
  OpenState<kPattern> expect_2(open_position_2, pattern_position_2);

  const auto check_2 = GetBoardPosition(kMoveEE, direction);
  expect_2.SetCheckPosition(check_2);

  GuardPositionList guard_2{{0}};
  guard_2[0] = GetBoardPosition(kMoveEE, direction);
  guard_2[1] = GetBoardPosition(kMoveDD, direction);
  guard_2[2] = GetBoardPosition(kMoveII, direction);
  expect_2.SetGuardPositionList(guard_2);

  // OpenState at kMoveGG by pattern kMoveFF
  const BoardPosition open_position_3 = GetBoardPosition(kMoveGG, direction);
  const BoardPosition pattern_position_3 = GetBoardPosition(kMoveFF, direction);
  OpenState<kPattern> expect_3(open_position_3, pattern_position_3);

  const auto check_3 = GetBoardPosition(kMoveII, direction);
  expect_3.SetCheckPosition(check_3);

  GuardPositionList guard_3{{0}};
  guard_3[0] = GetBoardPosition(kMoveII, direction);
  guard_3[1] = GetBoardPosition(kMoveEE, direction);
  guard_3[2] = GetBoardPosition(kMoveJJ, direction);
  expect_3.SetGuardPositionList(guard_3);

  // OpenState at kMoveII by pattern kMoveFF
  const BoardPosition open_position_4 = GetBoardPosition(kMoveII, direction);
  const BoardPosition pattern_position_4 = GetBoardPosition(kMoveFF, direction);
  OpenState<kPattern> expect_4(open_position_4, pattern_position_4);

  const auto check_4 = GetBoardPosition(kMoveGG, direction);
  expect_4.SetCheckPosition(check_4);

  GuardPositionList guard_4{{0}};
  guard_4[0] = GetBoardPosition(kMoveGG, direction);
  guard_4[1] = GetBoardPosition(kMoveEE, direction);
  guard_4[2] = GetBoardPosition(kMoveJJ, direction);
  expect_4.SetGuardPositionList(guard_4);

  ASSERT_EQ(4, open_state_list.size());
  EXPECT_TRUE(expect_1 == open_state_list[0]);
  EXPECT_TRUE(expect_2 == open_state_list[1]);
  EXPECT_TRUE(expect_3 == open_state_list[2]);
  EXPECT_TRUE(expect_4 == open_state_list[3]);
}

TEST_F(LineNeighborhoodTest, GetOpenStateSemiThreeWhiteTest)
{
  // kNextSemiThreeWhite
  //   A B C D E F G H I J K L M N O 
  // A + --------------------------+ A 
  // B | . . . . . . . . . . . . . | B 
  // C | . . . . . . . . . . . . . | C 
  // D | . . * . . . . o . . * . . | D 
  // E | . . . . . . . . . . . . . | E 
  // F | . . . . x . . . . . . . . | F 
  // G | . . . . . . . o . . . . . | G 
  // H | . . . . . . x . . . . . . | H 
  // I | . . . . . . . . . . . . . | I 
  // J | . . . . . . . . . . . . . | J 
  // K | . . . . . . . . . . . . . | K 
  // L | . . * . . . . . . . * . . | L 
  // M | . . . . . . . . . . . . . | M 
  // N | . . . . . . . . . . . . . | N 
  // O + --------------------------+ O 
  //   A B C D E F G H I J K L M N O 
  constexpr OpenStatePattern kPattern = kNextSemiThreeWhite;
  constexpr PlayerTurn P = GetPatternPlayerTurn(kPattern);
  BitBoard bit_board(MoveList("hhigffid"));
  constexpr MovePosition move = kMoveID;
  LineNeighborhood line_neighborhood(move, kOpenStateNeighborhoodSize, bit_board);

  vector< OpenState<kPattern> > open_state_list, expect_list;
  line_neighborhood.GetOpenState<kPattern, P>(&open_state_list);

  const BoardDirection direction = kVerticalDirection;

  // OpenState at kMoveIE by pattern kMoveID
  const BoardPosition open_position_1 = GetBoardPosition(kMoveIE, direction);
  const BoardPosition pattern_position_1 = GetBoardPosition(kMoveID, direction);
  OpenState<kPattern> expect_1(open_position_1, pattern_position_1);

  GuardPositionList guard_1{{0}};
  guard_1[0] = GetBoardPosition(kMoveIF, direction);
  guard_1[1] = GetBoardPosition(kMoveIC, direction);
  guard_1[2] = GetBoardPosition(kMoveIH, direction);
  expect_1.SetGuardPositionList(guard_1);

  // OpenState at kMoveIF by pattern kMoveID
  const BoardPosition open_position_2 = GetBoardPosition(kMoveIF, direction);
  const BoardPosition pattern_position_2 = GetBoardPosition(kMoveID, direction);
  OpenState<kPattern> expect_2(open_position_2, pattern_position_2);

  GuardPositionList guard_2{{0}};
  guard_2[0] = GetBoardPosition(kMoveIE, direction);
  guard_2[1] = GetBoardPosition(kMoveIC, direction);
  guard_2[2] = GetBoardPosition(kMoveIH, direction);
  expect_2.SetGuardPositionList(guard_2);

  ASSERT_EQ(2, open_state_list.size());
  EXPECT_TRUE(expect_1 == open_state_list[0]);
  EXPECT_TRUE(expect_2 == open_state_list[1]);
}

}   // namespace realcore
