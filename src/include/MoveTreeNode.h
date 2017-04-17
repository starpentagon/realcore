//! @file
//! @brief 探索木データの定義
//! @author Koichi NABETANI
//! @date 2017/04/16

#ifndef MOVE_TREE_NODE_H
#define MOVE_TREE_NODE_H

#include <cstdint>
#include <limits>
#include <string>

#include "Move.h"

namespace realcore
{

typedef std::uint32_t MoveNodeIndex;    //!< MoveTreeクラスのノード配列のindex

constexpr MoveNodeIndex kMaxMoveNodeIndex = std::numeric_limits<MoveNodeIndex>::max();
constexpr MoveNodeIndex kNullNodeIndex = std::numeric_limits<MoveNodeIndex>::max();
constexpr MoveNodeIndex kRootNodeIndex = 0;   //!< root nodeのMoveNodeIndex

// 前方宣言
class MoveTreeNodeTest;

//! @brief 木構造を格納するデータ
template<class T>
class MoveTreeNode
{
friend class MoveTreeNodeTest;

public:
  MoveTreeNode(const MoveNodeIndex parent_index, const MovePosition move);

  //! @brief 代入演算子
  const MoveTreeNode<T>& operator=(const MoveTreeNode<T> &rhs);

  //! @brief 比較演算子
  const bool operator==(const MoveTreeNode<T> &rhs) const;
  const bool operator!=(const MoveTreeNode<T> &rhs) const;

  //! @brief 親のindexを返す
  MoveNodeIndex GetParentIndex() const;

  //! @brief 次の兄弟のindexを返す
  MoveNodeIndex GetNextSiblingIndex() const;

  //! @brief 次の兄弟のindexを設定する
  void SetNextSiblingIndex(const MoveNodeIndex next_sibling_index);

  //! @brief 最初の子のindexを返す
  MoveNodeIndex GetFirstChildIndex() const;

  //! @brief 最初の子のindexを設定する
  void SetFirstChildIndex(const MoveNodeIndex first_child_index);

  //! @brief 親ノードからの指し手を取得する
  MovePosition GetMove() const;

  //! @brief 付加情報を取得する
  const T& GetAdditionalData() const;

  //! @brief 付加情報を設定する
  void SetAdditionalData(const T& additional_data);

private:
  MoveNodeIndex parent_index_;         //!< 親のindex
  MoveNodeIndex next_sibling_index_;   //!< 次の兄弟のindex
  MoveNodeIndex first_child_index_;    //!< 最初の子のindex
  MovePosition move_;                  //!< 親から当該ノードへの指し手
  T additional_data_;                  //!< 付加情報
};

}   // namespace realcore

#include "MoveTreeNode-inl.h"

#endif    // MOVE_TREE_NODE_H