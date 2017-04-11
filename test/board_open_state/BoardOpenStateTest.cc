#include "gtest/gtest.h"

#include <vector>

#include "MoveList.h"
#include "BitBoard.h"
#include "BoardOpenState.h"

using namespace std;

namespace realcore
{

class BoardOpenStateTest
: public ::testing::Test
{
  void ClearInfluencedOpenStateTest(){
    vector<OpenState> test_list;

    test_list.emplace_back(kNextFourBlack, 37, 37);
    test_list.emplace_back(kNextFourBlack, 41, 37);
    test_list.emplace_back(kNextFourBlack, 41, 38);
    test_list.emplace_back(kNextFourBlack, 42, 38);
    
    BoardOpenState board_open_state;
    vector<OpenState> cleared_list;
    
    board_open_state.ClearInfluencedOpenState<kWhiteTurn>(test_list, kMoveIB, &cleared_list);
    EXPECT_TRUE(cleared_list.empty());
  }
};

TEST_F(BoardOpenStateTest, DefaultConstructor)
{
  BoardOpenState board_open_state;

  EXPECT_TRUE(board_open_state.GetList(kNextOverline).empty());
  EXPECT_TRUE(board_open_state.GetList(kNextOpenFourBlack).empty());
  EXPECT_TRUE(board_open_state.GetList(kNextOpenFourWhite).empty());
  EXPECT_TRUE(board_open_state.GetList(kNextFourBlack).empty());
  EXPECT_TRUE(board_open_state.GetList(kNextFourWhite).empty());
  EXPECT_TRUE(board_open_state.GetList(kNextSemiThreeBlack).empty());
  EXPECT_TRUE(board_open_state.GetList(kNextSemiThreeWhite).empty());
}

TEST_F(BoardOpenStateTest, UpdateOverlineTest)
{
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
  
  MoveList board_move_list("hhhgihghmhnhlhmgjh");
  BitBoard bit_board(board_move_list);
  MovePosition move = board_move_list.GetLastMove();

  // 増加分が反映されるか
  BoardOpenState board_open_state;
  board_open_state.Update<kBlackTurn>(move, bit_board);

  {
    const auto &open_state_list = board_open_state.GetList(kNextOverline);
    ASSERT_EQ(1, open_state_list.size());
  }

  // 減少分が反映されるか
  move = kMoveKH;
  bit_board.SetState<kWhiteStone>(move);
  board_open_state.Update<kWhiteTurn>(move, bit_board);

  {
    const auto &open_state_list = board_open_state.GetList(kNextOverline);
    ASSERT_EQ(0, open_state_list.size());
  }
}

TEST_F(BoardOpenStateTest, UpdateOpenFourBlackTest)
{
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
  MoveList board_move_list("hhhgihigjh");
  BitBoard bit_board(board_move_list);
  MovePosition move = board_move_list.GetLastMove();

  // 増加分が反映されるか
  BoardOpenState board_open_state;
  board_open_state.Update<kBlackTurn>(move, bit_board);

  {
    const auto &open_state_list = board_open_state.GetList(kNextOpenFourBlack);
    ASSERT_EQ(2, open_state_list.size());
  }

  // 減少分が反映されるか
  move = kMoveKH;
  bit_board.SetState<kWhiteStone>(move);
  board_open_state.Update<kWhiteTurn>(move, bit_board);

  {
    const auto &open_state_list = board_open_state.GetList(kNextOpenFourBlack);
    ASSERT_EQ(0, open_state_list.size());    
  }
}

TEST_F(BoardOpenStateTest, UpdateOpenFourWhiteTest)
{
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
  MoveList board_move_list("hhhgihigjhjg");
  BitBoard bit_board(board_move_list);
  MovePosition move = board_move_list.GetLastMove();

  // 増加分が反映されるか
  BoardOpenState board_open_state;
  board_open_state.Update<kWhiteTurn>(move, bit_board);

  {
    const auto &open_state_list = board_open_state.GetList(kNextOpenFourWhite);
    ASSERT_EQ(2, open_state_list.size());
  }

  // 減少分が反映されるか
  move = kMoveKG;
  bit_board.SetState<kBlackStone>(move);
  board_open_state.Update<kBlackTurn>(move, bit_board);

  {
    const auto &open_state_list = board_open_state.GetList(kNextOpenFourWhite);
    ASSERT_EQ(0, open_state_list.size());    
  }
}

TEST_F(BoardOpenStateTest, UpdateFourBlackTest)
{
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
   MoveList board_move_list("hhighfkehd");
  BitBoard bit_board(board_move_list);
  MovePosition move = board_move_list.GetLastMove();

  // 増加分が反映されるか
  BoardOpenState board_open_state;
  board_open_state.Update<kBlackTurn>(move, bit_board);

  {
    const auto &open_state_list = board_open_state.GetList(kNextFourBlack);
    ASSERT_EQ(2, open_state_list.size());
  }

  // 減少分が反映されるか
  move = kMoveHE;
  bit_board.SetState<kWhiteStone>(move);
  board_open_state.Update<kWhiteTurn>(move, bit_board);

  {
    const auto &open_state_list = board_open_state.GetList(kNextFourBlack);
    ASSERT_EQ(0, open_state_list.size());    
  }
}

TEST_F(BoardOpenStateTest, UpdateFourWhiteTest)
{
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
  MoveList board_move_list("hhighfkehdmc");
  BitBoard bit_board(board_move_list);
  MovePosition move = board_move_list.GetLastMove();

  // 増加分が反映されるか
  BoardOpenState board_open_state;
  board_open_state.Update<kWhiteTurn>(move, bit_board);

  {
    const auto &open_state_list = board_open_state.GetList(kNextFourWhite);
    ASSERT_EQ(2, open_state_list.size());
  }

  // 減少分が反映されるか
  move = kMoveLD;
  bit_board.SetState<kBlackStone>(move);
  board_open_state.Update<kBlackTurn>(move, bit_board);

  {
    const auto &open_state_list = board_open_state.GetList(kNextFourWhite);
    ASSERT_EQ(0, open_state_list.size());    
  }
}

TEST_F(BoardOpenStateTest, UpdateSemiThreeBlackTest)
{
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
  MoveList board_move_list("hhigff");
  BitBoard bit_board(board_move_list);
  MovePosition move = board_move_list.GetLastMove();

  // 増加分が反映されるか
  BoardOpenState board_open_state;
  board_open_state.Update<kBlackTurn>(move, bit_board);

  {
    const auto &open_state_list = board_open_state.GetList(kNextSemiThreeBlack);
    ASSERT_EQ(4, open_state_list.size());
  }

  // 減少分が反映されるか
  move = kMoveEE;
  bit_board.SetState<kWhiteStone>(move);
  board_open_state.Update<kWhiteTurn>(move, bit_board);

  {
    const auto &open_state_list = board_open_state.GetList(kNextSemiThreeBlack);
    ASSERT_EQ(0, open_state_list.size());    
  }
}

TEST_F(BoardOpenStateTest, UpdateSemiThreeWhiteTest)
{
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
  MoveList board_move_list("hhigffid");
  BitBoard bit_board(board_move_list);
  MovePosition move = board_move_list.GetLastMove();

  // 増加分が反映されるか
  BoardOpenState board_open_state;
  board_open_state.Update<kWhiteTurn>(move, bit_board);

  {
    const auto &open_state_list = board_open_state.GetList(kNextSemiThreeWhite);
    ASSERT_EQ(2, open_state_list.size());
  }

  // 減少分が反映されるか
  move = kMoveIC;
  bit_board.SetState<kBlackStone>(move);
  board_open_state.Update<kBlackTurn>(move, bit_board);

  {
    const auto &open_state_list = board_open_state.GetList(kNextSemiThreeWhite);
    ASSERT_EQ(0, open_state_list.size());    
  }
}

TEST_F(BoardOpenStateTest, IsEqualTest)
{
  BoardOpenState state_1, state_2;

  EXPECT_TRUE(IsEqual(state_1, state_2));

  MoveList board_move_list("hhigff");
  BitBoard bit_board(board_move_list);
  MovePosition move = board_move_list.GetLastMove();
  state_1.Update<kBlackTurn>(move, bit_board);

  EXPECT_FALSE(IsEqual(state_1, state_2));

  // 空間状態がすべて空になりstate_2と等しくなる
  move = kMoveEE;
  bit_board.SetState<kWhiteStone>(move);
  state_1.Update<kWhiteTurn>(move, bit_board);

  EXPECT_TRUE(IsEqual(state_1, state_2));
}

TEST_F(BoardOpenStateTest, CompareOperTest)
{
  BoardOpenState state_1, state_2;

  EXPECT_TRUE(state_1 == state_2);

  MoveList board_move_list("hhigff");
  BitBoard bit_board(board_move_list);
  MovePosition move = board_move_list.GetLastMove();
  state_1.Update<kBlackTurn>(move, bit_board);

  EXPECT_TRUE(state_1 != state_2);

  // 空間状態がすべて空になりstate_2と等しくなる
  move = kMoveEE;
  bit_board.SetState<kWhiteStone>(move);
  state_1.Update<kWhiteTurn>(move, bit_board);

  EXPECT_TRUE(state_1 == state_2);
}

TEST_F(BoardOpenStateTest, CopyTest)
{
  BoardOpenState state_1, state_2;

  MoveList board_move_list("hhigff");
  BitBoard bit_board(board_move_list);
  MovePosition move = board_move_list.GetLastMove();
  state_1.Update<kBlackTurn>(move, bit_board);

  EXPECT_FALSE(IsEqual(state_1, state_2));

  Copy(state_1, &state_2);
  EXPECT_TRUE(IsEqual(state_1, state_2));
}

TEST_F(BoardOpenStateTest, AssignOperTest)
{
  BoardOpenState state_1, state_2;

  MoveList board_move_list("hhigff");
  BitBoard bit_board(board_move_list);
  MovePosition move = board_move_list.GetLastMove();
  state_1.Update<kBlackTurn>(move, bit_board);

  EXPECT_TRUE(state_1 != state_2);

  state_2 = state_1;
  EXPECT_TRUE(state_1 == state_2);
}

TEST_F(BoardOpenStateTest, CopyConstructerTest)
{
  BoardOpenState state_1;

  MoveList board_move_list("hhigff");
  BitBoard bit_board(board_move_list);
  MovePosition move = board_move_list.GetLastMove();
  state_1.Update<kBlackTurn>(move, bit_board);

  BoardOpenState state_2(state_1);
  EXPECT_TRUE(state_1 == state_2);
}

TEST_F(BoardOpenStateTest, EmptyTest)
{
  BoardOpenState state_1;
  EXPECT_TRUE(state_1.empty());

  MoveList board_move_list("hhigff");
  BitBoard bit_board(board_move_list);
  MovePosition move = board_move_list.GetLastMove();
  state_1.Update<kBlackTurn>(move, bit_board);
  
  EXPECT_FALSE(state_1.empty());
}
}   // namespace realcore
