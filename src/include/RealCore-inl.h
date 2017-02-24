#ifndef REAL_CORE_INL_H
#define REAL_CORE_INL_H

#include <array>

#include "RealCore.h"

namespace realcore
{

inline const std::array<BoardDirection, kBoardDirectionNum>& GetBoardDirection(){
  static const std::array<BoardDirection, kBoardDirectionNum> board_direction_list{{
    kLateralDirection, kVerticalDirection, kLeftDiagonalDirection, kRightDiagonalDirection
  }};

  return board_direction_list;
}

}   // realcore

#endif    // REAL_CORE_INL_H