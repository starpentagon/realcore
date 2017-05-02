#include "gtest/gtest.h"

#include "Move.h"
#include "OpenState.h"

using namespace std;

template<class T>
const bool HasValue(const vector<T> &list, const T& value)
{
  const auto find_it = find(list.begin(), list.end(), value);
  return find_it != list.end();
}

namespace realcore
{
class OpenStateTest
: public ::testing::Test
{
public:
  void ConstructorTest(){
    constexpr BoardPosition open_position = 1;
    constexpr BoardPosition pattern_position = 2;
    constexpr size_t pattern_search_index = 3;
    OpenState open_state(kNextOverline, open_position, pattern_position, pattern_search_index);
    
    EXPECT_EQ(kNextOverline, open_state.pattern_);
    EXPECT_EQ(open_position, open_state.open_position_);
    EXPECT_EQ(pattern_position, open_state.pattern_position_);
    EXPECT_EQ(pattern_search_index, open_state.pattern_search_index_);
  }
};

TEST_F(OpenStateTest, ConstructorTest){
  ConstructorTest();
}

TEST_F(OpenStateTest, GetSetCheckPositionListTest)
{
  constexpr BoardPosition open_position = 1;
  constexpr BoardPosition pattern_position = 1;
  constexpr size_t pattern_search_index = 0;

  // XO[BBOO]OX
  OpenState open_state(kNextSemiThreeBlack, open_position, pattern_position, pattern_search_index);

  EXPECT_EQ(2, open_state.GetCheckPosition());
}

TEST_F(OpenStateTest, GetInfluenceAreaTest)
{
  {
    // 長連点, B[OBBB]B
    constexpr OpenStatePattern kPattern = kNextOverline;
    constexpr BoardPosition open_position = 4;
    constexpr BoardPosition pattern_position = 1;
    constexpr size_t pattern_search_index = 3;
    OpenState open_state(kPattern, open_position, pattern_position, pattern_search_index);

    vector<BoardPosition> black_influence_area, white_influence_area;

    open_state.GetInfluenceArea<kBlackTurn>(&black_influence_area);
    EXPECT_TRUE(black_influence_area.empty());

    open_state.GetInfluenceArea<kWhiteTurn>(&white_influence_area);
    ASSERT_EQ(1, white_influence_area.size());
    ASSERT_EQ(open_position, white_influence_area[0]);
  }
  {
    // 達四点(黒), XO[BBOB]OX
    constexpr OpenStatePattern kPattern = kNextOpenFourBlack;
    constexpr BoardPosition pattern_position = 17;
    constexpr BoardPosition open_position = pattern_position + 1;
    constexpr size_t pattern_search_index = 1;
    OpenState open_state(kPattern, open_position, pattern_position, pattern_search_index);

    vector<BoardPosition> black_influence_area, white_influence_area;

    open_state.GetInfluenceArea<kBlackTurn>(&black_influence_area);
    ASSERT_EQ(5, black_influence_area.size());
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position - 2));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position - 1));
    EXPECT_TRUE(HasValue(black_influence_area, open_position));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position + 4));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position + 5));

    open_state.GetInfluenceArea<kWhiteTurn>(&white_influence_area);
    ASSERT_EQ(3, white_influence_area.size());
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position - 1));
    EXPECT_TRUE(HasValue(white_influence_area, open_position));
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position + 4));
  }
  {
    // 達四点(白), O[WWOW]O
    constexpr OpenStatePattern kPattern = kNextOpenFourWhite;
    constexpr BoardPosition pattern_position = 17;
    constexpr BoardPosition open_position = pattern_position + 1;
    constexpr size_t pattern_search_index = 1;
    OpenState open_state(kPattern, open_position, pattern_position, pattern_search_index);

    vector<BoardPosition> black_influence_area, white_influence_area;

    open_state.GetInfluenceArea<kBlackTurn>(&black_influence_area);
    ASSERT_EQ(3, black_influence_area.size());
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position - 1));
    EXPECT_TRUE(HasValue(black_influence_area, open_position));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position + 4));

    open_state.GetInfluenceArea<kWhiteTurn>(&white_influence_area);
    ASSERT_EQ(3, white_influence_area.size());
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position - 1));
    EXPECT_TRUE(HasValue(white_influence_area, open_position));
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position + 4));
  }
  {
    // 四ノビ点(黒), X[BBBOO]X
    constexpr OpenStatePattern kPattern = kNextFourBlack;
    constexpr BoardPosition pattern_position = 17;
    constexpr BoardPosition open_position = pattern_position + 1;
    constexpr size_t pattern_search_index = 0;
    OpenState open_state(kPattern, open_position, pattern_position, pattern_search_index);

    vector<BoardPosition> black_influence_area, white_influence_area;

    open_state.GetInfluenceArea<kBlackTurn>(&black_influence_area);
    ASSERT_EQ(4, black_influence_area.size());
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position - 1));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position + 1));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position + 5));

    open_state.GetInfluenceArea<kWhiteTurn>(&white_influence_area);
    ASSERT_EQ(2, white_influence_area.size());
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position));
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position + 1));
  }
  {
    // 四ノビ点(白), [WWWOO]
    constexpr OpenStatePattern kPattern = kNextFourWhite;
    constexpr BoardPosition pattern_position = 17;
    constexpr BoardPosition open_position = pattern_position + 1;
    constexpr size_t pattern_search_index = 0;
    OpenState open_state(kPattern, open_position, pattern_position, pattern_search_index);

    vector<BoardPosition> black_influence_area, white_influence_area;

    open_state.GetInfluenceArea<kBlackTurn>(&black_influence_area);
    ASSERT_EQ(2, black_influence_area.size());
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position + 1));

    open_state.GetInfluenceArea<kWhiteTurn>(&white_influence_area);
    ASSERT_EQ(2, white_influence_area.size());
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position));
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position + 1));
  }
  {
    // 見かけの三ノビ点(黒), XO[OOBB]OX
    constexpr OpenStatePattern kPattern = kNextSemiThreeBlack;
    constexpr BoardPosition pattern_position = 17;
    constexpr BoardPosition open_position = pattern_position + 2;
    constexpr size_t pattern_search_index = 5;
    OpenState open_state(kPattern, open_position, pattern_position, pattern_search_index);

    vector<BoardPosition> black_influence_area, white_influence_area;

    open_state.GetInfluenceArea<kBlackTurn>(&black_influence_area);
    ASSERT_EQ(6, black_influence_area.size());
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position - 2));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position - 1));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position + 2));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position + 3));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position + 4));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position + 5));

    open_state.GetInfluenceArea<kWhiteTurn>(&white_influence_area);
    ASSERT_EQ(4, white_influence_area.size());
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position - 1));
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position + 2));
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position + 3));
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position + 4));
  }
  {
    // 見かけの三ノビ点(白), O[OOWW]O
    constexpr OpenStatePattern kPattern = kNextSemiThreeWhite;
    constexpr BoardPosition pattern_position = 17;
    constexpr BoardPosition open_position = pattern_position + 2;
    constexpr size_t pattern_search_index = 5;
    OpenState open_state(kPattern, open_position, pattern_position, pattern_search_index);

    vector<BoardPosition> black_influence_area, white_influence_area;

    open_state.GetInfluenceArea<kBlackTurn>(&black_influence_area);
    ASSERT_EQ(4, black_influence_area.size());
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position - 1));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position + 2));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position + 3));
    EXPECT_TRUE(HasValue(black_influence_area, pattern_position + 4));

    open_state.GetInfluenceArea<kWhiteTurn>(&white_influence_area);
    ASSERT_EQ(4, white_influence_area.size());
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position - 1));
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position + 2));
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position + 3));
    EXPECT_TRUE(HasValue(white_influence_area, pattern_position + 4));
  }
}

