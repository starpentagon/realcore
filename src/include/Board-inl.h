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
inline void Board::EnumerateOpenFourMoves(MoveBitSet * const open_four_move_set) const
{
  const auto &board_open_state = board_open_state_list_.back();
  bit_board_.EnumerateOpenFourMoves<P>(board_open_state, open_four_move_set);
}

inline void Board::EnumerateOpenFourMoves(const bool is_black_turn, MoveBitSet * const open_four_move_set) const
{
  if(is_black_turn){
    EnumerateOpenFourMoves<kBlackTurn>(open_four_move_set);
  }else{
    EnumerateOpenFourMoves<kWhiteTurn>(open_four_move_set);
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
inline void Board::EnumeratePointOfSwordMoves(MoveBitSet * const point_of_sword_move_set) const
{
  const auto &board_open_state = board_open_state_list_.back();
  bit_board_.EnumeratePointOfSwordMoves<P>(board_open_state, point_of_sword_move_set);
}

inline void Board::EnumeratePointOfSwordMoves(const bool is_black_turn, MoveBitSet * const point_of_sword_move_set) const
{
  if(is_black_turn){
    EnumeratePointOfSwordMoves<kBlackTurn>(point_of_sword_move_set);
  }else{
    EnumeratePointOfSwordMoves<kWhiteTurn>(point_of_sword_move_set);
  }
}

template<PlayerTurn P>
void Board::EnumerateMiseMoves(MoveBitSet * const mise_move_set, MoveBitSet * const multi_mise_move_set) const
{
  const auto &board_open_state = board_open_state_list_.back();
  bit_board_.EnumerateMiseMoves<P>(board_open_state, mise_move_set, multi_mise_move_set);
}

inline void Board::EnumerateMiseMoves(const bool is_black_turn, MoveBitSet * const mise_move_set, MoveBitSet * const multi_mise_move_set) const
{
  if(is_black_turn){
    EnumerateMiseMoves<kBlackTurn>(mise_move_set, multi_mise_move_set);
  }else{
    EnumerateMiseMoves<kWhiteTurn>(mise_move_set, multi_mise_move_set);
  }
}

template<PlayerTurn P>
void Board::EnumerateDoubleSemiThreeMoves(MoveBitSet * const double_semi_three_move_set) const
{
  const auto &board_open_state = board_open_state_list_.back();
  bit_board_.EnumerateDoubleSemiThreeMoves<P>(board_open_state, double_semi_three_move_set);
}

inline void Board::EnumerateDoubleSemiThreeMoves(const bool is_black_turn, MoveBitSet * const double_semi_three_move_set) const
{
  if(is_black_turn){
    EnumerateDoubleSemiThreeMoves<kBlackTurn>(double_semi_three_move_set);
  }else{
    EnumerateDoubleSemiThreeMoves<kWhiteTurn>(double_semi_three_move_set);
  }
}

}   // namespace realcore

#endif    // BOARD_INL_H
