// @brief メンバ関数のテスト
#include "gtest/gtest.h"

#include "Move.h"
#include "MoveList.h"
#include "BoardOpenState.h"
#include "BitBoard.h"

using namespace std;

namespace realcore
{

class BitBoardTest
: public ::testing::Test
{
public:
  void GetSetStateTest(){
    BitBoard bit_board;
    const auto &all_move_list = GetAllMove();

    // 黒石
    for(auto set_move : all_move_list){
      bit_board.SetState<kBlackStone>(set_move);

      for(auto get_move : all_move_list){
        // 設定した位置では黒石が、それ以外は空点が設定されていることを確認する
        PositionState check_state = kOverBoard;

        if(IsInBoardMove(get_move)){
          check_state = set_move == get_move ? kBlackStone : kOpenPosition;
        }

        EXPECT_EQ(check_state, bit_board.GetState(get_move));
      }

      bit_board.SetState<kOpenPosition>(set_move);
    }

    // 白石
    for(auto set_move : all_move_list){
      bit_board.SetState<kWhiteStone>(set_move);

      for(auto get_move : all_move_list){
        // 設定した位置では白石が、それ以外は空点が設定されていることを確認する
        PositionState check_state = kOverBoard;

        if(IsInBoardMove(get_move)){
          check_state = set_move == get_move ? kWhiteStone : kOpenPosition;
        }

        EXPECT_EQ(check_state, bit_board.GetState(get_move));
      }

      bit_board.SetState<kOpenPosition>(set_move);
    }
  }

  void GetOpenStateOverlineTest(){
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
    BitBoard bit_board(MoveList("hhhgihghmhnhlhmgjh"));

    BoardOpenState board_open_state;
    bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);

    vector<OpenState> expect_list;
    const auto &open_state_list = board_open_state.GetList(kNextOverline);

    const BoardDirection direction = kLateralDirection;
    const BoardPosition open_position = GetBoardPosition(kMoveKH, direction);
    const BoardPosition pattern_position = GetBoardPosition(kMoveIH, direction);
    OpenState expect(kPattern, open_position, pattern_position);

    ASSERT_EQ(1, open_state_list.size());
    EXPECT_TRUE(expect == open_state_list[0]);
  }

