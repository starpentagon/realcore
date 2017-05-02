#ifndef MOVE_TREE_INL_H
#define MOVE_TREE_INL_H

#include <string>
#include <map>

#include "MoveTree.h"

namespace realcore
{

template<class T>
inline MoveTreeBase<T>::MoveTreeBase()
{
  clear();
}

template<class T>
void MoveTreeBase<T>::AddChild(const MovePosition move)
{
  assert(tree_.size() < kMaxMoveNodeIndex);
  const MoveNodeIndex child_node_index = static_cast<MoveNodeIndex>(tree_.size());

  tree_.emplace_back(current_node_index_, move);
  auto& current_node = tree_[current_node_index_];

  if(!current_node.HasChild()){
    current_node.SetFirstChildIndex(child_node_index);
  }else{
    const auto youngest_child_index = GetYoungestChildNodeIndex();
    auto& youngest_child_node = tree_[youngest_child_index];

    youngest_child_node.SetNextSiblingIndex(child_node_index);
  }
}

template<class T>
void MoveTreeBase<T>::AddSubtree(const MoveTreeBase<T> &move_tree)
{
  const MoveNodeIndex start_node_index = static_cast<MoveNodeIndex>(tree_.size() - 1);
  const auto& subtree_node_list = move_tree.GetMoveTreeNodeList();
  const auto subtree_node_count = subtree_node_list.size() - 1;   // root node以外

  if(subtree_node_count == 0){
    return;
  }

  assert(tree_.size() + subtree_node_count < kMaxMoveNodeIndex);
  tree_.reserve(tree_.size() + subtree_node_count);

  // subtreeを追加する
  std::vector<MoveNodeIndex> subtree_top_node;   // sub treeのroot node直下のノード一覧

  for(size_t i=1; i<=subtree_node_count; i++){
    const auto& subtree_node = subtree_node_list[i];
    tree_.emplace_back(subtree_node);

    auto& added_node = tree_.back();
    const size_t node_index = start_node_index + i;

    if(added_node.GetParentIndex() == kRootNodeIndex){
      subtree_top_node.emplace_back(node_index);
      added_node.SetNextSiblingIndex(kNullNodeIndex);
    }else{
      added_node.SetParentIndex(added_node.GetParentIndex() + start_node_index);

      const auto next_sibling_index = added_node.GetNextSiblingIndex();
      
      if(next_sibling_index != kNullNodeIndex){
        added_node.SetNextSiblingIndex(next_sibling_index + start_node_index);
      }
    }

    const auto first_child_index = added_node.GetFirstChildIndex();

    if(first_child_index != kNullNodeIndex){
      added_node.SetFirstChildIndex(first_child_index + start_node_index);
    }
  }

  // カレントノードとsub treeのroot node直下のノードの親子関係を更新する
  auto& current_node = tree_[current_node_index_];

  for(auto node_index : subtree_top_node){
    auto& subtree_top_node = tree_[node_index];

    subtree_top_node.SetParentIndex(current_node_index_);

    if(!current_node.HasChild()){
      current_node.SetFirstChildIndex(node_index);
    }else{
      const auto youngest_child_index = GetYoungestChildNodeIndex();
      auto& youngest_child_node = tree_[youngest_child_index];

      youngest_child_node.SetNextSiblingIndex(node_index);
    }
  }
}

template<class T>
const bool MoveTreeBase<T>::MoveChildNode(const MovePosition move)
{
  const auto child_node_index = GetChildNodeIndex(move);

  if(child_node_index == kNullNodeIndex){
    return false;
  }

  current_node_index_ = child_node_index;
  return true;
}

template<class T>
inline void MoveTreeBase<T>::MoveParent()
{
  if(IsRootNode()){
    return;
  }

  const auto& current_node = tree_[current_node_index_];
  current_node_index_ = current_node.GetParentIndex();
}

template<class T>
void MoveTreeBase<T>::GetChildMoveList(MoveList * const move_list) const
{
  assert(move_list != nullptr);
  assert(move_list->empty());

  MoveNodeIndex child_node_index = tree_[current_node_index_].GetFirstChildIndex();

  while(child_node_index != kNullNodeIndex){
    const auto& child_node = tree_[child_node_index];
    const auto child_move = child_node.GetMove();

    (*move_list) += child_move;

    child_node_index = child_node.GetNextSiblingIndex();
  }
}

template<class T>
inline const size_t MoveTreeBase<T>::size() const
{
  return tree_.size() - 1;
}

template<class T>
inline const bool MoveTreeBase<T>::IsRootNode() const
{
  return (current_node_index_ == kRootNodeIndex);
}

template<class T>
const MoveNodeIndex MoveTreeBase<T>::GetChildNodeIndex(const MovePosition move) const
{
  MoveNodeIndex child_node_index = tree_[current_node_index_].GetFirstChildIndex();

  while(child_node_index != kNullNodeIndex){
    const auto& child_node = tree_[child_node_index];
    const auto child_move = child_node.GetMove();

    if(move == child_move){
      return child_node_index;
    }

    child_node_index = child_node.GetNextSiblingIndex();
  }

  return kNullNodeIndex;
}

template<class T>
const MoveNodeIndex MoveTreeBase<T>::GetYoungestChildNodeIndex() const
{
  MoveNodeIndex child_node_index = tree_[current_node_index_].GetFirstChildIndex();
  assert(child_node_index != kNullNodeIndex);

  while(child_node_index != kNullNodeIndex){
    const auto& child_node = tree_[child_node_index];
    const auto next_sibling_index = child_node.GetNextSiblingIndex();

    if(next_sibling_index == kNullNodeIndex){
      return child_node_index;
    }

    child_node_index = next_sibling_index;
  }
  
  return kNullNodeIndex;
}

template<class T>
std::string MoveTreeBase<T>::str() const
{
  return str(kRootNodeIndex);
}

template<class T>
std::string MoveTreeBase<T>::str(MoveNodeIndex move_node_index) const
{
  std::string subtree_str = "";
  MoveNodeIndex child_node_index = tree_[move_node_index].GetFirstChildIndex();
  
  if(child_node_index == kNullNodeIndex){
    // leaf node
    return subtree_str;
  }

  const auto &child_node = tree_[child_node_index];
  const bool is_multiple_brothers = child_node.GetNextSiblingIndex() != kNullNodeIndex;

  if(is_multiple_brothers){
    while(child_node_index != kNullNodeIndex){
      auto &child_node = tree_[child_node_index];

      subtree_str += "(";
      subtree_str += MoveString(child_node.GetMove());
      subtree_str += str(child_node_index);
      subtree_str += ")";
      
      child_node_index = child_node.GetNextSiblingIndex();
    }
  }else{
    subtree_str = MoveString(child_node.GetMove());
    subtree_str += str(child_node_index);
  }

  return subtree_str;
}

template<class T>
inline const std::vector< MoveTreeNode<T> >& MoveTreeBase<T>::GetMoveTreeNodeList() const
{
  return tree_;
}

template<class T>
inline const MovePosition MoveTreeBase<T>::GetTopNodeMove() const
{
  const auto& root_node = tree_[kRootNodeIndex];

  if(!root_node.HasChild())
    return kInvalidMove;
  
  const auto& top_node = tree_[root_node.GetFirstChildIndex()];
  return top_node.GetMove();
}

template<class T>
inline const bool MoveTreeBase<T>::empty() const
{
  return size() == 0;
}

template<class T>
inline const void MoveTreeBase<T>::clear()
{
  tree_.clear();
  
  // root nodeを設定
  tree_.emplace_back(kNullNodeIndex, kInvalidMove);
  current_node_index_ = kRootNodeIndex;
}

template<class T>
const bool MoveTreeBase<T>::IsConflictORNode(const MovePosition move) const
{
  constexpr bool kNodeOR = true;

  std::map<MoveNodeIndex, bool> node_and_or;    // 各ノードがAND/OR nodeかを記録する

  for(size_t node_index=1, size=tree_.size(); node_index<size; node_index++){
    const auto &node = tree_[node_index];
    const auto parent_node_index = node.GetParentIndex();
    bool is_node_or = false;

    if(parent_node_index == kRootNodeIndex){
      // Root node直下はOR node
      is_node_or = kNodeOR;
    }else{
      // 親のAND/ORを反転させたのが該当nodeのAND/OR
      const bool parent_node_and_or = node_and_or.at(parent_node_index);
      is_node_or = !parent_node_and_or;
    }

    node_and_or[node_index] = is_node_or;

    if(is_node_or && node.GetMove() == move){
      // OR nodeで兄弟ノードがないノードで指し手が競合する場合は証明木がOR nodeで競合する
      const auto& parent_node = tree_[parent_node_index];
      const auto& first_child_node_index = parent_node.GetFirstChildIndex();
      const auto& first_child_node = tree_[first_child_node_index];

      if(first_child_node.GetNextSiblingIndex() == kNullNodeIndex){
        return true;
      }
    }
  }

  return false;
}

}   // namespace realcore

#endif    // MOVE_TREE_INL_H