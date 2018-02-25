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

  void GetPlayerStoneCombinedBitTest()
  {
    BitBoard bit_board;
    constexpr size_t distance = 1;

    {
      // 黒石
      LineNeighborhood line_neighborhood(kMoveHH, distance, bit_board);
      line_neighborhood.SetCenterState<kBlackStone>();
      
      auto expect_bit = LeftShift<7>(0b1) | (LeftShift<7>(0b1) << 32ULL);
      expect_bit |= expect_bit << 1;

      ASSERT_EQ(expect_bit, line_neighborhood.GetPlayerStoneCombinedBit<kBlackTurn>());
      ASSERT_EQ(0ULL, line_neighborhood.GetPlayerStoneCombinedBit<kWhiteTurn>());
    }
    {
      // 白石
      LineNeighborhood line_neighborhood(kMoveHH, distance, bit_board);
      line_neighborhood.SetCenterState<kWhiteStone>();
      
      auto expect_bit = LeftShift<7>(0b1) | (LeftShift<7>(0b1) << 32ULL);
      expect_bit |= expect_bit << 1;

      ASSERT_EQ(0ULL, line_neighborhood.GetPlayerStoneCombinedBit<kBlackTurn>());
      ASSERT_EQ(expect_bit, line_neighborhood.GetPlayerStoneCombinedBit<kWhiteTurn>());
    }
  }

  void GetOpenPositionCombinedBitTest()
  {
    BitBoard bit_board;
    constexpr size_t distance = 1;

    LineNeighborhood line_neighborhood(kMoveHH, distance, bit_board);
    line_neighborhood.SetCenterState<kBlackStone>();
    
    auto expect_bit = LeftShift<6>(0b1) | LeftShift<8>(0b1);
    expect_bit |= expect_bit << 32ULL;
    expect_bit |= expect_bit << 1ULL;

    ASSERT_EQ(expect_bit, line_neighborhood.GetOpenPositionCombinedBit());
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
    ASSERT_EQ(385, board_position_list[1]);
    ASSERT_EQ(737, board_position_list[2]);
    ASSERT_EQ(994, board_position_list[3]);
  }

  void GetBoardPositionTest()
  {
    BitBoard bit_board;
    constexpr size_t distance = 7;
    
    LineNeighborhood line_neighborhood(kMoveHH, distance, bit_board);
    LocalBitBoard bit_list{{0}};

    bit_list[0] = (0b1ULL << 0) | (0b01ULL << 34);
    bit_list[1] = (0b1ULL << 2) | (0b01ULL << 36);

    const uint64_t combined_bit = bit_list[0] | (bit_list[1] << 1);

    vector<size_t> combined_shift_list;
    GetBitIndexList(combined_bit, &combined_shift_list);
    vector<BoardPosition> board_position_list;

    for(const auto combined_shift : combined_shift_list){
      const auto board_position = line_neighborhood.GetBoardPosition(combined_shift);
      board_position_list.emplace_back(board_position);
    }

    ASSERT_EQ(4, board_position_list.size());

    auto find_it = find(board_position_list.begin(), board_position_list.end(), 129);
    ASSERT_TRUE(find_it != board_position_list.end());

    find_it = find(board_position_list.begin(), board_position_list.end(), 385);
    ASSERT_TRUE(find_it != board_position_list.end());    
    
    find_it = find(board_position_list.begin(), board_position_list.end(), 737);
    ASSERT_TRUE(find_it != board_position_list.end());    
    
    find_it = find(board_position_list.begin(), board_position_list.end(), 994);
    ASSERT_TRUE(find_it != board_position_list.end());    
  }

  void GetLocalBitBoardTest(){
    MoveList board_list("hhigiigg");
    BitBoard bit_board(board_list);
    constexpr size_t distance = 3;

    LineNeighborhood line_neighborhood(kMoveHH, distance, bit_board);
    LocalBitBoard local_bit_board{{0}};

    line_neighborhood.GetLocalBitBoard(&local_bit_board);

    ASSERT_EQ(local_bit_board[0], line_neighborhood.local_bit_board_[0]);
    ASSERT_EQ(local_bit_board[1], line_neighborhood.local_bit_board_[1]);
  }
};