  void GetOpenStateOpenFourBlackTest()
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
    BitBoard bit_board(MoveList("hhhgihigjh"));
    BoardOpenState board_open_state;
    bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);

    vector<OpenState> expect_list;
    const auto &open_state_list = board_open_state.GetList(kNextOpenFourBlack);

    const BoardDirection direction = kLateralDirection;

    const BoardPosition open_position_1 = GetBoardPosition(kMoveGH, direction);
    const BoardPosition pattern_position_1 = GetBoardPosition(kMoveGH, direction);
    OpenState expect_1(kPattern, open_position_1, pattern_position_1);

    const BoardPosition open_position_2 = GetBoardPosition(kMoveKH, direction);
    const BoardPosition pattern_position_2 = GetBoardPosition(kMoveHH, direction);
    OpenState expect_2(kPattern, open_position_2, pattern_position_2);

    ASSERT_EQ(2, open_state_list.size());
    EXPECT_TRUE(expect_1 == open_state_list[0]);
    EXPECT_TRUE(expect_2 == open_state_list[1]);
  }

  void EnumerateOpenFourMovesBlackTest()
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
    BitBoard bit_board(MoveList("hhhgihigjh"));
    BoardOpenState board_open_state;
    bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);

    MoveBitSet open_four_bit;
    bit_board.EnumerateOpenFourMoves<kBlackTurn>(board_open_state, &open_four_bit);

    ASSERT_EQ(2, open_four_bit.count());
    ASSERT_TRUE(open_four_bit[kMoveGH]);
    ASSERT_TRUE(open_four_bit[kMoveKH]);
  }


  void GetOpenStateOpenFourWhiteTest()
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
    BitBoard bit_board(MoveList("hhhgihigjhjg"));
    BoardOpenState board_open_state;
    bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);

    vector<OpenState> expect_list;
    const auto &open_state_list = board_open_state.GetList(kNextOpenFourWhite);

    const BoardDirection direction = kLateralDirection;

    const BoardPosition open_position_1 = GetBoardPosition(kMoveGG, direction);
    const BoardPosition pattern_position_1 = GetBoardPosition(kMoveGG, direction);
    OpenState expect_1(kPattern, open_position_1, pattern_position_1);

    const BoardPosition open_position_2 = GetBoardPosition(kMoveKG, direction);
    const BoardPosition pattern_position_2 = GetBoardPosition(kMoveHG, direction);
    OpenState expect_2(kPattern, open_position_2, pattern_position_2);

    ASSERT_EQ(2, open_state_list.size());
    EXPECT_TRUE(expect_1 == open_state_list[0]);
    EXPECT_TRUE(expect_2 == open_state_list[1]);
  }
  
  void EnumerateOpenFourMovesWhiteTest()
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
    BitBoard bit_board(MoveList("hhhgihigjhjg"));
    BoardOpenState board_open_state;
    bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);

    MoveBitSet open_four_bit;
    bit_board.EnumerateOpenFourMoves<kWhiteTurn>(board_open_state, &open_four_bit);

    ASSERT_EQ(2, open_four_bit.count());
    ASSERT_TRUE(open_four_bit[kMoveGG]);
    ASSERT_TRUE(open_four_bit[kMoveKG]);
  }

  void GetOpenStateFourBlackTest()
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
    BitBoard bit_board(MoveList("hhighfkehd"));
    BoardOpenState board_open_state;
    bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);

    vector<OpenState> expect_list;
    const auto &open_state_list = board_open_state.GetList(kNextFourBlack);

    const BoardDirection direction = kVerticalDirection;

    const BoardPosition open_position_1 = GetBoardPosition(kMoveHE, direction);
    const BoardPosition pattern_position_1 = GetBoardPosition(kMoveHD, direction);
    OpenState expect_1(kPattern, open_position_1, pattern_position_1);

    GuardPositionList guard_1{{0}};
    guard_1[0] = GetBoardPosition(kMoveHG, direction);
    expect_1.SetGuardPositionList(guard_1);

    const BoardPosition open_position_2 = GetBoardPosition(kMoveHG, direction);
    const BoardPosition pattern_position_2 = GetBoardPosition(kMoveHD, direction);
    OpenState expect_2(kPattern, open_position_2, pattern_position_2);

    GuardPositionList guard_2{{0}};
    guard_2[0] = GetBoardPosition(kMoveHE, direction);
    expect_2.SetGuardPositionList(guard_2);

    ASSERT_EQ(2, open_state_list.size());
    EXPECT_TRUE(expect_1 == open_state_list[0]);
    EXPECT_TRUE(expect_2 == open_state_list[1]);
  }

  void EnumerateFourMovesBlackTest()
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
    BitBoard bit_board(MoveList("hhighfkehd"));
    BoardOpenState board_open_state;
    bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);

    MoveBitSet four_bit;
    bit_board.EnumerateFourMoves<kBlackTurn>(board_open_state, &four_bit);

    ASSERT_EQ(2, four_bit.count());
    ASSERT_TRUE(four_bit[kMoveHE]);
    ASSERT_TRUE(four_bit[kMoveHG]);

    vector<MovePair> four_pair_list;
    bit_board.EnumerateFourMoves<kBlackTurn>(board_open_state, &four_pair_list);

    ASSERT_EQ(2, four_pair_list.size());

    {
      auto &move_pair = four_pair_list[0];
      const bool is_expected = (move_pair.first == kMoveHE && move_pair.second == kMoveHG) || (move_pair.first == kMoveHG && move_pair.second == kMoveHE);
      ASSERT_TRUE(is_expected);
    }
    {
      auto &move_pair = four_pair_list[1];
      const bool is_expected = (move_pair.first == kMoveHE && move_pair.second == kMoveHG) || (move_pair.first == kMoveHG && move_pair.second == kMoveHE);
      ASSERT_TRUE(is_expected);
    }
  }

  void GetOpenStateFourWhiteTest()
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
    BitBoard bit_board(MoveList("hhighfkehdmc"));
    BoardOpenState board_open_state;
    bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);

    vector<OpenState> expect_list;
    const auto &open_state_list = board_open_state.GetList(kNextFourWhite);

    const BoardDirection direction = kLeftDiagonalDirection;

    const BoardPosition open_position_1 = GetBoardPosition(kMoveLD, direction);
    const BoardPosition pattern_position_1 = GetBoardPosition(kMoveMC, direction);
    OpenState expect_1(kPattern, open_position_1, pattern_position_1);

    GuardPositionList guard_1{{0}};
    guard_1[0] = GetBoardPosition(kMoveJF, direction);
    expect_1.SetGuardPositionList(guard_1);

    const BoardPosition open_position_2 = GetBoardPosition(kMoveJF, direction);
    const BoardPosition pattern_position_2 = GetBoardPosition(kMoveMC, direction);
    OpenState expect_2(kPattern, open_position_2, pattern_position_2);

    GuardPositionList guard_2{{0}};
    guard_2[0] = GetBoardPosition(kMoveLD, direction);
    expect_2.SetGuardPositionList(guard_2);

    ASSERT_EQ(2, open_state_list.size());
    EXPECT_TRUE(expect_1 == open_state_list[0]);
    EXPECT_TRUE(expect_2 == open_state_list[1]);
  }

  void EnumerateFourMovesWhiteTest1()
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
    BitBoard bit_board(MoveList("hhighfkehdmc"));
    BoardOpenState board_open_state;
    bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);

    MoveBitSet four_bit;
    bit_board.EnumerateFourMoves<kWhiteTurn>(board_open_state, &four_bit);

    ASSERT_EQ(2, four_bit.count());
    ASSERT_TRUE(four_bit[kMoveLD]);
    ASSERT_TRUE(four_bit[kMoveJF]);

    vector<MovePair> four_pair_list;
    bit_board.EnumerateFourMoves<kWhiteTurn>(board_open_state, &four_pair_list);

    ASSERT_EQ(2, four_pair_list.size());

    {
      auto &move_pair = four_pair_list[0];
      const bool is_expected = (move_pair.first == kMoveLD && move_pair.second == kMoveJF) || (move_pair.first == kMoveJF && move_pair.second == kMoveLD);
      ASSERT_TRUE(is_expected);
    }
    {
      auto &move_pair = four_pair_list[1];
      const bool is_expected = (move_pair.first == kMoveLD && move_pair.second == kMoveJF) || (move_pair.first == kMoveJF && move_pair.second == kMoveLD);
      ASSERT_TRUE(is_expected);
    }
  }

  void EnumerateFourMovesWhiteTest2()
  {
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . x x . . . . . | F 
    // G | . . . . . x o o . o . o . x G 
    // H | . . . . . . x . . . . . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhhgggigifkgogmghf"));
    BoardOpenState board_open_state;
    bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);

    MoveBitSet four_bit;
    bit_board.EnumerateFourMoves<kWhiteTurn>(board_open_state, &four_bit);

    ASSERT_EQ(2, four_bit.count());
    ASSERT_TRUE(four_bit[kMoveJG]);
    ASSERT_TRUE(four_bit[kMoveLG]);

    vector<MovePair> four_pair_list;
    bit_board.EnumerateFourMoves<kWhiteTurn>(board_open_state, &four_pair_list);

    ASSERT_EQ(2, four_pair_list.size());

    {
      auto &move_pair = four_pair_list[0];
      const bool is_expected = (move_pair.first == kMoveJG && move_pair.second == kMoveLG) || (move_pair.first == kMoveLG && move_pair.second == kMoveJG);
      ASSERT_TRUE(is_expected);
    }
    {
      auto &move_pair = four_pair_list[1];
      const bool is_expected = (move_pair.first == kMoveJG && move_pair.second == kMoveLG) || (move_pair.first == kMoveLG && move_pair.second == kMoveJG);
      ASSERT_TRUE(is_expected);
    }
  }

  void GetOpenStateSemiThreeBlackTest()
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
    BitBoard bit_board(MoveList("hhigff"));
    BoardOpenState board_open_state;
    bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);

    vector<OpenState> expect_list;
    const auto &open_state_list = board_open_state.GetList(kNextSemiThreeBlack);

    const BoardDirection direction = kRightDiagonalDirection;

    // OpenState at kMoveEE by pattern kMoveEE
    const BoardPosition open_position_1 = GetBoardPosition(kMoveEE, direction);
    const BoardPosition pattern_position_1 = GetBoardPosition(kMoveEE, direction);
    OpenState expect_1(kPattern, open_position_1, pattern_position_1);

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
    OpenState expect_2(kPattern, open_position_2, pattern_position_2);

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
    OpenState expect_3(kPattern, open_position_3, pattern_position_3);

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
    OpenState expect_4(kPattern, open_position_4, pattern_position_4);

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

  void EnumerateSemiThreeBlackTest()
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
    BitBoard bit_board(MoveList("hhigff"));
    BoardOpenState board_open_state;
    bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);

    MoveBitSet semi_three_bit;
    bit_board.EnumerateSemiThreeMoves<kBlackTurn>(board_open_state, &semi_three_bit);

    ASSERT_EQ(3, semi_three_bit.count());
    EXPECT_TRUE(semi_three_bit[kMoveEE]);
    EXPECT_TRUE(semi_three_bit[kMoveGG]);
    EXPECT_TRUE(semi_three_bit[kMoveII]);
  }

  void GetOpenStateSemiThreeWhiteTest(){
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
    BitBoard bit_board(MoveList("hhigffid"));
    BoardOpenState board_open_state;
    bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);

    vector<OpenState> expect_list;
    const auto &open_state_list = board_open_state.GetList(kNextSemiThreeWhite);

    const BoardDirection direction = kVerticalDirection;

    // OpenState at kMoveIE by pattern kMoveID
    const BoardPosition open_position_1 = GetBoardPosition(kMoveIE, direction);
    const BoardPosition pattern_position_1 = GetBoardPosition(kMoveID, direction);
    OpenState expect_1(kPattern, open_position_1, pattern_position_1);

    GuardPositionList guard_1{{0}};
    guard_1[0] = GetBoardPosition(kMoveIF, direction);
    guard_1[1] = GetBoardPosition(kMoveIC, direction);
    guard_1[2] = GetBoardPosition(kMoveIH, direction);
    expect_1.SetGuardPositionList(guard_1);

    // OpenState at kMoveIF by pattern kMoveID
    const BoardPosition open_position_2 = GetBoardPosition(kMoveIF, direction);
    const BoardPosition pattern_position_2 = GetBoardPosition(kMoveID, direction);
    OpenState expect_2(kPattern, open_position_2, pattern_position_2);

    GuardPositionList guard_2{{0}};
    guard_2[0] = GetBoardPosition(kMoveIE, direction);
    guard_2[1] = GetBoardPosition(kMoveIC, direction);
    guard_2[2] = GetBoardPosition(kMoveIH, direction);
    expect_2.SetGuardPositionList(guard_2);

    ASSERT_EQ(2, open_state_list.size());
    EXPECT_TRUE(expect_1 == open_state_list[0]);
    EXPECT_TRUE(expect_2 == open_state_list[1]);
  }

  void EnumerateSemiThreeWhiteTest()
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
    BitBoard bit_board(MoveList("hhigffid"));
    BoardOpenState board_open_state;
    bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);

    MoveBitSet semi_three_bit;
    bit_board.EnumerateSemiThreeMoves<kWhiteTurn>(board_open_state, &semi_three_bit);

    ASSERT_EQ(2, semi_three_bit.count());
    EXPECT_TRUE(semi_three_bit[kMoveIE]);
    EXPECT_TRUE(semi_three_bit[kMoveIF]);
  }

  void GetBoardStateBitTest(){
    BitBoard bit_board;

    bit_board.SetState<kBlackStone>(kMoveHH);
    bit_board.SetState<kWhiteStone>(kMoveHG);

    array<StateBit, 8> board_info;
    bit_board.GetBoardStateBit(&board_info);

    for(size_t i=0; i<8; i++){
      ASSERT_EQ(bit_board.bit_board_[i], board_info[i]);
    }
  }
};

