#ifndef LINE_NEIGHBORHOOD_INL_H
#define LINE_NEIGHBORHOOD_INL_H

#include "Board.h"
#include "LineNeighborhood.h"

namespace realcore
{

template<size_t N>
LineNeighborhood<N>::LineNeighborhood(const MovePosition move, const Board &board)
: local_bit_board_{{0}}
{
  std::array<StateBit, kBoardDirectionNum> line_neighborhood;
  board.GetLineNeighborhoodStateBit<N>(move, &line_neighborhood);

  local_bit_board_[0] |= line_neighborhood[kLateralDirection];
  local_bit_board_[0] |= line_neighborhood[kVerticalDirection] << 32ULL;
  local_bit_board_[1] |= line_neighborhood[kLeftDiagonalDirection];
  local_bit_board_[1] |= line_neighborhood[kRightDiagonalDirection] << 32ULL;
}


}   // namespace realcore

#endif    // LINE_NEIGHBORHOOD_INL_H
