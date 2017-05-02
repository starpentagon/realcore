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

}   // namespace realcore

#endif    // BOARD_INL_H