TEST_F(BitBoardTest, DefaultConstructorTest){
  const auto &move_list = GetAllInBoardMove();
  const BitBoard bit_board;

  // 盤内は空点で初期化されていることを確認する
  for(const auto move : move_list)
  {
    const PositionState state = bit_board.GetState(move);
    EXPECT_EQ(kOpenPosition, state);
  }

  // 盤内以外の手では盤外が返ってくることを確認する
  EXPECT_EQ(kOverBoard, bit_board.GetState(kInvalidMove));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kNullMove));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kDeclareEndGame));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kUndefinedMove01));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kUndefinedMove28));
}

TEST_F(BitBoardTest, CopyConstructorTest){
  BitBoard bit_board_1;

  bit_board_1.SetState<kBlackStone>(kMoveHH);

  BitBoard bit_board_2(bit_board_1);
  EXPECT_TRUE(bit_board_1 == bit_board_2);
}

TEST_F(BitBoardTest, MoveListConstructorTest){
  const MoveList board_move_list("hhhg");
  BitBoard bit_board(board_move_list);

  const auto &move_list = GetAllInBoardMove();

  // 盤内が初期化されているか確認する
  for(const auto move : move_list){
    const PositionState state = bit_board.GetState(move);

    if(move == kMoveHH){
      EXPECT_EQ(kBlackStone, state);
    }else if(move == kMoveHG){
      EXPECT_EQ(kWhiteStone, state);
    }else{
      EXPECT_EQ(kOpenPosition, state);
    }
  }
  
  // 盤内以外の手では盤外が返ってくることを確認する
  EXPECT_EQ(kOverBoard, bit_board.GetState(kInvalidMove));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kNullMove));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kDeclareEndGame));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kUndefinedMove01));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kUndefinedMove28));

  BitBoard bitboard_expect;
  bitboard_expect.SetState<kBlackStone>(kMoveHH);
  bitboard_expect.SetState<kWhiteStone>(kMoveHG);

  EXPECT_TRUE(bitboard_expect == bit_board);
}

