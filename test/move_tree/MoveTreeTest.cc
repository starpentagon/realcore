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
    
    const auto &root_node = move_tree.tree_.back();
    EXPECT_EQ(kNullNodeIndex, root_node.GetParentIndex());
    EXPECT_EQ(kNullNodeIndex, root_node.GetNextSiblingIndex());
    EXPECT_EQ(kNullNodeIndex, root_node.GetFirstChildIndex());
    EXPECT_EQ(kInvalidMove, root_node.GetMove());

    ASSERT_EQ(kRootNodeIndex, move_tree.current_node_index_);
  }

  void AddChildTest(){
    MoveTree move_tree;

    {
      // @root -> kMoveHH
      move_tree.AddChild(kMoveHH);

      ASSERT_EQ(2, move_tree.tree_.size());
      const auto &child_node = move_tree.tree_.back();

      EXPECT_EQ(kRootNodeIndex, child_node.GetParentIndex());
      EXPECT_EQ(kNullNodeIndex, child_node.GetNextSiblingIndex());
      EXPECT_EQ(kNullNodeIndex, child_node.GetFirstChildIndex());
      EXPECT_EQ(kMoveHH, child_node.GetMove());

      ASSERT_EQ(kRootNodeIndex, move_tree.current_node_index_);
    }
    {
      // @root -> kMoveHH/kMoveHI
      move_tree.AddChild(kMoveHI);

      ASSERT_EQ(3, move_tree.tree_.size());
      const auto &child_node = move_tree.tree_.back();

      EXPECT_EQ(kRootNodeIndex, child_node.GetParentIndex());
      EXPECT_EQ(kNullNodeIndex, child_node.GetNextSiblingIndex());
      EXPECT_EQ(kNullNodeIndex, child_node.GetFirstChildIndex());
      EXPECT_EQ(kMoveHI, child_node.GetMove());

      ASSERT_EQ(kRootNodeIndex, move_tree.current_node_index_);

      const auto &brother_node = move_tree.tree_[1];

      EXPECT_EQ(kRootNodeIndex, brother_node.GetParentIndex());
      EXPECT_EQ(2, brother_node.GetNextSiblingIndex());
      EXPECT_EQ(kNullNodeIndex, brother_node.GetFirstChildIndex());
      EXPECT_EQ(kMoveHH, brother_node.GetMove());
    }

    // root -> @kMoveHH/kMoveHI
    const auto is_move = move_tree.MoveChildNode(kMoveHH);
    ASSERT_TRUE(is_move);
    
    {
      // root -> @kMoveHH -> kMoveHG/kMoveHI
      move_tree.AddChild(kMoveHG);

      ASSERT_EQ(4, move_tree.tree_.size());
      const auto &child_node = move_tree.tree_.back();

      EXPECT_EQ(1, child_node.GetParentIndex());
      EXPECT_EQ(kNullNodeIndex, child_node.GetNextSiblingIndex());
      EXPECT_EQ(kNullNodeIndex, child_node.GetFirstChildIndex());
      EXPECT_EQ(kMoveHG, child_node.GetMove());

      ASSERT_EQ(1, move_tree.current_node_index_);
    }
  }

  void AddChildListTest(){
    MoveTree move_tree;
    MoveList move_list;

    move_list += kMoveHH;
    move_list += kMoveHI;

    move_tree.AddChild(move_list);

    ASSERT_EQ(2, move_tree.size());
    ASSERT_TRUE(move_tree.MoveChildNode(kMoveHH));
    ASSERT_TRUE(move_tree.MoveChildNode(kMoveHI));
  }

  void GetChildNodeIndexTest()
  {
    MoveTree move_tree;
    
    ASSERT_EQ(kNullNodeIndex, move_tree.GetChildNodeIndex(kMoveHH));
    
    move_tree.AddChild(kMoveHH);
    ASSERT_EQ(1, move_tree.GetChildNodeIndex(kMoveHH));
    ASSERT_EQ(kNullNodeIndex, move_tree.GetChildNodeIndex(kMoveHG));
  }

  void GetYoungestChildNodeIndexTest()
  {
    MoveTree move_tree;
    
    move_tree.AddChild(kMoveHH);
    ASSERT_EQ(1, move_tree.GetYoungestChildNodeIndex());
    
    move_tree.AddChild(kMoveHI);
    ASSERT_EQ(2, move_tree.GetYoungestChildNodeIndex());
  }

  void MoveChildNodeTest()
  {
    MoveTree move_tree;
    ASSERT_FALSE(move_tree.MoveChildNode(kMoveHH));

    move_tree.AddChild(kMoveHH);
    ASSERT_TRUE(move_tree.MoveChildNode(kMoveHH));
    ASSERT_EQ(1, move_tree.current_node_index_);
  }

  void MoveChildNodeMoveListTest()
  {
    MoveTree move_tree;
    move_tree.AddChild(kMoveHH);
    move_tree.MoveChildNode(kMoveHH);

    move_tree.AddChild(kMoveHI);
    move_tree.MoveChildNode(kMoveHI);

    move_tree.AddChild(kMoveHJ);
    move_tree.MoveRootNode();

    {
      MoveList move_list("hhhg");
      ASSERT_FALSE(move_tree.MoveChildNode(move_list));
      ASSERT_EQ(kRootNodeIndex, move_tree.current_node_index_);      
    }
    {
      MoveList move_list("hhhi");
      ASSERT_TRUE(move_tree.MoveChildNode(move_list));
      ASSERT_EQ(2, move_tree.current_node_index_);      
    }
  }

  void MoveParentTest()
  {
    MoveTree move_tree;
    move_tree.MoveParent();

    ASSERT_EQ(kRootNodeIndex, move_tree.current_node_index_);

    move_tree.AddChild(kMoveHH);
    ASSERT_TRUE(move_tree.MoveChildNode(kMoveHH));
    ASSERT_EQ(1, move_tree.current_node_index_);
    
    move_tree.MoveParent();
    ASSERT_EQ(kRootNodeIndex, move_tree.current_node_index_);
  }

  void IsRootNodeTest()
  {
    MoveTree move_tree;
    ASSERT_TRUE(move_tree.IsRootNode());

    move_tree.AddChild(kMoveHH);
    ASSERT_TRUE(move_tree.MoveChildNode(kMoveHH));

    ASSERT_FALSE(move_tree.IsRootNode());
  }

  void AddSubtreeTest()
  {
    MoveTree move_tree, sub_tree;

    move_tree.AddChild(kMoveHH);
    move_tree.MoveChildNode(kMoveHH);

    sub_tree.AddChild(kMoveHG);
    sub_tree.AddChild(kMoveIG);
    sub_tree.MoveChildNode(kMoveIG);
    sub_tree.AddChild(kMoveII);
    
    move_tree.AddSubtree(sub_tree);

    ASSERT_EQ(4, move_tree.size());

    {
      const auto& node = move_tree.tree_[0];
      ASSERT_EQ(1, node.GetFirstChildIndex());
      ASSERT_EQ(kNullNodeIndex, node.GetNextSiblingIndex());
    }
    {
      const auto& node = move_tree.tree_[1];
      ASSERT_EQ(2, node.GetFirstChildIndex());
      ASSERT_EQ(kNullNodeIndex, node.GetNextSiblingIndex());
      ASSERT_EQ(kMoveHH, node.GetMove());
    }
    {
      const auto& node = move_tree.tree_[2];
      ASSERT_EQ(kNullNodeIndex, node.GetFirstChildIndex());
      ASSERT_EQ(3, node.GetNextSiblingIndex());
      ASSERT_EQ(kMoveHG, node.GetMove());
    }
    {
      const auto& node = move_tree.tree_[3];
      ASSERT_EQ(4, node.GetFirstChildIndex());
      ASSERT_EQ(kNullNodeIndex, node.GetNextSiblingIndex());
      ASSERT_EQ(kMoveIG, node.GetMove());
    }
    {
      const auto& node = move_tree.tree_[4];
      ASSERT_EQ(kNullNodeIndex, node.GetFirstChildIndex());
      ASSERT_EQ(kNullNodeIndex, node.GetNextSiblingIndex());
      ASSERT_EQ(kMoveII, node.GetMove());
    }

    ASSERT_EQ("hh(hg)(igii)", move_tree.str());
  }

  void MoveRootNodeTest(){
    MoveTree move_tree;
    
    ASSERT_TRUE(move_tree.IsRootNode());

    move_tree.AddChild(kMoveHH);
    move_tree.MoveChildNode(kMoveHH);

    ASSERT_FALSE(move_tree.IsRootNode());

    move_tree.MoveRootNode();
    ASSERT_TRUE(move_tree.IsRootNode());
  }
};