TEST_F(LineNeighborhoodTest, ForbiddenCheckTest)
{
  //   A B C D E F G H I J K L M N O 
  // A + --------------------------+ A 
  // B | . . . . . . . . . . . . . | B 
  // C | . . . . . . . . . . . . . | C 
  // D | . . * . . . . . . . * . . | D 
  // E | . . . . . . . . . . . . . | E 
  // F | . . . . x . . . . . . . . | F 
  // G | . . . . . . . . . . . . . | G 
  // H | . . . . . . x . . . . . . | H 
  // I | . . . . . . o . . . . . . | I 
  // J | . . . . . . o . . . . . . | J 
  // K | . . . . . . . . . . . . . | K 
  // L | . . * . . . . . . . * . . | L 
  // M | . . . . . . . . . . . . . | M 
  // N | . . . . . . . . . . . . . | N 
  // O + --------------------------+ O 
  //   A B C D E F G H I J K L M N O 
  constexpr size_t kForbiddenCheckSize = 5;
  BitBoard bit_board(MoveList("hhhiffhj"));
  LineNeighborhood line_neighborhood(kMoveGG, kForbiddenCheckSize, bit_board);
  line_neighborhood.SetCenterState<kBlackStone>();

  vector<BoardPosition> list;
  const auto result = line_neighborhood.ForbiddenCheck(&list);

  ASSERT_EQ(kNonForbiddenMove, result);
}

TEST_F(LineNeighborhoodTest, AddOpenStateTest)
{
  //   A B C D E F G H I J K L M N O 
  // A + --------------------------+ A 
  // B | . x . . x x x o . . . . . | B 
  // C | . . o . o . . o x x . . . | C 
  // D | . . * o . x . o o o x . . | D 
  // E | . . . x o . o . x o . o . | E 
  // F | . . . . . o x x . o o o x | F 
  // G | . . . . x o x o x x x x o | G 
  // H | . . . . o . x . . o o . . | H 
  // I | . . . . . x o . . . x . . | I 
  // J | . . x . . . . . . . . . . | J 
  // K | . . . . . . . . . . . . . | K 
  // L | . . * . . . . . . . * . . | L 
  // M | . . . . . . . . . . . . . | M 
  // N | . . . . . . . . . . . . . | N 
  // O + --------------------------+ O 
  //   A B C D E F G H I J K L M N O 
  BoardOpenState board_open_state;
  BitBoard bit_board(MoveList("hhhijggggiigfgfhhfhedjidifgfgdkfjekdjcjdldfeeeedhgfcfbkekcichblfmgmfnfmekgdccblhlikhlgnggbib"));
  
  LineNeighborhood line_neighborhood(kMoveIB, kOpenStateNeighborhoodSize, bit_board);
  line_neighborhood.AddOpenState<kWhiteTurn>(kUpdateAllOpenState, &board_open_state);

  EXPECT_TRUE(board_open_state.GetList(kNextFourBlack).empty());
}

TEST_F(LineNeighborhoodTest, GetOpenMovePositionTest)
{
  //   A B C D E F G H I J K L M N O 
  // A + --------------------------+ A 
  // B | . . . . . . . . . . . . . | B 
  // C | . . . . . . . . . . . . . | C 
  // D | . . o . . . . . . . o . . | D 
  // E | . . . . . . . . . . . . . | E 
  // F | . . . . x . . . . . . . . | F 
  // G | o . x x x . . . . . . . . | G 
  // H | . . . . . . x . . . . . . | H 
  // I | . . . . . . . x . . . . . | I 
  // J | . . . . . . . . o . . . . | J 
  // K | . . . . . . . . . o . . . | K 
  // L | . . o . . . . . . . * . . | L 
  // M | . . . . . . . . . . . . . | M 
  // N | . . . . . . . . . . . . . | N 
  // O + --------------------------+ O 
  //   A B C D E F G H I J K L M N O 
  constexpr size_t kSize = 2;
  BitBoard bit_board(MoveList("hhkkiiddeglddgdlffjjfgbg"));

  LineNeighborhood line_neighborhood(kMoveGG, kSize, bit_board);
  MoveList move_list;

  line_neighborhood.GetOpenMovePosition(&move_list);

  ASSERT_EQ(12, move_list.size());
  ASSERT_TRUE(find(move_list.begin(), move_list.end(), kMoveHG) != move_list.end());
  ASSERT_TRUE(find(move_list.begin(), move_list.end(), kMoveIG) != move_list.end());
  ASSERT_TRUE(find(move_list.begin(), move_list.end(), kMoveGE) != move_list.end());
  ASSERT_TRUE(find(move_list.begin(), move_list.end(), kMoveGF) != move_list.end());
  ASSERT_TRUE(find(move_list.begin(), move_list.end(), kMoveGH) != move_list.end());
  ASSERT_TRUE(find(move_list.begin(), move_list.end(), kMoveGI) != move_list.end());
  ASSERT_TRUE(find(move_list.begin(), move_list.end(), kMoveEE) != move_list.end());
  ASSERT_TRUE(find(move_list.begin(), move_list.end(), kMoveIE) != move_list.end());
  ASSERT_TRUE(find(move_list.begin(), move_list.end(), kMoveHF) != move_list.end());
  ASSERT_TRUE(find(move_list.begin(), move_list.end(), kMoveFH) != move_list.end());
  ASSERT_TRUE(find(move_list.begin(), move_list.end(), kMoveEI) != move_list.end());
  ASSERT_TRUE(find(move_list.begin(), move_list.end(), kMoveGG) != move_list.end());
}

