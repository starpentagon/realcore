//! @file
//! @brief 盤面の管理クラス
//! @author Koichi NABETANI
//! @date 2017/02/22

#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <array>
#include <vector>

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
//! @param call_limit 再帰呼出し回数の上限
const bool MakeNonTerminateNormalSequence(const MoveBitSet &black_remain, const MoveBitSet &white_remain, MoveList * const modified_move_list, unsigned int * const call_limit);

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
  Board(const MoveList &move_list, const UpdateOpenStateFlag &update_flag);

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
  const bool IsNormalMove(const MovePosition move) const;

  //! @brief 指し手が終端手かどうかをチェックする
  //! @param move 指し手位置
  //! @retval true 指し手が終端手
  //! @note 終端手 = 達四ができる or (白番のみ: 四々 or 四ノビで極めること)
  //! @pre moveは着手前であること
  //! @pre moveは正規手であること
  //! @note 禁手チェックは行わない
  //! @see doc/02_terminate_definition/normality.pptx
  const bool IsTerminateMove(const MovePosition move) const;

  //! @brief 指し手が終端手かどうかをチェックする(四ノビ、四ノビ防手ペア版)
  const bool IsTerminateMove(const MovePair &four_pair) const;

  //! @brief 終端手が存在するかチェックする
  //! @param terminating_move 終端手の格納先
  //! @retval true 終端手が存在する
  //! @note 禁手チェックを行う
  const bool TerminateCheck(MovePosition * const terminating_move) const;

  //! @brief 相手に四が生じているか判定する
  //! @param 四の防手の格納先
  //! @retval true 相手に四が生じている
  const bool IsOpponentFour(MovePosition * const guard_move) const;

  //! @brief 相手に1手勝ちが生じているかチェックし、その防手を生成する
  //! @param P 防手を生成する側の手番
  //! @note 相手に四ノビが生じていないこと
  const bool GetTerminateGuard(MoveBitSet * const guard_move_set) const;

  //! @brief 禁点を列挙する
  //! @param 禁点の格納先
  void EnumerateForbiddenMoves(MoveBitSet * const forbidden_move_set) const;

  //! @brief 達四点を列挙する
  template<PlayerTurn P>
  void EnumerateOpenFourMoves(MoveBitSet * const open_four_move_set) const;
  void EnumerateOpenFourMoves(const bool is_black_turn, MoveBitSet * const open_four_move_set) const;

  //! @brief 四ノビ点を列挙する
  template<PlayerTurn P>
  void EnumerateFourMoves(MoveBitSet * const four_move_set) const;
  void EnumerateFourMoves(const bool is_black_turn, MoveBitSet * const four_move_set) const;

  template<PlayerTurn P>
  void EnumerateFourMoves(std::vector<MovePair> * const four_move_list) const;
  void EnumerateFourMoves(const bool is_black_turn, std::vector<MovePair> * const four_move_list) const;

  //! @brief 見かけの三ノビ点を列挙する
  template<PlayerTurn P>
  void EnumerateSemiThreeMoves(MoveBitSet * const semi_three_move_set) const;
  void EnumerateSemiThreeMoves(const bool is_black_turn, MoveBitSet * const semi_three_move_set) const;

  //! @brief 剣先点を列挙する
  template<PlayerTurn P>
  void EnumeratePointOfSwordMoves(MoveBitSet * const point_of_sword_move_set) const;
  void EnumeratePointOfSwordMoves(const bool is_black_turn, MoveBitSet * const point_of_sword_move_set) const;

  //! @brief ミセ手(Passすると四三)を列挙する
  template<PlayerTurn P>
  void EnumerateMiseMoves(MoveBitSet * const mise_move_set, MoveBitSet * const multi_mise_move_set) const;
  void EnumerateMiseMoves(const bool is_black_turn, MoveBitSet * const mise_move_set, MoveBitSet * const multi_mise_move_set) const;

  //! @brief 見かけの三々点を列挙する
  template<PlayerTurn P>
  void EnumerateDoubleSemiThreeMoves(MoveBitSet * const double_semi_three_move_set) const;
  void EnumerateDoubleSemiThreeMoves(const bool is_black_turn, MoveBitSet * const double_semi_three_move_set) const;

  //! @brief 二ノビ点を列挙する
  template<PlayerTurn P>
  void EnumerateTwoMoves(MoveBitSet * const two_move_set) const;
  void EnumerateTwoMoves(const bool is_black_turn, MoveBitSet * const two_move_set) const;

protected:
  //! @brief 終端手が存在するかチェックする
  //! @param terminating_move 終端手の格納先
  //! @retval true 終端手が存在する
  //! @note 禁手チェックを行う
  const bool TerminateCheckBlack(MovePosition * const terminating_move) const;
  const bool TerminateCheckWhite(MovePosition * const terminating_move) const;

  //! @brief 盤面状態を保持するBitBoard
  BitBoard bit_board_;

  //! @brief 盤面の指し手リスト
  MoveList board_move_sequence_;

  //! @brief 盤面空点状態リスト
  std::vector<BoardOpenState> board_open_state_list_;
};

}   // namespace realcore

#include "Board-inl.h"

#endif    // BOARD_H
