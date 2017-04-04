#include "gtest/gtest.h"

#include "OpenState.h"

using namespace std;

namespace realcore
{
class OpenStateTest
: public ::testing::Test
{
public:
  void ConstructorTest(){
    constexpr BoardPosition pattern_position = 1;
    OpenState<kNextOverline> open_state(pattern_position);
    
    EXPECT_EQ(pattern_position, open_state.pattern_position_);
    EXPECT_TRUE(open_state.check_position_list_.empty());
    EXPECT_TRUE(open_state.guard_position_list_.empty());

    {
      // kNextOverline
      OpenState<kNextOverline> open_state(pattern_position);
      EXPECT_EQ(0, open_state.check_position_list_.capacity());
      EXPECT_EQ(0, open_state.guard_position_list_.capacity());
    }
    {
      // kNextOpenFourBlack
      OpenState<kNextOpenFourBlack> open_state(pattern_position);
      EXPECT_EQ(0, open_state.check_position_list_.capacity());
      EXPECT_EQ(0, open_state.guard_position_list_.capacity());
    }
    {
      // kNextOpenFourWhite
      OpenState<kNextOpenFourWhite> open_state(pattern_position);
      EXPECT_EQ(0, open_state.check_position_list_.capacity());
      EXPECT_EQ(0, open_state.guard_position_list_.capacity());
    }
    {
      // kNextFourBlack
      OpenState<kNextFourBlack> open_state(pattern_position);
      EXPECT_EQ(0, open_state.check_position_list_.capacity());
      EXPECT_EQ(1, open_state.guard_position_list_.capacity());
    }
    {
      // kNextFourWhite
      OpenState<kNextFourWhite> open_state(pattern_position);
      EXPECT_EQ(0, open_state.check_position_list_.capacity());
      EXPECT_EQ(1, open_state.guard_position_list_.capacity());
    }
    {
      // kNextSemiThreeBlack
      OpenState<kNextSemiThreeBlack> open_state(pattern_position);
      EXPECT_EQ(2, open_state.check_position_list_.capacity());
      EXPECT_EQ(4, open_state.guard_position_list_.capacity());
    }
    {
      // kNextSemiThreeWhite
      OpenState<kNextSemiThreeWhite> open_state(pattern_position);
      EXPECT_EQ(0, open_state.check_position_list_.capacity());
      EXPECT_EQ(4, open_state.guard_position_list_.capacity());
    }
  }
};

TEST_F(OpenStateTest, ConstructorTest){
  ConstructorTest();
}

TEST_F(OpenStateTest, GetSetCheckPositionListTest)
{
  constexpr BoardPosition pattern_position = 1;
  OpenState<kNextOverline> open_state(pattern_position);

  vector<BoardPosition> check_position_list;
  EXPECT_EQ(check_position_list, open_state.GetCheckPositionList());

  check_position_list.push_back(2);
  open_state.SetCheckPositionList(check_position_list);
  EXPECT_EQ(check_position_list, open_state.GetCheckPositionList());
}

TEST_F(OpenStateTest, GetSetGuardPositionListTest)
{
  constexpr BoardPosition pattern_position = 1;
  OpenState<kNextOverline> open_state(pattern_position);

  vector<BoardPosition> guard_position_list;
  EXPECT_EQ(guard_position_list, open_state.GetGuardPositionList());

  guard_position_list.push_back(3);
  open_state.SetGuardPositionList(guard_position_list);
  EXPECT_EQ(guard_position_list, open_state.GetGuardPositionList());
}

}   // namespace realcore