TEST_F(OpenStateTest, IsInfluenceMove)
{
  const auto in_board_move_list = GetAllInBoardMove();
  constexpr BoardPosition open_position = 1;
  constexpr size_t pattern_search_index = 0;

  {
    // kNextOverline
    constexpr BoardPosition board_position = 18;    // x = 2, y = 1

    OpenState open_state(kNextOverline, open_position, board_position, pattern_search_index);

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
    OpenState open_state(kNextOpenFourBlack, open_position, board_position, pattern_search_index);

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
    OpenState open_state(kNextOpenFourWhite, open_position, board_position, pattern_search_index);

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
    OpenState open_state(kNextFourBlack, open_position, board_position, pattern_search_index);

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
    OpenState open_state(kNextFourWhite, open_position, board_position, pattern_search_index);

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
    OpenState open_state(kNextSemiThreeBlack, open_position, board_position, pattern_search_index);

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
    OpenState open_state(kNextSemiThreeWhite, open_position, board_position, pattern_search_index);

    for(const auto move : in_board_move_list){
      const bool expect_black_result = (move == kMoveBI) || (move == kMoveCJ) || (move == kMoveDK) || (move == kMoveEL) || (move == kMoveFM) || (move == kMoveGN);
      const bool expect_white_result = (move == kMoveBI) || (move == kMoveCJ) || (move == kMoveDK) || (move == kMoveEL) || (move == kMoveFM) || (move == kMoveGN);

      const bool is_influence_black = open_state.IsInfluenceMove<kBlackTurn>(move);
      ASSERT_EQ(expect_black_result, is_influence_black);

      const bool is_influence_white = open_state.IsInfluenceMove<kWhiteTurn>(move);
      ASSERT_EQ(expect_white_result, is_influence_white);
    }
  }
  {
    // move = kNullMove
    constexpr BoardPosition board_position = 18;    // x = 2, y = 1
    OpenState open_state(kNextOverline, open_position, board_position, pattern_search_index);

    const bool is_influence_black = open_state.IsInfluenceMove<kBlackTurn>(kNullMove);
    ASSERT_FALSE(is_influence_black);

    const bool is_influence_white = open_state.IsInfluenceMove<kWhiteTurn>(kNullMove);
    ASSERT_FALSE(is_influence_white);
  }
}