TEST_F(BitBoardTest, strTest)
{
  BitBoard bit_board;

  {
    // 初期状態のテスト
    string board_str = bit_board.str();
    stringstream expect_ss;

    expect_ss << "  A B C D E F G H I J K L M N O " << endl;
    expect_ss << "A + --------------------------+ A " << endl;
    expect_ss << "B | . . . . . . . . . . . . . | B " << endl;
    expect_ss << "C | . . . . . . . . . . . . . | C " << endl;
    expect_ss << "D | . . * . . . . . . . * . . | D " << endl;
    expect_ss << "E | . . . . . . . . . . . . . | E " << endl;
    expect_ss << "F | . . . . . . . . . . . . . | F " << endl;
    expect_ss << "G | . . . . . . . . . . . . . | G " << endl;
    expect_ss << "H | . . . . . . * . . . . . . | H " << endl;
    expect_ss << "I | . . . . . . . . . . . . . | I " << endl;
    expect_ss << "J | . . . . . . . . . . . . . | J " << endl;
    expect_ss << "K | . . . . . . . . . . . . . | K " << endl;
    expect_ss << "L | . . * . . . . . . . * . . | L " << endl;
    expect_ss << "M | . . . . . . . . . . . . . | M " << endl;
    expect_ss << "N | . . . . . . . . . . . . . | N " << endl;
    expect_ss << "O + --------------------------+ O " << endl;
    expect_ss << "  A B C D E F G H I J K L M N O " << endl;
    
    EXPECT_TRUE(board_str == expect_ss.str());
  }
  {
    // 黒石、白石を設定
    bit_board.SetState<kBlackStone>(kMoveHH);
    bit_board.SetState<kWhiteStone>(kMoveHG);
    string board_str = bit_board.str();

    stringstream expect_ss;
    
    expect_ss << "  A B C D E F G H I J K L M N O " << endl;
    expect_ss << "A + --------------------------+ A " << endl;
    expect_ss << "B | . . . . . . . . . . . . . | B " << endl;
    expect_ss << "C | . . . . . . . . . . . . . | C " << endl;
    expect_ss << "D | . . * . . . . . . . * . . | D " << endl;
    expect_ss << "E | . . . . . . . . . . . . . | E " << endl;
    expect_ss << "F | . . . . . . . . . . . . . | F " << endl;
    expect_ss << "G | . . . . . . o . . . . . . | G " << endl;
    expect_ss << "H | . . . . . . x . . . . . . | H " << endl;
    expect_ss << "I | . . . . . . . . . . . . . | I " << endl;
    expect_ss << "J | . . . . . . . . . . . . . | J " << endl;
    expect_ss << "K | . . . . . . . . . . . . . | K " << endl;
    expect_ss << "L | . . * . . . . . . . * . . | L " << endl;
    expect_ss << "M | . . . . . . . . . . . . . | M " << endl;
    expect_ss << "N | . . . . . . . . . . . . . | N " << endl;
    expect_ss << "O + --------------------------+ O " << endl;
    expect_ss << "  A B C D E F G H I J K L M N O " << endl;

    EXPECT_TRUE(board_str == expect_ss.str());
  }
}

