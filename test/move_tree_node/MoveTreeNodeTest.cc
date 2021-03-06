#include <string>

#include "gtest/gtest.h"

#include "MoveTreeNode.h"

using namespace std;

namespace realcore
{

class MoveTreeNodeTest
: public ::testing::Test
{
public:
};

TEST_F(MoveTreeNodeTest, DefaultConstructorTest)
{
  MoveTreeNode<string> move_tree_node(kRootNodeIndex, kNullMove);

  EXPECT_EQ(kRootNodeIndex, move_tree_node.GetParentIndex());
  EXPECT_EQ(kNullNodeIndex, move_tree_node.GetNextSiblingIndex());
  EXPECT_EQ(kNullNodeIndex, move_tree_node.GetFirstChildIndex());
  EXPECT_EQ(kNullMove, move_tree_node.GetMove());
  EXPECT_EQ("", move_tree_node.GetAdditionalData());
}

TEST_F(MoveTreeNodeTest, GetSetParentIndexTest)
{
  constexpr MoveNodeIndex node_index = 1;
  MoveTreeNode<string> move_tree_node(node_index, kNullMove);

  EXPECT_EQ(node_index, move_tree_node.GetParentIndex());

  move_tree_node.SetParentIndex(2);
  EXPECT_EQ(2, move_tree_node.GetParentIndex());
}

TEST_F(MoveTreeNodeTest, GetSetNextSiblingIndexTest)
{
  constexpr MoveNodeIndex node_index = 1;
  MoveTreeNode<string> move_tree_node(kRootNodeIndex, kNullMove);

  move_tree_node.SetNextSiblingIndex(node_index);
  EXPECT_EQ(node_index, move_tree_node.GetNextSiblingIndex());
}

TEST_F(MoveTreeNodeTest, GetSetFirstChildIndexTest)
{
  constexpr MoveNodeIndex node_index = 1;
  MoveTreeNode<string> move_tree_node(kRootNodeIndex, kNullMove);

  move_tree_node.SetFirstChildIndex(node_index);
  EXPECT_EQ(node_index, move_tree_node.GetFirstChildIndex());
}

TEST_F(MoveTreeNodeTest, HasChildTest)
{
  MoveTreeNode<string> move_tree_node(kRootNodeIndex, kNullMove);
  
  EXPECT_FALSE(move_tree_node.HasChild());

  constexpr MoveNodeIndex node_index = 1;
  move_tree_node.SetFirstChildIndex(node_index);

  EXPECT_TRUE(move_tree_node.HasChild());
}

TEST_F(MoveTreeNodeTest, GetMoveTest)
{
  constexpr MovePosition move = kMoveHH;
  MoveTreeNode<string> move_tree_node(kRootNodeIndex, move);

  EXPECT_EQ(move, move_tree_node.GetMove());
}

TEST_F(MoveTreeNodeTest, GetSetAdditionalDataTest)
{
  MoveTreeNode<string> move_tree_node(kRootNodeIndex, kNullMove);

  move_tree_node.SetAdditionalData("test data");
  EXPECT_EQ("test data", move_tree_node.GetAdditionalData());
}

TEST_F(MoveTreeNodeTest, CompareOperTest){
  {
    // equal
    MoveTreeNode<string> move_tree_node_1(kRootNodeIndex, kNullMove);
    MoveTreeNode<string> move_tree_node_2(kRootNodeIndex, kNullMove);

    EXPECT_TRUE(move_tree_node_1 == move_tree_node_2);
    EXPECT_FALSE(move_tree_node_1 != move_tree_node_2);
  }
  {
    // parent_index_(not equal)
    MoveTreeNode<string> move_tree_node_1(kRootNodeIndex, kNullMove);
    MoveTreeNode<string> move_tree_node_2(kRootNodeIndex + 1, kNullMove);

    EXPECT_FALSE(move_tree_node_1 == move_tree_node_2);
    EXPECT_TRUE(move_tree_node_1 != move_tree_node_2);
  }
  {
    // next_sibling_index_
    MoveTreeNode<string> move_tree_node_1(kRootNodeIndex, kNullMove);
    MoveTreeNode<string> move_tree_node_2(kRootNodeIndex, kNullMove);
      
    EXPECT_TRUE(move_tree_node_1 == move_tree_node_2);
    EXPECT_FALSE(move_tree_node_1 != move_tree_node_2);

    move_tree_node_1.SetNextSiblingIndex(1);
    EXPECT_FALSE(move_tree_node_1 == move_tree_node_2);
    EXPECT_TRUE(move_tree_node_1 != move_tree_node_2);
  }
  {
    // first_child_index_
    MoveTreeNode<string> move_tree_node_1(kRootNodeIndex, kNullMove);
    MoveTreeNode<string> move_tree_node_2(kRootNodeIndex, kNullMove);
      
    EXPECT_TRUE(move_tree_node_1 == move_tree_node_2);
    EXPECT_FALSE(move_tree_node_1 != move_tree_node_2);

    move_tree_node_1.SetFirstChildIndex(1);
    EXPECT_FALSE(move_tree_node_1 == move_tree_node_2);
    EXPECT_TRUE(move_tree_node_1 != move_tree_node_2);
  }
  {
    // move_(not equal)
    MoveTreeNode<string> move_tree_node_1(kRootNodeIndex, kMoveHH);
    MoveTreeNode<string> move_tree_node_2(kRootNodeIndex, kNullMove);

    EXPECT_FALSE(move_tree_node_1 == move_tree_node_2);
    EXPECT_TRUE(move_tree_node_1 != move_tree_node_2);
  }
  {
    // additional_data_
    MoveTreeNode<string> move_tree_node_1(kRootNodeIndex, kNullMove);
    MoveTreeNode<string> move_tree_node_2(kRootNodeIndex, kNullMove);

    move_tree_node_1.SetAdditionalData("test_data");
    EXPECT_FALSE(move_tree_node_1 == move_tree_node_2);
    EXPECT_TRUE(move_tree_node_1 != move_tree_node_2);
  }
}

TEST_F(MoveTreeNodeTest, AssignOperTest){
    MoveTreeNode<string> move_tree_node_1(kRootNodeIndex + 1, kMoveHH);
    move_tree_node_1.SetNextSiblingIndex(2);
    move_tree_node_1.SetFirstChildIndex(3);
    move_tree_node_1.SetAdditionalData("test_data");

    MoveTreeNode<string> move_tree_node_2(kRootNodeIndex, kNullMove);
    move_tree_node_2 = move_tree_node_1;

    EXPECT_TRUE(move_tree_node_1 == move_tree_node_2);  
}

}   // namespace realcore