TEST_F(OpenStateTest, GetOpenPositionTest)
{
  constexpr BoardPosition open_position = 1;
  constexpr BoardPosition pattern_position = 2;
  constexpr size_t pattern_search_index = 0;

  OpenState open_state(kNextOverline, open_position, pattern_position, pattern_search_index);
  EXPECT_EQ(open_position, open_state.GetOpenPosition());
}

TEST_F(OpenStateTest, GetPatternPositionTest)
{
  constexpr BoardPosition open_position = 1;
  constexpr BoardPosition pattern_position = 2;
  constexpr size_t pattern_search_index = 0;

  OpenState open_state(kNextOverline, open_position, pattern_position, pattern_search_index);
  EXPECT_EQ(pattern_position, open_state.GetPatternPosition());
}

TEST_F(OpenStateTest, IsEqualTest)
{
  constexpr BoardPosition open_position_1 = 0, open_position_2 = 0, open_position_3 = 1;
  constexpr BoardPosition pattern_position_1 = 1, pattern_position_2 = 1, pattern_position_3 = 2;
  constexpr size_t pattern_search_index_1 = 3, pattern_search_index_2 = 3, pattern_search_index_3 = 4;

  constexpr OpenStatePattern pattern = kNextSemiThreeBlack;

  OpenState open_state_1(pattern, open_position_1, pattern_position_1, pattern_search_index_1);
  OpenState open_state_2(pattern, open_position_2, pattern_position_2, pattern_search_index_2);
  OpenState open_state_3(pattern, open_position_3, pattern_position_3, pattern_search_index_3);

  EXPECT_TRUE(IsEqual(open_state_1, open_state_2));
  EXPECT_FALSE(IsEqual(open_state_1, open_state_3));
}

