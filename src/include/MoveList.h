//! @file
//! @brief 指し手リストクラスの定義
//! @author Koichi NABETANI
//! @date 2017/02/21
#ifndef MOVE_LIST_H
#define MOVE_LIST_H

#include <vector>

#include "Move.h"

namespace realcore{
  class MoveListTest;

  //! @brief 指し手リストの管理クラス
  class MoveList
  {
    friend class MoveListTest;

  public:
    MoveList();
    MoveList(const MoveList &move_list);
    MoveList(const MovePosition move);

    //! @brief 指し手リストの長さを返す
    //! @retval 指し手リストの長さ
    const size_t size() const;

  private:
    //! @brief 初期化時に確保する領域長さを算出する
    //! @param initial_size 初期化時の指し手リスト長
    //! @retval 確保する領域長
    //! @note 領域の再確保を抑制するため「初期化時のリスト長 + 16」(=L_0)より大きい領域を確保する
    //! @note MovePosition型は1バイトのため8バイト単位で確保するためL_0より大きい最小の8の倍数を返す
    const size_t CalcInitialReserveSize(const size_t initial_list_size) const;

    //! @brief 指し手リスト
    std::vector<MovePosition> move_list_;
  };

}   // namespace realcore

#endif    // MOVE_LIST_H