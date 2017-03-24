#ifndef BOARD_INL_H
#define BOARD_INL_H

#include <iostream>
#include <array>
#include <cassert>

#include "Move.h"
#include "Board.h"

namespace realcore
{

inline const PositionState Board::GetState(const MovePosition move) const
{
  // BoardPositionの横方向とMovePositionは同じ値になる
  // see /home/subaru/dev/realcore/doc/01_data_definition/board_definition.xlsx
  const BoardPosition board_position = static_cast<BoardPosition>(move);
  return GetState(board_position);
}

inline const PositionState Board::GetState(const BoardPosition board_position) const
{
  const size_t index = GetBitBoardIndex(board_position);
  const size_t shift = GetBitBoardShift(board_position);

  constexpr StateBit state_mask = 0b11ULL;    // 下位2bit mask
  const StateBit state_bit = (bit_board_[index] >> shift) & state_mask;

  return static_cast<PositionState>(state_bit);
}

template<>
inline void Board::SetState<kOverBoard>(const MovePosition move)
{
  assert(false);
}

template<>
inline void Board::SetState<kBlackStone>(const MovePosition move)
{
  Cordinate x, y;
  GetMoveCordinate(move, &x, &y);

  if(!IsInBoard(x, y)){
    return;
  }

  std::array<size_t, kBoardDirectionNum> index_list;   // StateBit配列のindex
  GetBitBoardIndexList(x, y, &index_list);

  std::array<size_t, kBoardDirectionNum> shift_list;   // StateBit配列のshift量
  GetBitBoardShiftList(x, y, &shift_list);

  constexpr StateBit black_stone_xor_mask = 0b10ULL;    // kOpenPosition(0b11) XOR 0b10(mask) = 0b01(kBlackStone)

  for(size_t i=0; i<kBoardDirectionNum; i++){
    const size_t index = index_list[i];
    const size_t shift = shift_list[i];

    bit_board_[index] ^= (black_stone_xor_mask << shift);    
  }
}

template<>
inline void Board::SetState<kWhiteStone>(const MovePosition move)
{
  Cordinate x, y;
  GetMoveCordinate(move, &x, &y);

  if(!IsInBoard(x, y)){
    return;
  }

  std::array<size_t, kBoardDirectionNum> index_list;   // StateBit配列のindex
  GetBitBoardIndexList(x, y, &index_list);

  std::array<size_t, kBoardDirectionNum> shift_list;   // StateBit配列のshift量
  GetBitBoardShiftList(x, y, &shift_list);
  
  constexpr StateBit white_stone_xor_mask = 0b01ULL;    // kOpenPosition(0b11) XOR 0b01(mask) = 0b10(kWhiteStone)

  for(size_t i=0; i<kBoardDirectionNum; i++){
    const size_t index = index_list[i];
    const size_t shift = shift_list[i];

    bit_board_[index] ^= (white_stone_xor_mask << shift);    
  }
}

template<>
inline void Board::SetState<kOpenPosition>(const MovePosition move)
{
  Cordinate x, y;
  GetMoveCordinate(move, &x, &y);

  if(!IsInBoard(x, y)){
    return;
  }

  std::array<size_t, kBoardDirectionNum> index_list;   // StateBit配列のindex
  GetBitBoardIndexList(x, y, &index_list);

  std::array<size_t, kBoardDirectionNum> shift_list;   // StateBit配列のshift量
  GetBitBoardShiftList(x, y, &shift_list);
  
  constexpr StateBit open_position_or_mask = 0b11ULL;    // Any(0b**) OR 0b11(mask) = 0b11(kOpenPosition)

  for(size_t i=0; i<kBoardDirectionNum; i++){
    const size_t index = index_list[i];
    const size_t shift = shift_list[i];

    bit_board_[index] |= (open_position_or_mask << shift);
  }
}

inline void Board::SetState(const MovePosition move, const PositionState state)
{
  switch(state){
  case kOverBoard:
    SetState<kOverBoard>(move);
    break;
  
  case kBlackStone:
    SetState<kBlackStone>(move);
    break;

  case kWhiteStone:
    SetState<kWhiteStone>(move);
    break;

  case kOpenPosition:
    SetState<kOpenPosition>(move);
    break;
  }
}

inline const size_t Board::GetBitBoardIndex(const BoardPosition board_position) const
{
  const size_t index = board_position / 32;
  assert(index < kBitBoardElementNum);

  return index;
}

inline const size_t Board::GetBitBoardShift(const BoardPosition board_position) const
{
  const size_t shift_val = 2 * (board_position % 32);
  return shift_val;
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

constexpr inline bool IsInBoard(const Cordinate x, const Cordinate y)
{
  assert(0 <= x && x <= static_cast<Cordinate>(kBoardLineNum));
  assert(0 <= y && y <= static_cast<Cordinate>(kBoardLineNum));

  const bool x_in_board = x != 0;
  const bool y_in_board = y != 0;

  return x_in_board && y_in_board;
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
