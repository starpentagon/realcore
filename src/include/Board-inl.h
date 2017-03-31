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
    SetState<kBlackStone>(move);
  }else{
    SetState<kWhiteStone>(move);
  }

  move_list_ += move;
}

inline void Board::UndoMove()
{
  assert(move_list_.empty());

  const auto move = move_list_.GetLastMove();
  SetState<kOpenPosition>(move);
  --move_list_;
}

inline const BoardPosition GetBoardPosition(const size_t index, const size_t shift)
{
  return (32 * index + shift / 2);
}

inline const BoardDirection GetBoardDirection(const BoardPosition board_position)
{
  constexpr size_t kMaxLateralBoardPosition = 255;
  constexpr size_t kMaxVerticalBoardPosition = 511;
  constexpr size_t kMaxLeftDiagonalBoardPosition = 767;
  constexpr size_t kMaxRightDiagonalBoardPosition = 1023;

  if(board_position <= kMaxLateralBoardPosition){
    return kLateralDirection;
  }else if(board_position <= kMaxVerticalBoardPosition){
    return kVerticalDirection;
  }else if(board_position <= kMaxLeftDiagonalBoardPosition){
    return kLeftDiagonalDirection;
  }else if(board_position <= kMaxRightDiagonalBoardPosition){
    return kRightDiagonalDirection;
  }else{
    assert(false);
    return kLateralDirection;
  }
}

inline const void GetBoardCordinate(const BoardPosition board_position, Cordinate * const x, Cordinate * const y)
{
  assert(x != nullptr);
  assert(y != nullptr);
  
  const BoardDirection board_direction = GetBoardDirection(board_position);

  if(board_direction == kLateralDirection){
    *x = board_position % 16;
    *y = board_position / 16;
  }else if(board_direction == kVerticalDirection){
    *x = (board_position - 256) / 16;
    *y = (board_position - 256) % 16;
  }else if(board_direction == kLeftDiagonalDirection){
    *y = (board_position - 512 + 1) % 16;
    *x = ((board_position - 512) / 16 - *y + 18) % 16;
  }else if(board_direction == kRightDiagonalDirection){
    *y = (board_position - 768 + 1) % 16;
    *x = (*y - (board_position - 768) / 16 + 30) % 16;
  }else{
    assert(false);
  }
}

inline const MovePosition GetBoardMove(const BoardPosition board_position)
{
  Cordinate x = 0, y = 0;
  GetBoardCordinate(board_position, &x, &y);

  if(!IsInBoard(x, y))
  {
    return kInvalidMove;
  }

  return GetMove(x, y);
}

inline void GetBitBoardIndexList(const Cordinate x, const Cordinate y, std::array<size_t, kBoardDirectionNum> * const index_list)
{
  assert(IsInBoard(x, y));

  (*index_list)[kLateralDirection] = y / 2;
  (*index_list)[kVerticalDirection] = x / 2 + 8;
  (*index_list)[kLeftDiagonalDirection] = ((x + y - 2) % 16) / 2 + 16;
  (*index_list)[kRightDiagonalDirection] = ((y - x + 14) % 16) / 2 + 24;
}

inline void GetBitBoardShiftList(const Cordinate x, const Cordinate y, std::array<size_t, kBoardDirectionNum> * const shift_list)
{
  assert(IsInBoard(x, y));

  (*shift_list)[kLateralDirection] = 2 * x + 32 * (y % 2);
  (*shift_list)[kVerticalDirection] = 2 * y + 32 * (x % 2);
  (*shift_list)[kLeftDiagonalDirection] = 2 * (y - 1) + 32 * ((x + y) % 2);
  (*shift_list)[kRightDiagonalDirection] = 2 * (y - 1) + 32 * ((x + y) % 2);
}

template<size_t N>
void Board::GetLineNeighborhoodStateBit(const MovePosition move, std::array<StateBit, kBoardDirectionNum> * const line_neighborhood_list) const
{
  static_assert(1 <= N && N <= 7, "N must be in [1, 7]");
  assert(line_neighborhood_list != nullptr);

  std::fill(line_neighborhood_list->begin(), line_neighborhood_list->end(), 0);

  Cordinate x = 0, y = 0;
  GetMoveCordinate(move, &x, &y);

  if(!IsInBoard(x, y)){
    return;
  }

  std::array<size_t, kBoardDirectionNum> index_list, shift_list;
  GetBitBoardIndexList(x, y, &index_list);
  GetBitBoardShiftList(x, y, &shift_list);

  constexpr size_t kBitNumber = 2 * (2 * N + 1);  // (2 * N + 1)地点（指定の地点 + 左右にN路ずつ） * 2bit/地点
  constexpr uint64_t kLineNeighborhoodMask = GetConsectiveBit<kBitNumber>();  // (2*N+1)地点の状態を取り出すマスク

  for(auto direction : GetBoardDirection()){
    const size_t index = index_list[direction];
    const size_t shift = shift_list[direction];

    constexpr size_t kMaskShift = 2 * N;    // moveを中心とした左右対称なマスクを生成するためN個の状態(2 * N bit)分を右シフト

    // moveを中心としたマスクを生成
    uint64_t neighborhood_mask = kLineNeighborhoodMask;

    if(shift >= kMaskShift){
      neighborhood_mask <<= shift - kMaskShift;
    }else{
      neighborhood_mask >>= kMaskShift - shift;
    }

    const StateBit neighborhood_state_bit = (bit_board_[index] & neighborhood_mask);

    // 14-15bit目がmove位置になるように揃える
    constexpr size_t kCenterAlignment = 14;

    if(shift >= kCenterAlignment){
      (*line_neighborhood_list)[direction] = neighborhood_state_bit >> (shift - kCenterAlignment);
    }else{
      (*line_neighborhood_list)[direction] = neighborhood_state_bit << (kCenterAlignment - shift);
    }
  }
}
}   // namespace realcore

#endif    // BOARD_INL_H