TEST_F(BitBoardTest, GetLineNeighborhoodStateBitTest)
{
  {
    // 盤の中央に黒石、白石があるケース(N=1)
    MoveList move_list("hhhg");
    BitBoard bit_board(move_list);
    
    array<StateBit, kBoardDirectionNum> line_neighborhood;
    bit_board.GetLineNeighborhoodStateBit<1>(kMoveHH, &line_neighborhood);

    const StateBit lateral_state_bit = GetStateBit("OBOXXXXXX");
    EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

    const StateBit vertical_state_bit = GetStateBit("OBWXXXXXX");
    EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

    const StateBit left_diagonal_state_bit = GetStateBit("OBOXXXXXX");
    EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

    const StateBit right_diagonal_state_bit = GetStateBit("OBOXXXXXX");
    EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
  }
  {
    // 盤の中央に黒石、白石があるケース(N=7)
    MoveList move_list("hhhg");
    BitBoard bit_board(move_list);
    
    array<StateBit, kBoardDirectionNum> line_neighborhood;
    bit_board.GetLineNeighborhoodStateBit<7>(kMoveHH, &line_neighborhood);

    const StateBit lateral_state_bit = GetStateBit("OOOOOOOBOOOOOOO");
    EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

    const StateBit vertical_state_bit = GetStateBit("OOOOOOOBWOOOOOO");
    EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

    const StateBit left_diagonal_state_bit = GetStateBit("OOOOOOOBOOOOOOO");
    EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

    const StateBit right_diagonal_state_bit = GetStateBit("OOOOOOOBOOOOOOO");
    EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
  }
  {
    // 四隅に黒石、白石があるケース
    MoveList move_list("aaoaaooo");
    BitBoard bit_board(move_list);
    //   A B C D E F G H I J K L M N O 
    // A x --------------------------o A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . . . . . . . . . | G 
    // H | . . . . . . * . . . . . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O x --------------------------o O 
    //   A B C D E F G H I J K L M N O 
    {
      // 左上隅(N=1)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<1>(kMoveAA, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("OBXXXXXXX");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("OBXXXXXXX");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("BXXXXXXX");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("OBXXXXXXX");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 左上隅(N=7)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<7>(kMoveAA, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("OOOOOOOBXXXXXXX");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("OOOOOOOBXXXXXXX");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("OOOOOOXBXXXXXXX");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("OOOOOOOBXXXXXXX");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 右上隅(N=1)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<1>(kMoveOA, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("WOXXXXXX");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("OWXXXXXXX");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("OWXXXXXXX");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("WXXXXXXX");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 右上隅(N=7)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<7>(kMoveOA, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("WOOOOOOO");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("OOOOOOOWXOOOOOO");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("OOOOOOOWXXXXXXX");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("OOOOOOXWXXXXXXX");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 左下隅(N=1)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<1>(kMoveAO, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("OBXXXXXXX");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("BOXXXXXX");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("BOXXXXXX");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("BXXXXXXX");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 左下隅(N=7)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<7>(kMoveAO, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("OOOOOOOBXOOOOOO");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("BOOOOOOO");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("OOOOOXXBOOOOOOO");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("OOOOOOXBXOOOOOO");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 右下隅(N=1)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<1>(kMoveOO, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("WOXXXXXX");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("WOXXXXXX");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("WXXXXXXX");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("WOXXXXXX");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 右下隅(N=7)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<7>(kMoveOO, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("WOOOOOOO");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("WOOOOOOO");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("OOOOOOXWXOOOOOO");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("OOOOOXXWOOOOOOO");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 盤外のケース
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<7>(kNullMove, &line_neighborhood);

      for(auto value : line_neighborhood){
        EXPECT_EQ(0, value);
      }
    }
  }
}

