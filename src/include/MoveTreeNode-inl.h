#ifndef MOVE_TREE_NODE_INL_H
#define MOVE_TREE_NODE_INL_H

#include "MoveTreeNode.h"

namespace realcore
{

template<class T>
inline MoveTreeNode<T>::MoveTreeNode(const MoveNodeIndex parent_index, const MovePosition move)
: parent_index_(parent_index), next_sibling_index_(kNullNodeIndex), first_child_index_(kNullNodeIndex), move_(move)
{
}

template<class T>
inline const MoveTreeNode<T>& MoveTreeNode<T>::operator=(const MoveTreeNode<T> &rhs)
{
  if(this != &rhs){
    parent_index_ = rhs.GetParentIndex();
    next_sibling_index_ = rhs.GetNextSiblingIndex();
    first_child_index_ = rhs.GetFirstChildIndex();
    move_ = rhs.GetMove();
    additional_data_ = rhs.GetAdditionalData();
  }

  return *this;
}

template<class T>
inline const bool MoveTreeNode<T>::operator==(const MoveTreeNode<T> &rhs) const
{
  if(parent_index_ != rhs.GetParentIndex()){
    return false;
  }

  if(next_sibling_index_ != rhs.GetNextSiblingIndex()){
    return false;
  }

  if(first_child_index_ != rhs.GetFirstChildIndex()){
    return false;
  }

  if(move_ != rhs.GetMove()){
    return false;
  }

  if(additional_data_ != rhs.GetAdditionalData()){
    return false;
  }

  return true;
}

template<class T>
inline const bool MoveTreeNode<T>::operator!=(const MoveTreeNode<T> &rhs) const
{
  return !(*this == rhs);
}

template<class T>
inline MoveNodeIndex MoveTreeNode<T>::GetParentIndex() const
{
  return parent_index_;
}

template<class T>
inline void MoveTreeNode<T>::SetParentIndex(const MoveNodeIndex parent_index)
{
  parent_index_ = parent_index;
}

template<class T>
inline MoveNodeIndex MoveTreeNode<T>::GetNextSiblingIndex() const
{
  return next_sibling_index_;
}

template<class T>
inline void MoveTreeNode<T>::SetNextSiblingIndex(const MoveNodeIndex next_sibling_index)
{
  next_sibling_index_ = next_sibling_index;
}

template<class T>
inline MoveNodeIndex MoveTreeNode<T>::GetFirstChildIndex() const
{
  return first_child_index_;
}

template<class T>
inline void MoveTreeNode<T>::SetFirstChildIndex(const MoveNodeIndex first_child_index)
{
  first_child_index_ = first_child_index;
}

template<class T>
inline const bool MoveTreeNode<T>::HasChild() const
{
  return first_child_index_ != kNullNodeIndex;
}

template<class T>
inline MovePosition MoveTreeNode<T>::GetMove() const
{
  return move_;
}

template<class T>
inline const T& MoveTreeNode<T>::GetAdditionalData() const
{
  return additional_data_;
}

template<class T>
inline void MoveTreeNode<T>::SetAdditionalData(const T& additional_data)
{
  additional_data_ = additional_data;
}

}   // realcore

#endif    // MOVE_TREE_NODE_INL_H
