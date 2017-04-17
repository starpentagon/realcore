//! @file
//! @brief 探索木の定義
//! @author Koichi NABETANI
//! @date 2017/04/16

#ifndef MOVE_TREE_H
#define MOVE_TREE_H

#include <vector>

#include "MoveList.h"
#include "MoveTreeNode.h"

namespace realcore
{

// 前方宣言
class MoveTreeBaseTest;

//! @brief 探索木
template<class T>
class MoveTreeBase
{
  friend class MoveTreeBaseTest;

public:
  MoveTreeBase();

  //! @brief カレントノードから子ノードへ移動する
  //! @param move 子ノードへの指し手
  //! @retval true: 移動成功, false: 指定の指し手の子ノードが存在せず移動失敗
  const bool MoveChildNode(const MovePosition move);

  //! @brief カレントノードから親ノードへ移動する
  void MoveParent();

  //! @brief 子ノードを追加する
  //! @param move 子ノードへの指し手
  void AddChild(const MovePosition move);

  //! @brief すべての子ノードの指し手を取得する
  void GetChildMoveList(MoveList * const move_list) const;

  //! @brief 探索木のサイズ（root node以外のノード数）を返す
  const size_t size() const;

private:
  //! @brief 指定の指し手の子ノードのnode indexを取得する
  //! @note 指定の指し手の子ノードが存在しなかった場合はkNullNodeIndexを返す
  const MoveNodeIndex GetChildNodeIndex(const MovePosition move) const;

  std::vector< MoveTreeNode<T> > tree_;   //!< 木構造
  MoveNodeIndex current_node_index_;      // カレントノードのnode index
};

typedef struct structEmptyAditionalData
{
}EmptyAditionalData;

typedef MoveTreeBase<EmptyAditionalData> MoveTree;
typedef MoveTreeBase<std::string> MoveTextTree;

}   // realcore

#include "MoveTree-inl.h"

#endif    // MOVE_TREE_H