TEST_F(BitBoardTest, GetSetStateTest)
{
  GetSetStateTest();
}

TEST_F(BitBoardTest, CompareOperTest){
  BitBoard bit_board_1, bit_board_2;

  EXPECT_TRUE(bit_board_1 == bit_board_1);
  EXPECT_TRUE(bit_board_1 == bit_board_2);

  bit_board_1.SetState<kBlackStone>(kMoveHH);
  EXPECT_TRUE(bit_board_1 == bit_board_1);
  EXPECT_TRUE(bit_board_1 != bit_board_2);
  
  bit_board_2.SetState<kBlackStone>(kMoveHH);
  EXPECT_TRUE(bit_board_1 == bit_board_2);
}

TEST_F(BitBoardTest, AssignOperTest){
  BitBoard bit_board_1, bit_board_2;

  bit_board_1.SetState<kBlackStone>(kMoveHH);
  EXPECT_TRUE(bit_board_1 != bit_board_2);

  bit_board_2 = bit_board_1;
  EXPECT_TRUE(bit_board_1 == bit_board_2);
}

TEST_F(BitBoardTest, GetOpenStateOverlineTest)
{
  GetOpenStateOverlineTest();
}

TEST_F(BitBoardTest, GetOpenStateOpenFourBlackTest)
{
  GetOpenStateOpenFourBlackTest();
}

