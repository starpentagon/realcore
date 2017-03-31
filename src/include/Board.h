//! @file
//! @brief 盤面の管理クラス
//! @author Koichi NABETANI
//! @date 2017/02/22

#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <array>

#include "RealCore.h"
#include "BitBoard.h"
#include "MoveList.h"

namespace realcore
{
// 前方宣言
enum MovePosition : std::uint8_t;
class BoardTest;
class Board;

//! @brief 2つのBoardを比較する
//! @param board_1, 2: 比較対象
//! @retval true 2つのBoardが同一の内容を保持
bool IsEqual(const Board &board_1, const Board &board_2);

//! @brief コピーを作成する
//! @param board_from コピー元
//! @param board_to コピー先
void Copy(const Board &board_from, Board * const board_to);

//! @brief 盤面管理クラス
class Board
{
  friend class BoardTest;
  friend bool IsEqual(const Board &board_1, const Board &board_2);
  friend void Copy(const Board &board_from, Board * const board_to);

public:
  Board();
  Board(const Board &board);
  Board(const MoveList &move_list);

  //! @brief 代入演算子
  const Board& operator=(const Board &board);

  //! @brief 比較演算子
  const bool operator==(const Board &board) const;
  const bool operator!=(const Board &board) const;

  //! @brief 指し手を設定する
  //! @param move 指し手位置
  void MakeMove(const MovePosition move);

  //! @brief 指し手を１手戻す
  void UndoMove();

  //! @brief 指し手が正規手かどうかをチェックする
  //! @param move 指し手位置
  //! @retval true 指し手が正規手
  //! @pre moveは着手前であること
  //! @see doc/02_terminate_definition/normality.pptx
  template<PlayerTurn P>
  const bool IsNormalMove(const MovePosition move) const;

  //! @brief 指し手が終端手かどうかをチェックする
  //! @param move 指し手位置
  //! @retval true 指し手が終端手
  //! @pre moveは着手前であること
  //! @see doc/02_terminate_definition/normality.pptx
  template<PlayerTurn P>
  const bool IsTerminateMove(const MovePosition move) const;

protected:
  //! @brief 盤面状態を保持する配列
  Bitboard bit_board_;

  //! @brief 盤面の指し手リスト
  MoveList move_list_;
};

}   // namespace realcore

#include "Board-inl.h"

#endif    // BOARD_H
