#include <string>

#include "gtest/gtest.h"

#include "MoveTree.h"

using namespace std;

namespace realcore
{

class MoveTreeBaseTest
: public ::testing::Test
{
public:
  void DefaultConstructorTest(){
    MoveTree move_tree;

    ASSERT_EQ(1, move_tree.tree_.size());
    
    const auto &root_node = move_tree.tree_[0];
    EXPECT_EQ(kNullNodeIndex, root_node.GetParentIndex());
    EXPECT_EQ(kInvalidMove, root_node.GetMove());

    ASSERT_EQ(kRootNodeIndex, move_tree.current_node_index_);
  }
};

TEST_F(MoveTreeBaseTest, DefaultConstructorTest)
{
  DefaultConstructorTest();
}

}   // namespace realcore