TEST_F(MoveTreeBaseTest, DefaultConstructorTest)
{
  DefaultConstructorTest();
}

TEST_F(MoveTreeBaseTest, AddChildTest)
{
  AddChildTest();
}

TEST_F(MoveTreeBaseTest, AddChildListTest)
{
  AddChildListTest();
}

TEST_F(MoveTreeBaseTest, GetChildNodeIndexTest)
{
  GetChildNodeIndexTest();
}

TEST_F(MoveTreeBaseTest, GetYoungestChildNodeIndexTest)
{
  GetYoungestChildNodeIndexTest();
}

TEST_F(MoveTreeBaseTest, MoveChildNodeTest)
{
  MoveChildNodeTest();
}

TEST_F(MoveTreeBaseTest, MoveChildNodeMoveListTest)
{
  MoveChildNodeMoveListTest();
}

TEST_F(MoveTreeBaseTest, MoveParentTest)
{
  MoveParentTest();
}

TEST_F(MoveTreeBaseTest, IsRootNodeTest)
{
  IsRootNodeTest();
}

TEST_F(MoveTreeBaseTest, GetChildMoveListTest){
  MoveTree move_tree;

  {
    MoveList move_list;
    move_tree.GetChildMoveList(&move_list);

    ASSERT_TRUE(move_list.empty());
  }

  move_tree.AddChild(kMoveHH);

  {
    MoveList move_list;
    move_tree.GetChildMoveList(&move_list);

    ASSERT_EQ(1, move_list.size());
    ASSERT_EQ(kMoveHH, move_list[0]);
  }

  move_tree.AddChild(kMoveHI);

  {
    MoveList move_list;
    move_tree.GetChildMoveList(&move_list);

    ASSERT_EQ(2, move_list.size());
    ASSERT_EQ(kMoveHH, move_list[0]);
    ASSERT_EQ(kMoveHI, move_list[1]);
  }
}

