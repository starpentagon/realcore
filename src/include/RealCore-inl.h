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

inline const std::array<BoardSymmetry, kBoardSymmetryNum>& GetBoardSymmetry(){
  static const std::array<BoardSymmetry, kBoardSymmetryNum> board_symmetry_list{{
    kIdenticalSymmetry, kHorizontalSymmetry, kVerticalSymmetry, kCentricSymmetry, 
    kDiagonalSymmetry1, kDiagonalSymmetry2, kDiagonalSymmetry3, kDiagonalSymmetry4
  }};

  return board_symmetry_list;
}

inline constexpr PositionState GetPlayerStone(const PlayerTurn player)
{
  return player == kBlackTurn ? kBlackStone : kWhiteStone;
}

}   // realcore

#endif    // REAL_CORE_INL_H