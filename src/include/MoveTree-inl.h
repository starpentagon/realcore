#ifndef MOVE_TREE_INL_H
#define MOVE_TREE_INL_H

#include "MoveTree.h"

namespace realcore
{

template<class T>
MoveTreeBase<T>::MoveTreeBase()
{
  // root nodeを設定
  tree_.emplace_back(kNullNodeIndex, kInvalidMove);
  current_node_index_ = kRootNodeIndex;
}


}   // namespace realcore

#endif    // MOVE_TREE_INL_H