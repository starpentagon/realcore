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
    OpenState<kNextOverline> open_state(open_position, pattern_position);
    
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
  OpenState<kNextSemiThreeBlack> open_state(open_position, pattern_position);

  CheckPositionList check_position_list{{0}};
  EXPECT_EQ(check_position_list, open_state.GetCheckPositionList());

  check_position_list[0] = 2;
  open_state.SetCheckPositionList(check_position_list);
  EXPECT_EQ(check_position_list, open_state.GetCheckPositionList());
}

TEST_F(OpenStateTest, GetSetGuardPositionListTest)
{
  constexpr BoardPosition open_position = 1;
  constexpr BoardPosition pattern_position = 1;
  OpenState<kNextSemiThreeBlack> open_state(open_position, pattern_position);

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
    OpenState<kNextOverline> open_state(open_position, board_position);

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
    OpenState<kNextOpenFourBlack> open_state(open_position, board_position);

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
    OpenState<kNextOpenFourWhite> open_state(open_position, board_position);

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
    OpenState<kNextFourBlack> open_state(open_position, board_position);

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
    OpenState<kNextFourWhite> open_state(open_position, board_position);

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
    OpenState<kNextSemiThreeBlack> open_state(open_position, board_position);

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
    OpenState<kNextSemiThreeWhite> open_state(open_position, board_position);

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

  OpenState<kNextOverline> open_state(open_position, pattern_position);
  EXPECT_EQ(open_position, open_state.GetOpenPosition());
}

TEST_F(OpenStateTest, GetPatternPositionTest)
{
  constexpr BoardPosition open_position = 1;
  constexpr BoardPosition pattern_position = 2;

  OpenState<kNextOverline> open_state(open_position, pattern_position);
  EXPECT_EQ(pattern_position, open_state.GetPatternPosition());
}

TEST_F(OpenStateTest, IsEqualTest)
{
  constexpr BoardPosition open_position_1 = 0, open_position_2 = 0, open_position_3 = 1;
  constexpr BoardPosition pattern_position_1 = 1, pattern_position_2 = 1, pattern_position_3 = 2;

  constexpr OpenStatePattern Pattern = kNextSemiThreeBlack;

  OpenState<Pattern> open_state_1(open_position_1, pattern_position_1);
  OpenState<Pattern> open_state_2(open_position_2, pattern_position_2);
  OpenState<Pattern> open_state_3(open_position_3, pattern_position_3);

  EXPECT_TRUE(IsEqual<Pattern>(open_state_1, open_state_2));
  EXPECT_FALSE(IsEqual<Pattern>(open_state_1, open_state_3));

  CheckPositionList check_position_list{{0}};
  check_position_list[0] = 1;

  open_state_1.SetCheckPositionList(check_position_list);
  EXPECT_FALSE(IsEqual<Pattern>(open_state_1, open_state_2));

  open_state_2.SetCheckPositionList(check_position_list);
  EXPECT_TRUE(IsEqual<Pattern>(open_state_1, open_state_2));
  
  GuardPositionList guard_position_list{{0}};
  guard_position_list[0] = 1;

  open_state_1.SetGuardPositionList(guard_position_list);
  EXPECT_FALSE(IsEqual<Pattern>(open_state_1, open_state_2));

  open_state_2.SetGuardPositionList(guard_position_list);
  EXPECT_TRUE(IsEqual<Pattern>(open_state_1, open_state_2));
}

TEST_F(OpenStateTest, CompareOperTest)
{
  constexpr OpenStatePattern Pattern = kNextSemiThreeBlack;
  
  constexpr BoardPosition open_position_1 = 0, open_position_2 = 0, open_position_3 = 1;
  constexpr BoardPosition pattern_position_1 = 1, pattern_position_2 = 1, pattern_position_3 = 2;
  OpenState<Pattern> open_state_1(open_position_1, pattern_position_1);
  OpenState<Pattern> open_state_2(open_position_2, pattern_position_2);
  OpenState<Pattern> open_state_3(open_position_3, pattern_position_3);

  EXPECT_TRUE(open_state_1 == open_state_2);
  EXPECT_TRUE(open_state_1 != open_state_3);

  CheckPositionList check_position_list{{0}};
  check_position_list[0] = 1;

  open_state_1.SetCheckPositionList(check_position_list);
  EXPECT_TRUE(open_state_1 != open_state_2);

  open_state_2.SetCheckPositionList(check_position_list);
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
  constexpr OpenStatePattern Pattern = kNextSemiThreeBlack;

  constexpr BoardPosition open_position_1 = 0, open_position_2 = 1;
  constexpr BoardPosition pattern_position_1 = 1, pattern_position_2 = 2;
  OpenState<Pattern> open_state_1(open_position_1, pattern_position_1);
  OpenState<Pattern> open_state_2(open_position_2, pattern_position_2);

  CheckPositionList check_position_list{{0}};
  check_position_list[0] = 1;
  open_state_1.SetCheckPositionList(check_position_list);

  GuardPositionList guard_position_list{{0}};
  guard_position_list[0] = 1;
  open_state_1.SetGuardPositionList(guard_position_list);

  EXPECT_FALSE(IsEqual<Pattern>(open_state_1, open_state_2));
  Copy<Pattern>(open_state_1, &open_state_2);

  EXPECT_TRUE(IsEqual<Pattern>(open_state_1, open_state_2)); 
}

TEST_F(OpenStateTest, AssignOperTest)
{
  constexpr OpenStatePattern Pattern = kNextSemiThreeBlack;

  constexpr BoardPosition open_position_1 = 0, open_position_2 = 1;
  constexpr BoardPosition pattern_position_1 = 1, pattern_position_2 = 2;
  OpenState<Pattern> open_state_1(open_position_1, pattern_position_1);
  OpenState<Pattern> open_state_2(open_position_2, pattern_position_2);

  CheckPositionList check_position_list{{0}};
  check_position_list[0] = 0;
  open_state_1.SetCheckPositionList(check_position_list);

  GuardPositionList guard_position_list{{0}};
  guard_position_list[0] = 1;
  open_state_1.SetGuardPositionList(guard_position_list);

  EXPECT_TRUE(open_state_1 != open_state_2);
  open_state_2 = open_state_1;

  EXPECT_TRUE(open_state_1 == open_state_2);
}

TEST_F(OpenStateTest, CopyConstructorTest)
{
  constexpr OpenStatePattern Pattern = kNextSemiThreeBlack;

  constexpr BoardPosition open_position_1 = 0;
  constexpr BoardPosition pattern_position_1 = 1;
  OpenState<Pattern> open_state_1(open_position_1, pattern_position_1);

  CheckPositionList check_position_list{{0}};
  check_position_list[0] = 0;
  open_state_1.SetCheckPositionList(check_position_list);

  GuardPositionList guard_position_list{{0}};
  guard_position_list[0] = 1;
  open_state_1.SetGuardPositionList(guard_position_list);

  OpenState<Pattern> open_state_2(open_state_1);

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
