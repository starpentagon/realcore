//! @file
//! @brief 探索木の定義
//! @author Koichi NABETANI
//! @date 2017/04/16

#ifndef MOVE_TREE_H
#define MOVE_TREE_H

namespace realcore
{

//! @brief 探索木
template<class T>
class MoveTreeBase
{
public:

private:
  MoveTreeData tree_;   //!< 木構造
  T aditional_data_;    //!< 付加情報
};

typedef struct structEmptyAditionalData
{
}EmptyAditionalData;

typedef MoveTreeBase<EmptyAditionalData> MoveTree;
typedef MoveTreeBase<std::string> MoveTextTree;


}   // realcore





#endif    // MOVE_TREE_H