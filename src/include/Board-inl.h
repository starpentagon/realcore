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
  if(!board_move_sequence_.empty()){
    const auto last_move = board_move_sequence_.GetLastMove();

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
    // 四々が作れるかチェックする
    if(bit_board_.IsDoubleFourMove<kWhiteTurn>(move)){
      return true;
    }

    // 禁手に極められるかチェックする
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

template<PlayerTurn P>
inline void Board::EnumerateFourMoves(MoveBitSet * const four_move_set) const
{
  const auto &board_open_state = board_open_state_list_.back();
  bit_board_.EnumerateFourMoves<P>(board_open_state, four_move_set);
}

inline void Board::EnumerateFourMoves(const bool is_black_turn, MoveBitSet * const four_move_set) const
{
  if(is_black_turn){
    EnumerateFourMoves<kBlackTurn>(four_move_set);
  }else{
    EnumerateFourMoves<kWhiteTurn>(four_move_set);
  }
}

template<PlayerTurn P>
inline void Board::EnumerateFourMoves(std::vector<MovePair> * const four_move_list) const
{
  const auto &board_open_state = board_open_state_list_.back();
  bit_board_.EnumerateFourMoves<P>(board_open_state, four_move_list);
}

inline void Board::EnumerateFourMoves(const bool is_black_turn, std::vector<MovePair> * const four_move_list) const
{
  if(is_black_turn){
    EnumerateFourMoves<kBlackTurn>(four_move_list);
  }else{
    EnumerateFourMoves<kWhiteTurn>(four_move_list);
  }
}

template<PlayerTurn P>
inline void Board::EnumerateSemiThreeMoves(MoveBitSet * const semi_three_move_set) const
{
  const auto &board_open_state = board_open_state_list_.back();
  bit_board_.EnumerateSemiThreeMoves<P>(board_open_state, semi_three_move_set);
}

inline void Board::EnumerateSemiThreeMoves(const bool is_black_turn, MoveBitSet * const semi_three_move_set) const
{
  if(is_black_turn){
    EnumerateSemiThreeMoves<kBlackTurn>(semi_three_move_set);
  }else{
    EnumerateSemiThreeMoves<kWhiteTurn>(semi_three_move_set);
  }
}

template<PlayerTurn P>
const bool Board::GetTerminateGuard(MoveBitSet * const guard_move_set) const
{
  const auto &board_open_state = board_open_state_list_.back();
  bit_board_.GetTerminateGuard<P>(board_open_state, guard_move_set);
}

}   // namespace realcore

#endif    // BOARD_INL_H
