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
