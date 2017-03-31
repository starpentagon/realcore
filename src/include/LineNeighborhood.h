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
typedef StateBit LocalBitBoard;

// 前方宣言
enum MovePosition : std::uint8_t;
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

  //! @brief moveが禁手かチェックする
  //! @param next_open_four_list 見かけの三に対する達四を作るBoardPositionのリスト
  //! @retval kForbiddenMove 禁手
  //! @retval kPossibleForbiddenMove 見かけの三々があり禁手の可能性あり
  //! @retval kNonForbiddenMove 否禁
  const ForbiddenCheckState ForbiddenCheck(std::vector<BoardPosition> * const next_open_four_list) const;

private:
  //! @brief local_bit_board配列のindexとbit indexから対応する方向を求める
  const BoardDirection GetBoardDirection(const size_t index, const size_t bit_index) const;

  static constexpr size_t kLocalBitBoardNum = 2;

  //! @brief 直線近傍の状態を保持する
  //! @note local_bit_board_[0]の下位32bit: 横方向(14-15bit目が中心)
  //! @note local_bit_board_[0]の上位32bit: 縦方向(46-47bit目が中心)
  //! @note local_bit_board_[1]の下位32bit: 左下斜め方向(14-15bit目が中心)
  //! @note local_bit_board_[1]の上位32bit: 右下斜め方向(46-47bit目が中心)
  std::array<LocalBitBoard, kLocalBitBoardNum> local_bit_board_;
  
  //! @brief 中心位置
  MovePosition move_;
};  // class LineNeighborhood

}   // namespace realcore

#include "LineNeighborhood-inl.h"

#endif    // LINE_NEIGHBORHOOD_H