TEST_F(BitBoardTest, GetOpenStateOpenFourWhiteTest)
{
  GetOpenStateOpenFourWhiteTest();
}

TEST_F(BitBoardTest, GetOpenStateFourBlackTest)
{
  GetOpenStateFourBlackTest();
}

TEST_F(BitBoardTest, GetOpenStateFourWhiteTest)
{
  GetOpenStateFourWhiteTest();
}

TEST_F(BitBoardTest, GetOpenStateSemiThreeBlackTest)
{
  GetOpenStateSemiThreeBlackTest();
}

TEST_F(BitBoardTest, GetOpenStateSemiThreeWhiteTest)
{
  GetOpenStateSemiThreeWhiteTest();
}

TEST_F(BitBoardTest, EnumerateOpenFourMovesBlackTest)
{
  EnumerateOpenFourMovesBlackTest();
}

TEST_F(BitBoardTest, EnumerateOpenFourMovesWhiteTest)
{
  EnumerateOpenFourMovesWhiteTest();
}

TEST_F(BitBoardTest, EnumerateFourMovesBlackTest)
{
  EnumerateFourMovesBlackTest();
}

TEST_F(BitBoardTest, EnumerateFourMovesWhiteTest1)
{
  EnumerateFourMovesWhiteTest1();
}

TEST_F(BitBoardTest, EnumerateFourMovesWhiteTest2)
{
  EnumerateFourMovesWhiteTest2();
}

TEST_F(BitBoardTest, EnumerateSemiThreeBlackTest)
{
  EnumerateSemiThreeBlackTest();
}

TEST_F(BitBoardTest, EnumerateSemiThreeWhiteTest)
{
  EnumerateSemiThreeWhiteTest();
}

TEST_F(BitBoardTest, GetBoardStateBitTest)
{
  GetBoardStateBitTest();
}

}
