//! @file
//! @brief 盤面空点状態の管理クラス
//! @author Koichi NABETANI
//! @date 2017/04/05

#ifndef BOARD_OPEN_STATE_H
#define BOARD_OPEN_STATE_H

#include "OpenState.h"

namespace realcore
{

//! 前方宣言
//! @brief 空点状態の差分計算クラス
class BoardOpenState
{
public:
  BoardOpenState();
  BoardOpenState(const BoardOpenState &board_open_state);

  //! @brief 着手による空点状態の変更を反映する
  //! @param P 手番
  //! @param move 着手
  //! @param bit_board BitBoard
  //! @pre moveは着手後であること
  template<PlayerTurn P>
  void Update(const MovePosition move, const BitBoard &bit_board);
  void Update(const bool black_turn, const MovePosition move, const BitBoard &bit_board);

  //! @brief 長連点のリストを返す
  const std::vector< OpenState<kNextOverline> >& GetNextOverline() const;

  //! @brief 達四点(黒)のリストを返す
  const std::vector< OpenState<kNextOpenFourBlack> >& GetNextOpenFourBlack() const;

  //! @brief 達四点(白)のリストを返す
  const std::vector< OpenState<kNextOpenFourWhite> >& GetNextOpenFourWhite() const;

  //! @brief 四ノビ点(黒)のリストを返す
  const std::vector< OpenState<kNextFourBlack> >& GetNextFourBlack() const;

  //! @brief 四ノビ点(白)のリストを返す
  const std::vector< OpenState<kNextFourWhite> >& GetNextFourWhite() const;

  //! @brief 見かけの三ノビ点(黒)のリストを返す
  const std::vector< OpenState<kNextSemiThreeBlack> >& GetNextSemiThreeBlack() const;

  //! @brief 見かけの三ノビ点(白)のリストを返す
  const std::vector< OpenState<kNextSemiThreeWhite> >& GetNextSemiThreeWhite() const;
  
private:
  //! @brief 着手の影響を受けるOpenState要素を削除したリストを生成する
  //! @param P moveの手番
  //! @param open_state_list move着手前のOpenStateのリスト
  //! @param move 着手
  //! @param cleared_open_state_list move着手による影響分を除外したOpenStateのリスト
  template<OpenStatePattern Pattern, PlayerTurn P>
  void ClearInfluencedElement(const std::vector< OpenState<Pattern> > &open_state_list, const MovePosition move, std::vector< OpenState<Pattern> > * const cleared_open_state_list) const;

  //! @brief 着手によるOpenState要素を追加する
  //! @param added_open_state_list 新たに発生したOpenStateのリスト
  template<OpenStatePattern Pattern, PlayerTurn P>
  void AddElement(const LineNeighborhood<kOpenStateNeighborhoodSize> &line_neighbor, std::vector< OpenState<Pattern> > * const added_open_state_list) const;

  std::vector< OpenState<kNextOverline> > next_overline_;          //!< 長連点
  std::vector< OpenState<kNextOpenFourBlack> > next_open_four_black_;   //!< 達四点(黒)
  std::vector< OpenState<kNextOpenFourWhite> > next_open_four_white_;   //!< 達四点(白)
  std::vector< OpenState<kNextFourBlack> > next_four_black_;        //!< 四ノビ点(黒)
  std::vector< OpenState<kNextFourWhite> > next_four_white_;        //!< 四ノビ点(白)
  std::vector< OpenState<kNextSemiThreeBlack> > next_semi_three_black_;  //!< 見かけの三ノビ点(黒)
  std::vector< OpenState<kNextSemiThreeWhite> > next_semi_three_white_;  //!< 見かけの三ノビ点(白)
};
}   // namespace realcore 

#include "BoardOpenState-inl.h"

#endif    // BOARD_OPEN_STATE_H