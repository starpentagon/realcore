//! @file
//! @brief 直線近傍を管理するクラス
//! @author Koichi NABETANI
//! @date 2017/03/12
#ifndef LINE_NEIGHBORHOOD_H
#define LINE_NEIGHBORHOOD_H

#include <cstdint>
#include <array>
#include <vector>

#include "BitSearch.h"

namespace realcore
{

//! @brief 直線近傍の状態を保持するStateBit
static constexpr size_t kLocalBitBoardNum = 2;
typedef std::array<StateBit, kLocalBitBoardNum> LocalBitBoard;

//! 空点状態更新用の直線近傍の長さ
static constexpr size_t kOpenStateNeighborhoodSize = 5;

// 前方宣言
enum MovePosition : std::uint8_t;
enum OpenStatePattern : std::uint8_t;
template<OpenStatePattern Pattern> class OpenState;
class BitBoard;

//! @brief moveを中心としたN路の直線近傍を管理するクラス
template<size_t N>
class LineNeighborhood{
    friend class LineNeighborhoodTest;
public:
  LineNeighborhood(const MovePosition move, const BitBoard &board);

  //! @brief 中心に状態を設定する
  //! @param S 黒石 or 白石
  //! @pre SはkBlackStoneもしくはkWhiteStoneであること
  //! @pre 中心の状態が空点であること
  template<PositionState S>
  void SetCenterState();

  //! @brief moveが達四を作る手かチェックする
  //! @retval true 達四ができている
  template<PlayerTurn P>
  const bool IsOpenFour() const;

  //! @brief moveが四を作る手かチェックする
  //! @param guard_move 防手の格納先
  //! @retval true 四ができている
  //! @note 四々、達四ができている場合はいずれかの防手を設定する
  template<PlayerTurn P>
  const bool IsFour(MovePosition * const guard_move) const;

  //! @brief moveが四々を作る手かチェックする
  //! @retval true 四々ができている
  template<PlayerTurn P>
  const bool IsDoubleFour() const;

  //! @brief moveが禁手かチェックする
  //! @param next_open_four_list 見かけの三に対する達四を作るBoardPositionのリスト
  //! @retval kForbiddenMove 禁手
  //! @retval kPossibleForbiddenMove 見かけの三々があり禁手の可能性あり
  //! @retval kNonForbiddenMove 否禁
  const ForbiddenCheckState ForbiddenCheck(std::vector<BoardPosition> * const next_open_four_list) const;

  //! @brief 空点状態を返す
  //! @param Pattern 空点状態の対象となる指し手パターン(長連点, 達四点, etc)
  //! @param open_state_list 空点状態リストの格納先
  template<OpenStatePattern Pattern, PlayerTurn P>
  void GetOpenState(std::vector< OpenState<Pattern> > *open_state_list) const;

private:
  //! @brief local_bit_board配列のindexとbit indexから対応する方向を求める
  const BoardDirection GetBoardDirection(const size_t index, const size_t bit_index) const;

  //! @brief 直線近傍でOnBitとなっているBoardPositionの位置を求める
  //! @param bit_list 位置を求めるbit
  //! @param board_position_list BoardPositionの格納先
  void GetBoardPositionList(const LocalBitBoard &bit_list, std::vector<BoardPosition> * const board_position_list) const;

  //! @brief 空点状態を返す
  //! @param Pattern 空点状態の対象となる指し手パターン(長連点, 達四点, etc)
  //! @param open_state_list 空点状態リストの格納先
  template<OpenStatePattern Pattern, PlayerTurn P>
  void GetOpenState(const LocalBitBoard &stone_bit, const LocalBitBoard &open_bit, std::vector< OpenState<Pattern> > *open_state_list) const;

  //! @brief 空点状態を返す(達四点版)
  template<OpenStatePattern Pattern, PlayerTurn P>
  void GetOpenStateOpenFour(const LocalBitBoard &stone_bit, const LocalBitBoard &open_bit, std::vector< OpenState<Pattern> > *open_state_list) const;

  //! @brief 空点状態を返す(四ノビ点版)
  template<OpenStatePattern Pattern, PlayerTurn P>
  void GetOpenStateFour(const LocalBitBoard &stone_bit, const LocalBitBoard &open_bit, std::vector< OpenState<Pattern> > *open_state_list) const;

  //! @brief 直線近傍の状態を保持する
  //! @note local_bit_board_[0]の下位32bit: 横方向(14-15bit目が中心)
  //! @note local_bit_board_[0]の上位32bit: 縦方向(46-47bit目が中心)
  //! @note local_bit_board_[1]の下位32bit: 左下斜め方向(14-15bit目が中心)
  //! @note local_bit_board_[1]の上位32bit: 右下斜め方向(46-47bit目が中心)
  LocalBitBoard local_bit_board_;
  
  //! @brief 中心位置
  MovePosition move_;
};  // class LineNeighborhood

}   // namespace realcore

#include "LineNeighborhood-inl.h"

#endif    // LINE_NEIGHBORHOOD_H