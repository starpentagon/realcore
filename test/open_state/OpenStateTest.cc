#include "gtest/gtest.h"

#include "Move.h"
#include "OpenState.h"

using namespace std;

namespace realcore
{
class OpenStateTest
: public ::testing::Test
{
public:
  void ConstructorTest(){
    constexpr BoardPosition open_position = 1;
    constexpr BoardPosition pattern_position = 1;
    OpenState open_state(kNextOverline, open_position, pattern_position);
    
    EXPECT_EQ(pattern_position, open_state.pattern_position_);
  }
};

TEST_F(OpenStateTest, ConstructorTest){
  ConstructorTest();
}

TEST_F(OpenStateTest, GetSetCheckPositionListTest)
{
  constexpr BoardPosition open_position = 1;
  constexpr BoardPosition pattern_position = 1;
  OpenState open_state(kNextSemiThreeBlack, open_position, pattern_position);

  EXPECT_EQ(0, open_state.GetCheckPosition());

  constexpr BoardPosition check_position = 2;
  open_state.SetCheckPosition(check_position);
  EXPECT_EQ(check_position, open_state.GetCheckPosition());
}

TEST_F(OpenStateTest, GetSetGuardPositionListTest)
{
  constexpr BoardPosition open_position = 1;
  constexpr BoardPosition pattern_position = 1;
  OpenState open_state(kNextSemiThreeBlack, open_position, pattern_position);

  GuardPositionList guard_position_list{{0}};
  EXPECT_EQ(guard_position_list, open_state.GetGuardPositionList());

  guard_position_list[0] = 3;
  open_state.SetGuardPositionList(guard_position_list);
  EXPECT_EQ(guard_position_list, open_state.GetGuardPositionList());
}

TEST_F(OpenStateTest, IsInfluenceMove)
{
  const auto in_board_move_list = GetAllInBoardMove();
  constexpr BoardPosition open_position = 1;

  {
    // kNextOverline
    constexpr BoardPosition board_position = 18;    // x = 2, y = 1
    OpenState open_state(kNextOverline, open_position, board_position);

    for(const auto move : in_board_move_list){
      const bool expect_black_result = false;
      const bool expect_white_result = (move == kMoveBA) || (move == kMoveCA) || (move == kMoveDA) || (move == kMoveEA);

      const bool is_influence_black = open_state.IsInfluenceMove<kBlackTurn>(move);
      EXPECT_EQ(expect_black_result, is_influence_black);

      const bool is_influence_white = open_state.IsInfluenceMove<kWhiteTurn>(move);
      EXPECT_EQ(expect_white_result, is_influence_white);
    }
  }
  {
    // kNextOpenFourBlack
    constexpr BoardPosition board_position = 274;    // x = 1, y = 2
    OpenState open_state(kNextOpenFourBlack, open_position, board_position);

    for(const auto move : in_board_move_list){
      const bool expect_black_result = (move == kMoveAA) || (move == kMoveAB) || (move == kMoveAC) || (move == kMoveAD) || (move == kMoveAE) || (move == kMoveAF) || (move == kMoveAG);
      const bool expect_white_result = (move == kMoveAA) || (move == kMoveAB) || (move == kMoveAC) || (move == kMoveAD) || (move == kMoveAE) || (move == kMoveAF);

      const bool is_influence_black = open_state.IsInfluenceMove<kBlackTurn>(move);
      EXPECT_EQ(expect_black_result, is_influence_black);

      const bool is_influence_white = open_state.IsInfluenceMove<kWhiteTurn>(move);
      EXPECT_EQ(expect_white_result, is_influence_white);
    }
  }
  {
    // kNextOpenFourWhite
    constexpr BoardPosition board_position = 274;    // x = 1, y = 2
    OpenState open_state(kNextOpenFourWhite, open_position, board_position);

    for(const auto move : in_board_move_list){
      const bool expect_black_result = (move == kMoveAA) || (move == kMoveAB) || (move == kMoveAC) || (move == kMoveAD) || (move == kMoveAE) || (move == kMoveAF);
      const bool expect_white_result = (move == kMoveAA) || (move == kMoveAB) || (move == kMoveAC) || (move == kMoveAD) || (move == kMoveAE) || (move == kMoveAF);

      const bool is_influence_black = open_state.IsInfluenceMove<kBlackTurn>(move);
      EXPECT_EQ(expect_black_result, is_influence_black);

      const bool is_influence_white = open_state.IsInfluenceMove<kWhiteTurn>(move);
      EXPECT_EQ(expect_white_result, is_influence_white);
    }
  }
  {
    // kNextFourBlack
    constexpr BoardPosition board_position = 609;    // x = 6, y = 2
    OpenState open_state(kNextFourBlack, open_position, board_position);

    for(const auto move : in_board_move_list){
      const bool expect_black_result = (move == kMoveGA) || (move == kMoveFB) || (move == kMoveEC) || (move == kMoveDD) || (move == kMoveCE) || (move == kMoveBF) || (move == kMoveAG);
      const bool expect_white_result = (move == kMoveFB) || (move == kMoveEC) || (move == kMoveDD) || (move == kMoveCE) || (move == kMoveBF);

      const bool is_influence_black = open_state.IsInfluenceMove<kBlackTurn>(move);
      EXPECT_EQ(expect_black_result, is_influence_black);

      const bool is_influence_white = open_state.IsInfluenceMove<kWhiteTurn>(move);
      EXPECT_EQ(expect_white_result, is_influence_white);
    }
  }
  {
    // kNextFourWhite
    constexpr BoardPosition board_position = 609;    // x = 6, y = 2
    OpenState open_state(kNextFourWhite, open_position, board_position);

    for(const auto move : in_board_move_list){
      const bool expect_black_result = (move == kMoveFB) || (move == kMoveEC) || (move == kMoveDD) || (move == kMoveCE) || (move == kMoveBF);
      const bool expect_white_result = (move == kMoveFB) || (move == kMoveEC) || (move == kMoveDD) || (move == kMoveCE) || (move == kMoveBF);

      const bool is_influence_black = open_state.IsInfluenceMove<kBlackTurn>(move);
      EXPECT_EQ(expect_black_result, is_influence_black);

      const bool is_influence_white = open_state.IsInfluenceMove<kWhiteTurn>(move);
      EXPECT_EQ(expect_white_result, is_influence_white);
    }
  }
  {
    // kNextSemiThreeBlack
    constexpr BoardPosition board_position = 857;    // x = 3, y = 10
    OpenState open_state(kNextSemiThreeBlack, open_position, board_position);

    for(const auto move : in_board_move_list){
      const bool expect_black_result = (move == kMoveAH) || (move == kMoveBI) || (move == kMoveCJ) || (move == kMoveDK) || (move == kMoveEL) || (move == kMoveFM) || (move == kMoveGN) || (move == kMoveHO);
      const bool expect_white_result = (move == kMoveBI) || (move == kMoveCJ) || (move == kMoveDK) || (move == kMoveEL) || (move == kMoveFM) || (move == kMoveGN);

      const bool is_influence_black = open_state.IsInfluenceMove<kBlackTurn>(move);
      ASSERT_EQ(expect_black_result, is_influence_black);

      const bool is_influence_white = open_state.IsInfluenceMove<kWhiteTurn>(move);
      ASSERT_EQ(expect_white_result, is_influence_white);
    }
  }
  {
    // kNextSemiThreeWhite
    constexpr BoardPosition board_position = 857;    // x = 3, y = 10
    OpenState open_state(kNextSemiThreeWhite, open_position, board_position);

    for(const auto move : in_board_move_list){
      const bool expect_black_result = (move == kMoveBI) || (move == kMoveCJ) || (move == kMoveDK) || (move == kMoveEL) || (move == kMoveFM) || (move == kMoveGN);
      const bool expect_white_result = (move == kMoveBI) || (move == kMoveCJ) || (move == kMoveDK) || (move == kMoveEL) || (move == kMoveFM) || (move == kMoveGN);

      const bool is_influence_black = open_state.IsInfluenceMove<kBlackTurn>(move);
      ASSERT_EQ(expect_black_result, is_influence_black);

      const bool is_influence_white = open_state.IsInfluenceMove<kWhiteTurn>(move);
      ASSERT_EQ(expect_white_result, is_influence_white);
    }
  }
}

TEST_F(OpenStateTest, GetOpenPositionTest)
{
  constexpr BoardPosition open_position = 1;
  constexpr BoardPosition pattern_position = 2;

  OpenState open_state(kNextOverline, open_position, pattern_position);
  EXPECT_EQ(open_position, open_state.GetOpenPosition());
}

TEST_F(OpenStateTest, GetPatternPositionTest)
{
  constexpr BoardPosition open_position = 1;
  constexpr BoardPosition pattern_position = 2;

  OpenState open_state(kNextOverline, open_position, pattern_position);
  EXPECT_EQ(pattern_position, open_state.GetPatternPosition());
}

TEST_F(OpenStateTest, IsEqualTest)
{
  constexpr BoardPosition open_position_1 = 0, open_position_2 = 0, open_position_3 = 1;
  constexpr BoardPosition pattern_position_1 = 1, pattern_position_2 = 1, pattern_position_3 = 2;
  constexpr BoardPosition check_position_1 = 2, check_position_2 = 2;

  constexpr OpenStatePattern pattern = kNextSemiThreeBlack;

  OpenState open_state_1(pattern, open_position_1, pattern_position_1);
  OpenState open_state_2(pattern, open_position_2, pattern_position_2);
  OpenState open_state_3(pattern, open_position_3, pattern_position_3);

  EXPECT_TRUE(IsEqual(open_state_1, open_state_2));
  EXPECT_FALSE(IsEqual(open_state_1, open_state_3));

  open_state_1.SetCheckPosition(check_position_1);
  EXPECT_FALSE(IsEqual(open_state_1, open_state_2));

  open_state_2.SetCheckPosition(check_position_2);
  EXPECT_TRUE(IsEqual(open_state_1, open_state_2));
  
  GuardPositionList guard_position_list{{0}};
  guard_position_list[0] = 1;

  open_state_1.SetGuardPositionList(guard_position_list);
  EXPECT_FALSE(IsEqual(open_state_1, open_state_2));

  open_state_2.SetGuardPositionList(guard_position_list);
  EXPECT_TRUE(IsEqual(open_state_1, open_state_2));
}

TEST_F(OpenStateTest, CompareOperTest)
{
  constexpr OpenStatePattern pattern = kNextSemiThreeBlack;
  
  constexpr BoardPosition open_position_1 = 0, open_position_2 = 0, open_position_3 = 1;
  constexpr BoardPosition pattern_position_1 = 1, pattern_position_2 = 1, pattern_position_3 = 2;
  constexpr BoardPosition check_position = 3;

  OpenState open_state_1(pattern, open_position_1, pattern_position_1);
  OpenState open_state_2(pattern, open_position_2, pattern_position_2);
  OpenState open_state_3(pattern, open_position_3, pattern_position_3);

  EXPECT_TRUE(open_state_1 == open_state_2);
  EXPECT_TRUE(open_state_1 != open_state_3);

  open_state_1.SetCheckPosition(check_position);
  EXPECT_TRUE(open_state_1 != open_state_2);

  open_state_2.SetCheckPosition(check_position);
  EXPECT_TRUE(open_state_1 == open_state_2);
  
  GuardPositionList guard_position_list{{0}};
  guard_position_list[0] = 1;

  open_state_1.SetGuardPositionList(guard_position_list);
  EXPECT_TRUE(open_state_1 != open_state_2);

  open_state_2.SetGuardPositionList(guard_position_list);
  EXPECT_TRUE(open_state_1 == open_state_2);
}

TEST_F(OpenStateTest, CopyTest)
{
  constexpr OpenStatePattern pattern = kNextSemiThreeBlack;

  constexpr BoardPosition open_position_1 = 0, open_position_2 = 1;
  constexpr BoardPosition pattern_position_1 = 1, pattern_position_2 = 2;
  constexpr BoardPosition check_position = 3;

  OpenState open_state_1(pattern, open_position_1, pattern_position_1);
  OpenState open_state_2(pattern, open_position_2, pattern_position_2);

  open_state_1.SetCheckPosition(check_position);

  GuardPositionList guard_position_list{{0}};
  guard_position_list[0] = 1;
  open_state_1.SetGuardPositionList(guard_position_list);

  EXPECT_FALSE(IsEqual(open_state_1, open_state_2));
  Copy(open_state_1, &open_state_2);

  EXPECT_TRUE(IsEqual(open_state_1, open_state_2)); 
}

TEST_F(OpenStateTest, AssignOperTest)
{
  constexpr OpenStatePattern pattern = kNextSemiThreeBlack;

  constexpr BoardPosition open_position_1 = 0, open_position_2 = 1;
  constexpr BoardPosition pattern_position_1 = 1, pattern_position_2 = 2;
  constexpr BoardPosition check_position = 3;

  OpenState open_state_1(pattern, open_position_1, pattern_position_1);
  OpenState open_state_2(pattern, open_position_2, pattern_position_2);

  open_state_1.SetCheckPosition(check_position);

  GuardPositionList guard_position_list{{0}};
  guard_position_list[0] = 1;
  open_state_1.SetGuardPositionList(guard_position_list);

  EXPECT_TRUE(open_state_1 != open_state_2);
  open_state_2 = open_state_1;

  EXPECT_TRUE(open_state_1 == open_state_2);
}

TEST_F(OpenStateTest, CopyConstructorTest)
{
  constexpr OpenStatePattern pattern = kNextSemiThreeBlack;

  constexpr BoardPosition open_position_1 = 0;
  constexpr BoardPosition pattern_position_1 = 1;
  constexpr BoardPosition check_position = 3;

  OpenState open_state_1(pattern, open_position_1, pattern_position_1);

  open_state_1.SetCheckPosition(check_position);

  GuardPositionList guard_position_list{{0}};
  guard_position_list[0] = 1;
  open_state_1.SetGuardPositionList(guard_position_list);

  OpenState open_state_2(open_state_1);

  EXPECT_TRUE(open_state_1 == open_state_2);
}

TEST_F(OpenStateTest, GetPatternPlayerTurnTest)
{
  EXPECT_EQ(kBlackTurn, GetPatternPlayerTurn(kNextOverline));
  EXPECT_EQ(kBlackTurn, GetPatternPlayerTurn(kNextOpenFourBlack));
  EXPECT_EQ(kWhiteTurn, GetPatternPlayerTurn(kNextOpenFourWhite));
  EXPECT_EQ(kBlackTurn, GetPatternPlayerTurn(kNextFourBlack));
  EXPECT_EQ(kWhiteTurn, GetPatternPlayerTurn(kNextFourWhite));
  EXPECT_EQ(kBlackTurn, GetPatternPlayerTurn(kNextSemiThreeBlack));
  EXPECT_EQ(kWhiteTurn, GetPatternPlayerTurn(kNextSemiThreeWhite));
}

}   // namespace realcore