TEST_F(MoveTreeBaseTest, sizeTest){
  MoveTree move_tree;
  ASSERT_EQ(0, move_tree.size());

  move_tree.AddChild(kMoveHH);
  ASSERT_EQ(1, move_tree.size());
}

TEST_F(MoveTreeBaseTest, strTest){
  MoveTree move_tree;

  ASSERT_EQ("", move_tree.str());

  move_tree.AddChild(kMoveHH);
  ASSERT_EQ("hh", move_tree.str());

  move_tree.AddChild(kMoveHI);
  ASSERT_EQ("(hh)(hi)", move_tree.str());

  move_tree.MoveChildNode(kMoveHH);
  move_tree.AddChild(kMoveOO);
  ASSERT_EQ("(hhoo)(hi)", move_tree.str());
}

TEST_F(MoveTreeBaseTest, GetSGFLabeledTextTest){
  MoveTree move_tree;
  constexpr bool kBlackTurn = true;

  ASSERT_EQ("", move_tree.GetSGFLabeledText(kBlackTurn));

  move_tree.AddChild(kMoveHH);
  ASSERT_EQ(";LB[hh:1];B[hh]", move_tree.GetSGFLabeledText(kBlackTurn));

  move_tree.AddChild(kMoveHI);
  ASSERT_EQ("(;LB[hh:1];B[hh])(;LB[hi:1];B[hi])", move_tree.GetSGFLabeledText(kBlackTurn));

  move_tree.MoveChildNode(kMoveHH);
  move_tree.AddChild(kMoveOO);
  ASSERT_EQ("(;LB[hh:1];B[hh];LB[oo:2];W[oo])(;LB[hi:1];B[hi])", move_tree.GetSGFLabeledText(kBlackTurn));

  move_tree.MoveChildNode(kMoveOO);
  move_tree.AddChild(kNullMove);
  ASSERT_EQ("(;LB[hh:1];B[hh];LB[oo:2];W[oo];LB[tt:3];B[tt])(;LB[hi:1];B[hi])", move_tree.GetSGFLabeledText(kBlackTurn));
}

TEST_F(MoveTreeBaseTest, AddSubtreeTest){
  AddSubtreeTest();
}

TEST_F(MoveTreeBaseTest, GetTopNodeMove){
  MoveTree move_tree;

  ASSERT_EQ(kInvalidMove, move_tree.GetTopNodeMove());
  
  move_tree.AddChild(kMoveHH);
  move_tree.MoveChildNode(kMoveHH);
  move_tree.AddChild(kMoveOO);

  ASSERT_EQ(kMoveHH, move_tree.GetTopNodeMove());
}

TEST_F(MoveTreeBaseTest, emptyTest){
  MoveTree move_tree;
  ASSERT_TRUE(move_tree.empty());

  move_tree.AddChild(kMoveHH);
  ASSERT_FALSE(move_tree.empty());  
}

