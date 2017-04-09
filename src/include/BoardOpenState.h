//! @file
//! @brief 盤面空点状態の管理クラス
//! @author Koichi NABETANI
//! @date 2017/04/05

#ifndef BOARD_OPEN_STATE_H
#define BOARD_OPEN_STATE_H

#include <vector>
#include "OpenState.h"

namespace realcore
{

//! 前方宣言
class OpenState;
class BoardOpenState;

class LineNeighborhood;
class BitBoard;

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
  friend void Copy(const BoardOpenState &board_from, BoardOpenState * const board_to);

public:
  BoardOpenState();
  BoardOpenState(const BoardOpenState &board_open_state);

  //! @brief 代入演算子
  const BoardOpenState& operator=(const BoardOpenState &rhs);

  //! @brief 比較演算子
  const bool operator==(const BoardOpenState &rhs) const;
  const bool operator!=(const BoardOpenState &rhs) const;

  //! @brief 着手による空点状態の変更を反映する
  //! @param P 手番
  //! @param move 着手
  //! @param bit_board BitBoard
  //! @pre moveは着手後であること
  template<PlayerTurn P>
  void Update(const MovePosition move, const BitBoard &bit_board);
  void Update(const bool black_turn, const MovePosition move, const BitBoard &bit_board);

  //! @brief 空点状態のリストを返す
  //! @param pattern 指し手パターン(長連点, 達四点, etc)
  const std::vector<OpenState>& GetList(const OpenStatePattern pattern) const;

  //! @brief 長連点のリストを追加する
  void AddNextOverline(const BoardPosition open_position, const BoardPosition pattern_position);

  //! @brief 達四点を追加する
  template<PlayerTurn P>
  void AddNextOpenFour(const BoardPosition open_position, const BoardPosition pattern_position);

  //! @brief 四ノビ点を追加する
  template<PlayerTurn P>
  void AddNextFour(const BoardPosition open_position, const BoardPosition pattern_position, const BoardPosition guard_position);

  //! @brief 見かけの三ノビ点を追加する
  template<PlayerTurn P>
  void AddNextSemiThree(const BoardPosition open_position, const BoardPosition pattern_position, const BoardPosition check_position, const GuardPositionList &guard_position_list);

  //! @brief すべての空点状態が空かどうかを判定する
  const bool empty() const;
  
private:
  //! @brief 着手の影響を受けるOpenState要素を削除したリストを生成する
  //! @param P moveの手番
  //! @param open_state_list move着手前のOpenStateのリスト
  //! @param move 着手
  //! @param cleared_open_state_list move着手による影響分を除外したOpenStateのリスト
  template<PlayerTurn P>
  void ClearInfluencedElement(const std::vector<OpenState> &open_state_list, const MovePosition move, std::vector<OpenState> * const cleared_open_state_list) const;

  //! @brief 着手によるOpenState要素を追加する
  //! @param added_open_state_list 新たに発生したOpenStateのリスト
  template<PlayerTurn P>
  void AddElement(const OpenStatePattern pattern, const LineNeighborhood &line_neighbor, std::vector<OpenState> * const added_open_state_list) const;

  std::vector<OpenState> next_overline_;                 //!< 長連点
  std::vector<OpenState> next_open_four_black_;     //!< 達四点(黒)
  std::vector<OpenState> next_open_four_white_;     //!< 達四点(白)
  std::vector<OpenState> next_four_black_;              //!< 四ノビ点(黒)
  std::vector<OpenState> next_four_white_;              //!< 四ノビ点(白)
  std::vector<OpenState> next_semi_three_black_;   //!< 見かけの三ノビ点(黒)
  std::vector<OpenState> next_semi_three_white_;   //!< 見かけの三ノビ点(白)
};
}   // namespace realcore 

#include "BoardOpenState-inl.h"

#endif    // BOARD_OPEN_STATE_H