TEST_F(LineNeighborhoodTest, IsAllOpenPositionTest)
{
  //   A B C D E F G H I J K L M N O 
  // A + --------------------------+ A 
  // B | . . . . . . . . . . . . . | B 
  // C | . . . . . . . . . . . . . | C 
  // D | . . * . . . . . . . * . . | D 
  // E | . . . . . . . . . . . . . | E 
  // F | . . . . x . . . . . . . . | F 
  // G | . . . . . . . . . . . . . | G 
  // H | . . . . . . x . . . . . . | H 
  // I | . . . . . . o . . . . . . | I 
  // J | . . . . . . o . . . . . . | J 
  // K | . . . . . . . . . . . . . | K 
  // L | . . * . . . . . . . * . . | L 
  // M | . . . . . . . . . . . . . | M 
  // N | . . . . . . . . . . . . . | N 
  // O + --------------------------+ O 
  //   A B C D E F G H I J K L M N O 
  constexpr size_t kForbiddenCheckSize = 2;
  BitBoard bit_board(MoveList("hhhiffhj"));
  LineNeighborhood line_neighborhood(kMoveLD, kForbiddenCheckSize, bit_board);  

  ASSERT_TRUE(line_neighborhood.IsAllOpenPosition());

  line_neighborhood.SetCenterState<kBlackStone>();
  ASSERT_FALSE(line_neighborhood.IsAllOpenPosition());
}

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

TEST_F(LineNeighborhoodTest, GetBoardPositionTest)
{
  GetBoardPositionTest();
}

TEST_F(LineNeighborhoodTest, GetPlayerStoneCombinedBitTest)
{
  GetPlayerStoneCombinedBitTest();
}

TEST_F(LineNeighborhoodTest, GetOpenPositionCombinedBitTest)
{
  GetOpenPositionCombinedBitTest();
}

TEST_F(LineNeighborhoodTest, strTest)
{
  //   A B C D E F G H I J K L M N O 
  // A + --------------------------+ A 
  // B | . . . . . . . . . . . . . | B 
  // C | . . . . . . . . . . . . . | C 
  // D | . . * . . . . . . . * . . | D 
  // E | . . . . . . . . . . . . . | E 
  // F | . . . . x . . . . . . . . | F 
  // G | . . . . . . . . . . . . . | G 
  // H | . . . . . . x . . . . . . | H 
  // I | . . . . . . o . . . . . . | I 
  // J | . . . . . . o . . . . . . | J 
  // K | . . . . . . . . . . . . . | K 
  // L | . . * . . . . . . . * . . | L 
  // M | . . . . . . . . . . . . . | M 
  // N | . . . . . . . . . . . . . | N 
  // O + --------------------------+ O 
  //   A B C D E F G H I J K L M N O 
  constexpr size_t kForbiddenCheckSize = 2;
  BitBoard bit_board(MoveList("hhhiffhj"));
  LineNeighborhood line_neighborhood(kMoveHH, kForbiddenCheckSize, bit_board);  

  stringstream ss;

  ss << "Horizn: " << "OOBOO" << endl;
  ss << "Vertcl: " << "OOBWW" << endl;
  ss << "L-Down: " << "OOBOO" << endl;
  ss << "R-Down: " << "BOBOO" << endl;
  
  ASSERT_EQ(ss.str(), line_neighborhood.str());
}

TEST_F(LineNeighborhoodTest, GetLocalBitBoardTest)
{
  GetLocalBitBoardTest();
}

}   // namespace realcore