TEST_F(MoveTreeBaseTest, clearTest){
  MoveTree move_tree;
  ASSERT_TRUE(move_tree.empty());

  move_tree.AddChild(kMoveHH);
  ASSERT_FALSE(move_tree.empty());  

  move_tree.clear();
  ASSERT_TRUE(move_tree.empty());
}

TEST_F(MoveTreeBaseTest, IsConflictORNodeTest){
  MoveTree move_tree;
  
  move_tree.AddChild(kMoveHH);    // OR node on root
  move_tree.AddChild(kMoveHG);    // OR node on root
  move_tree.AddChild(kMoveHI);    // OR node on root
  
  move_tree.MoveChildNode(kMoveHH);

  move_tree.AddChild(kMoveHG);    // AND node
  move_tree.MoveChildNode(kMoveHG);

  move_tree.AddChild(kMoveHI);    // OR node

  ASSERT_FALSE(move_tree.IsConflictORNode(kMoveHH));
  ASSERT_FALSE(move_tree.IsConflictORNode(kMoveHG));
  ASSERT_TRUE(move_tree.IsConflictORNode(kMoveHI));
}

TEST_F(MoveTreeBaseTest, MoveRootNodeTest){
  MoveRootNodeTest();
}

TEST_F(MoveTreeBaseTest, depthTest){
  MoveTree move_tree;

  ASSERT_EQ(0, move_tree.depth());

  move_tree.AddChild(kMoveHH);    // OR node on root
  ASSERT_EQ(1, move_tree.depth());

  move_tree.AddChild(kMoveHG);    // OR node on root
  move_tree.AddChild(kMoveHI);    // OR node on root
  ASSERT_EQ(1, move_tree.depth());

  move_tree.MoveChildNode(kMoveHH);
  move_tree.AddChild(kMoveHG);    // AND node

  ASSERT_EQ(2, move_tree.depth());
}

TEST_F(MoveTreeBaseTest, GetMoveListTest){
  MoveTree move_tree;
  
  {
    MoveList move_list;
    move_tree.GetMoveList(&move_list);

    ASSERT_TRUE(move_list.empty());
  }

  move_tree.AddChild(kMoveHH);
  move_tree.MoveChildNode(kMoveHH);
  
  {
    MoveList move_list;
    move_tree.GetMoveList(&move_list);

    ASSERT_EQ(1, move_list.size());
    ASSERT_EQ(kMoveHH, move_list[0]);
  }

  move_tree.AddChild(kMoveHI);
  move_tree.MoveChildNode(kMoveHI);

  {
    MoveList move_list;
    move_tree.GetMoveList(&move_list);

    ASSERT_EQ(2, move_list.size());
    ASSERT_EQ(kMoveHH, move_list[0]);
    ASSERT_EQ(kMoveHI, move_list[1]);
  }
}

TEST_F(MoveTreeBaseTest, GetMoveTreeNodeListTest){
  MoveTree move_tree;

  move_tree.AddChild(kMoveHH);    // OR node on root
  move_tree.AddChild(kMoveHG);    // OR node on root
  move_tree.AddChild(kMoveHI);    // OR node on root

  move_tree.MoveChildNode(kMoveHH);
  move_tree.AddChild(kMoveHG);    // AND node
  
  const auto node_list = move_tree.GetMoveTreeNodeList();

  ASSERT_EQ(5, node_list.size());
  ASSERT_EQ(kMoveHH, node_list[1].GetMove());
  ASSERT_EQ(kMoveHG, node_list[2].GetMove());
  ASSERT_EQ(kMoveHI, node_list[3].GetMove());
  ASSERT_EQ(kMoveHG, node_list[4].GetMove());
}

TEST_F(MoveTreeBaseTest, GetLeafNodeListTest){
  MoveTree move_tree;

  move_tree.AddChild(kMoveHH);    // OR node on root
  move_tree.AddChild(kMoveHG);    // OR node on root
  move_tree.AddChild(kMoveHI);    // OR node on root

  move_tree.MoveChildNode(kMoveHH);
  move_tree.AddChild(kMoveHG);    // AND node
  
  std::vector<MoveNodeIndex> node_list;
  move_tree.GetLeafNodeList(&node_list);

  ASSERT_EQ(3, node_list.size());
  ASSERT_EQ(2, node_list[0]);
  ASSERT_EQ(3, node_list[1]);
  ASSERT_EQ(4, node_list[2]);
}

}   // namespace realcore
