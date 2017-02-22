#ifndef BOARD_INL_H
#define BOARD_INL_H

#include "Board.h"

namespace realcore
{
constexpr bool IsInBoard(const Cordinate x, const Cordinate y)
{
  const bool x_in_board = 1 <= x && x <= kBoardLineNum;
  const bool y_in_board = 1 <= y && y <= kBoardLineNum;

  return x_in_board && y_in_board;
}

}

#endif    // BOARD_INL_H
