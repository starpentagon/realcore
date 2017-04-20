#ifndef BOARD_INL_H
#define BOARD_INL_H

#include <iostream>
#include <array>
#include <cassert>

#include "Move.h"
#include "Board.h"

namespace realcore
{
template<PlayerTurn P>
const bool Board::IsNormalMove(const MovePosition move) const
{
  // Pass以外の盤外の手は正規手ではない
  if(!IsInBoardMove(move) && move != kNullMove){
    return false;
  }

  // 相手に四があるかチェック
  if(!move_list_.empty()){
    const auto last_move = move_list_.GetLastMove();

    constexpr PlayerTurn Q = GetOpponentTurn(P);
    MovePosition guard_move;
    const auto is_opponent_four = bit_board_.IsFourMoveOnBoard<Q>(last_move, &guard_move);

    if(is_opponent_four){
      // 相手に四がある
      return guard_move == move;
    }
  }

  // 相手に四がない場合、Passは正規手
  if(move == kNullMove){
    return true;
  }

  if(bit_board_.GetState(move) != kOpenPosition){
    return false;
  }

  if(bit_board_.IsForbiddenMove<P>(move)){
    return false;
  }

  return true;
}

inline const bool Board::IsNormalMove(const bool black_turn, const MovePosition move) const
{
  if(black_turn){
    return IsNormalMove<kBlackTurn>(move);
  }else{
    return IsNormalMove<kWhiteTurn>(move);
  }
}

template<PlayerTurn P>
const bool Board::IsTerminateMove(const MovePosition move) const
{
  assert(IsNormalMove<P>(move));

  if(bit_board_.IsOpenFourMove<P>(move)){
    // 達四が作れる
    return true;
  }

  if(P == kWhiteTurn){
    // 四々がつくれる
    if(bit_board_.IsDoubleFourMove<kWhiteTurn>(move)){
      return true;
    }

    MovePosition guard_move;
    const bool is_four = bit_board_.IsFourMove<kWhiteTurn>(move, &guard_move);

    if(is_four){
      BitBoard bit_board(bit_board_);
      bit_board.SetState<kWhiteStone>(move);

      if(bit_board.IsForbiddenMove<kBlackTurn>(guard_move)){
        return true;
      }
    }
  }

  return false;
}

template<PlayerTurn P>
const bool Board::IsTerminateMove(const MovePair &four_pair) const
{
  assert(IsNormalMove<P>(four_pair.first));

  if(bit_board_.IsOpenFourMove<P>(four_pair.first)){
    // 達四が作れる
    return true;
  }

  if(P == kWhiteTurn){
    // 四々がつくれる
    if(bit_board_.IsDoubleFourMove<kWhiteTurn>(four_pair.first)){
      return true;
    }

    BitBoard bit_board(bit_board_);
    bit_board.SetState<kWhiteStone>(four_pair.first);

    if(bit_board.IsForbiddenMove<kBlackTurn>(four_pair.second)){
      return true;
    }
  }

  return false;
}

inline const bool Board::IsTerminateMove(const bool black_turn, const MovePosition move) const
{
  if(black_turn){
    return IsTerminateMove<kBlackTurn>(move);
  }else{
    return IsTerminateMove<kWhiteTurn>(move);
  }
}

}   // namespace realcore

#endif    // BOARD_INL_H
