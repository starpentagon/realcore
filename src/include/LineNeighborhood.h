//! @file
//! @brief 直線近傍を管理するクラス
//! @author Koichi NABETANI
//! @date 2017/03/12
#ifndef LINE_NEIGHBORHOOD_H
#define LINE_NEIGHBORHOOD_H

#include <cstdint>
#include <array>

#include "BitSearch.h"

namespace realcore
{

//! @brief 直線近傍の状態を保持するStateBit
typedef StateBit LocalBitBoard;

// 前方宣言
enum MovePosition : std::uint8_t;
class Board;

//! @brief move_positionを中心としたN路の直線近傍を管理するクラス
template<size_t N>
class LineNeighborhood{
public:
  LineNeighborhood(const MovePosition move_position, const Board *board);

private:
  static constexpr size_t kLocalBitBoardNum = 2;

  //! @brief 直線近傍の状態を保持する
  //! @note local_bit_board_[0]の下位32bit: 横方向
  //! @note local_bit_board_[0]の上位32bit: 縦方向
  //! @note local_bit_board_[1]の下位32bit: 左下斜め方向
  //! @note local_bit_board_[1]の上位32bit: 右下斜め方向  
  std::array<LocalBitBoard, kLocalBitBoardNum> local_bit_board_;
};  // class LineNeighborhood

}   // namespace realcore

#endif    // LINE_NEIGHBORHOOD_H