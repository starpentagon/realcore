//! @file
//! @brief 盤面空点状態の管理クラス
//! @author Koichi NABETANI
//! @date 2017/04/05

#ifndef BOARD_OPEN_STATE_H
#define BOARD_OPEN_STATE_H

#include <array>
#include <vector>
#include "OpenState.h"

namespace realcore
{
//! 前方宣言
class BoardOpenState;

class LineNeighborhood;
class BitBoard;

typedef std::vector<OpenState> OpenStateList;

//! @brief 2つのBoardOpenStateを比較する
//! @param board_1, 2: 比較対象
//! @retval true 2つのBoardが同一の内容を保持
bool IsEqual(const BoardOpenState &lhs, const BoardOpenState &rhs);

//! @brief コピーを作成する
//! @param from コピー元
//! @param to コピー先
void Copy(const BoardOpenState &from, BoardOpenState * const to);

//! @brief 空点状態の差分計算クラス
class BoardOpenState
{
  friend class BoardOpenStateTest;
  friend void Copy(const BoardOpenState &board_from, BoardOpenState * const board_to);

public:
  BoardOpenState();
  BoardOpenState(const BoardOpenState &board_open_state);

  //! @brief 着手による空点状態の変更を反映する
  //! @param is_black_turn 手番
  //! @param move 着手
  //! @param bit_board BitBoard
  //! @pre moveは着手後であること
  BoardOpenState(const BoardOpenState &board_open_state, const bool is_black_turn, const MovePosition move, const BitBoard &bit_board);

  //! @brief 代入演算子
  const BoardOpenState& operator=(const BoardOpenState &rhs);

  //! @brief 比較演算子
  const bool operator==(const BoardOpenState &rhs) const;
  const bool operator!=(const BoardOpenState &rhs) const;

  //! @brief 空点状態のリストを返す
  //! @param pattern 指し手パターン(長連点, 達四点, etc)
  const std::vector<OpenState>& GetList(const OpenStatePattern pattern) const;
  
  //! @brief 空点状態のリストサイズを調整する
  template<OpenStatePattern Pattern>
  void ReserveList(const size_t list_size);

  //! @brief 空点状態のリストを追加する
  template<OpenStatePattern Pattern>
  void AddOpenState(const size_t pattern_search_index, const BoardPosition pattern_position);

  //! @brief すべての空点状態が空かどうかを判定する
  const bool empty() const;
  
private:
  //! @brief 着手の影響を受けるOpenState要素を削除したリストを生成する
  //! @param P moveの手番
  //! @param open_state_list move着手前のOpenStateのリスト
  //! @param move 着手
  //! @param cleared_open_state_list move着手による影響分を除外したOpenStateのリスト
  template<PlayerTurn P>
  void ClearInfluencedOpenState(const std::vector<OpenState> &open_state_list, const MovePosition move, std::vector<OpenState> * const cleared_open_state_list) const;
  void ClearInfluencedOpenState(const bool is_black_turn, const std::vector<OpenState> &open_state_list, const MovePosition move, std::vector<OpenState> * const cleared_open_state_list) const;

  std::array<OpenStateList, kOpenStatePatternNum> open_state_list_;    //! 指し手パターン(長連点, etc)ごとの空点状態リスト
};
}   // namespace realcore 

#include "BoardOpenState-inl.h"

#endif    // BOARD_OPEN_STATE_H