TEST_F(OpenStateTest, CompareOperTest)
{
  constexpr OpenStatePattern pattern = kNextSemiThreeBlack;
  
  constexpr BoardPosition open_position_1 = 0, open_position_2 = 0, open_position_3 = 1;
  constexpr BoardPosition pattern_position_1 = 1, pattern_position_2 = 1, pattern_position_3 = 2;
  constexpr size_t pattern_search_index_1 = 3, pattern_search_index_2 = 3, pattern_search_index_3 = 4;

  OpenState open_state_1(pattern, open_position_1, pattern_position_1, pattern_search_index_1);
  OpenState open_state_2(pattern, open_position_2, pattern_position_2, pattern_search_index_2);
  OpenState open_state_3(pattern, open_position_3, pattern_position_3, pattern_search_index_3);

  EXPECT_TRUE(open_state_1 == open_state_2);
  EXPECT_TRUE(open_state_1 != open_state_3);
}

TEST_F(OpenStateTest, CopyTest)
{
  constexpr OpenStatePattern pattern = kNextSemiThreeBlack;

  constexpr BoardPosition open_position_1 = 0, open_position_2 = 1;
  constexpr BoardPosition pattern_position_1 = 1, pattern_position_2 = 2;
  constexpr size_t pattern_search_index_1 = 3, pattern_search_index_2 = 4;

  OpenState open_state_1(pattern, open_position_1, pattern_position_1, pattern_search_index_1);
  OpenState open_state_2(pattern, open_position_2, pattern_position_2, pattern_search_index_2);

  EXPECT_FALSE(IsEqual(open_state_1, open_state_2));
  Copy(open_state_1, &open_state_2);

  EXPECT_TRUE(IsEqual(open_state_1, open_state_2)); 
}

TEST_F(OpenStateTest, AssignOperTest)
{
  constexpr OpenStatePattern pattern = kNextSemiThreeBlack;

  constexpr BoardPosition open_position_1 = 0, open_position_2 = 1;
  constexpr BoardPosition pattern_position_1 = 1, pattern_position_2 = 2;
  constexpr size_t pattern_search_index_1 = 3, pattern_search_index_2 = 4;

  OpenState open_state_1(pattern, open_position_1, pattern_position_1, pattern_search_index_1);
  OpenState open_state_2(pattern, open_position_2, pattern_position_2, pattern_search_index_2);

  EXPECT_TRUE(open_state_1 != open_state_2);
  open_state_2 = open_state_1;

  EXPECT_TRUE(open_state_1 == open_state_2);
}

TEST_F(OpenStateTest, CopyConstructorTest)
{
  constexpr OpenStatePattern pattern = kNextSemiThreeBlack;

  constexpr BoardPosition open_position_1 = 0;
  constexpr BoardPosition pattern_position_1 = 1;
  constexpr size_t pattern_search_index_1 = 2;

  OpenState open_state_1(pattern, open_position_1, pattern_position_1, pattern_search_index_1);

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

TEST_F(OpenStateTest, UpdateOpenStateFlagTest)
{
  ASSERT_TRUE(kUpdateForbiddenCheck[kNextOverline]);
  ASSERT_TRUE(kUpdateForbiddenCheck[kNextOpenFourBlack]);
  ASSERT_FALSE(kUpdateForbiddenCheck[kNextOpenFourWhite]);
  ASSERT_TRUE(kUpdateForbiddenCheck[kNextFourBlack]);
  ASSERT_FALSE(kUpdateForbiddenCheck[kNextFourWhite]);
  ASSERT_TRUE(kUpdateForbiddenCheck[kNextSemiThreeBlack]);
  ASSERT_FALSE(kUpdateForbiddenCheck[kNextSemiThreeWhite]);
}

}   // namespace realcore
