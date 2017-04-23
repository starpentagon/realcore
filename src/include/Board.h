//! @file
//! @brief 盤面の管理クラス
//! @author Koichi NABETANI
//! @date 2017/02/22

#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <array>
#include <vector>
#include <set>

#include "RealCore.h"
#include "BitBoard.h"
#include "MoveList.h"
#include "BoardOpenState.h"

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

//! @brief 指し手リストが終端ではない正規手順かどうかを判定する
const bool IsNonTerminateNormalSequence(const MoveList &move_list);

//! @brief 指し手リストを終端ではない正規手順に修正する
//! @pre 黒白同数 or 黒石数 = 白石数 + 1であること
const bool MakeNonTerminateNormalSequence(const MoveList &original_move_list, MoveList * const modified_move_list);

//! @param black_remain 未決定の黒石bit
//! @param white_remain 未決定の白石bit
//! @param on_overline_black 長連筋の両端の黒石bit
//! @param on_overline_black 長連筋内の白石bit
//! @param transposition_set 置換表
//! @param modified_move_list 決定済の指し手リスト
const bool MakeNonTerminateNormalSequence(const MoveBitSet &black_remain, const MoveBitSet &white_remain, const MoveBitSet &on_overline_black, const MoveBitSet &on_overline_white, std::set<std::string> * const transposition_set, MoveList * const modified_move_list);

//! @brief 盤面管理クラス
class Board
{
  friend class BoardTest;
  friend bool IsEqual(const Board &board_1, const Board &board_2);
  friend void Copy(const Board &board_from, Board * const board_to);

public:
  Board();
  Board(const UpdateOpenStateFlag &update_flag);
  Board(const Board &board);
  Board(const MoveList &move_list);

  //! @brief 代入演算子
  const Board& operator=(const Board &board);

  //! @brief 比較演算子
  const bool operator==(const Board &board) const;
  const bool operator!=(const Board &board) const;

  //! @brief 指し手を設定する
  //! @param move 指し手位置
  //! @pre 指し手は正規手かつ終端手ではないこと
  void MakeMove(const MovePosition move);

  //! @brief 指し手を１手戻す
  void UndoMove();

  //! @brief 指し手が正規手かどうかをチェックする
  //! @param move 指し手位置
  //! @retval true 指し手が正規手
  //! @note 正規手 = (空点 or Pass) AND (相手の直前手が四であればその防手) AND (相手の直前手が終端手なら投了)
  //! @note 相手の直前手が終端手かどうかは相手の手番時にチェックするため正規手チェックではチェックしない
  //! @pre moveは着手前であること
  //! @see doc/02_terminate_definition/normality.pptx
  template<PlayerTurn P>
  const bool IsNormalMove(const MovePosition move) const;
  
  const bool IsNormalMove(const bool black_turn, const MovePosition move) const;

  //! @brief 指し手が終端手かどうかをチェックする
  //! @param move 指し手位置
  //! @retval true 指し手が終端手
  //! @note 終端手 = 達四ができる or (白番のみ: 四々 or 四ノビで極めること)
  //! @pre moveは着手前であること
  //! @pre moveは正規手であること
  //! @note 禁手チェックは行わない
  //! @see doc/02_terminate_definition/normality.pptx
  template<PlayerTurn P>
  const bool IsTerminateMove(const MovePosition move) const;
  const bool IsTerminateMove(const bool black_turn, const MovePosition move) const;

  //! @brief 指し手が終端手かどうかをチェックする(四ノビ、四ノビ防手ペア版)
  template<PlayerTurn P>
  const bool IsTerminateMove(const MovePair &four_pair) const;

  //! @brief 終端手が存在するかチェックする
  //! @param terminating_move 終端手の格納先
  //! @retval true 終端手が存在する
  //! @note 禁手チェックを行う
  template<PlayerTurn P>
  const bool TerminateCheck(MovePosition * const terminating_move) const;

  //! @brief 相手に四が生じているか判定する
  //! @param 四の防手の格納先
  //! @retval true 相手に四が生じている
  const bool IsOpponentFour(MovePosition * const guard_move) const;

  //! @brief 禁点を列挙する
  //! @param 禁点の格納先
  void EnumerateForbiddenMoves(MoveBitSet * const forbidden_move_set) const;

  //! @brief 四ノビ点を列挙する
  template<PlayerTurn P>
  void EnumerateFourMoves(MoveBitSet * const four_move_set) const;
  void EnumerateFourMoves(const bool is_black_turn, MoveBitSet * const four_move_set) const;

  template<PlayerTurn P>
  void EnumerateFourMoves(std::vector<MovePair> * const four_move_list) const;
  void EnumerateFourMoves(const bool is_black_turn, std::vector<MovePair> * const four_move_list) const;

protected:
  //! @brief 盤面状態を保持するBitBoard
  BitBoard bit_board_;

  //! @brief 盤面の指し手リスト
  MoveList move_list_;

  //! @brief 盤面空点状態リスト
  std::vector<BoardOpenState> board_open_state_list_;
};

}   // namespace realcore

#include "Board-inl.h"

#endif    // BOARD_H
