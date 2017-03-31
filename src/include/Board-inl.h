#ifndef BOARD_INL_H
#define BOARD_INL_H

#include <iostream>
#include <array>
#include <cassert>

#include "Move.h"
#include "Board.h"

namespace realcore
{

inline void Board::MakeMove(const MovePosition move)
{
  if(move_list_.IsBlackTurn()){
    bit_board_.SetState<kBlackStone>(move);
  }else{
    bit_board_.SetState<kWhiteStone>(move);
  }

  move_list_ += move;
}

inline void Board::UndoMove()
{
  assert(move_list_.empty());

  const auto move = move_list_.GetLastMove();
  bit_board_.SetState<kOpenPosition>(move);
  --move_list_;
}

}   // namespace realcore

#endif    // BOARD_INL_H
