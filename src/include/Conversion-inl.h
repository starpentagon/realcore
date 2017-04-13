#ifndef CONVERSION_INL_H
#define CONVERSION_INL_H

#include <cassert>

#include "RealCore.h"
#include "Move.h"
#include "Conversion.h"

namespace realcore{
inline const size_t GetBitBoardIndex(const BoardPosition board_position)
{
  assert(board_position < kBoardPositionNum);
  const size_t index = board_position / 32;

  return index;
}

template<BoardDirection kDirection>
inline const size_t GetBitBoardIndex(const Cordinate x, const Cordinate y)
{
  assert(IsInBoard(x, y));

  switch(kDirection)
  {
  case kLateralDirection:
    return y / 2;
  case kVerticalDirection:
    return x / 2 + 8;
  case kLeftDiagonalDirection:
    return ((x + y - 2) % 16) / 2 + 16;
  case kRightDiagonalDirection: 
    return ((y - x + 14) % 16) / 2 + 24;
  default:
    assert(false);
    return 0;
  }
}

inline const size_t GetBitBoardIndex(const Cordinate x, const Cordinate y, const BoardDirection direction)
{
  switch(direction)
  {
  case kLateralDirection:
    return GetBitBoardIndex<kLateralDirection>(x, y);
  case kVerticalDirection:
    return GetBitBoardIndex<kVerticalDirection>(x, y);
  case kLeftDiagonalDirection:
    return GetBitBoardIndex<kLeftDiagonalDirection>(x, y);
  case kRightDiagonalDirection: 
    return GetBitBoardIndex<kRightDiagonalDirection>(x, y);
  default:
    assert(false);
    return 0;
  }
}

inline const size_t GetBitBoardShift(const BoardPosition board_position)
{
  const size_t shift_val = 2 * (board_position % 32);
  return shift_val;
}

template<BoardDirection kDirection>
inline const size_t GetBitBoardShift(const Cordinate x, const Cordinate y)
{
  assert(IsInBoard(x, y));

  switch(kDirection)
  {
  case kLateralDirection:
    return 2 * x + 32 * (y % 2);
  case kVerticalDirection:
    return 2 * y + 32 * (x % 2);
  case kLeftDiagonalDirection:
  case kRightDiagonalDirection: 
    return 2 * (y - 1) + 32 * ((x + y) % 2);
  default:
    assert(false);
    return 0;
  }
}

inline const size_t GetBitBoardShift(const Cordinate x, const Cordinate y, const BoardDirection direction)
{
  switch(direction)
  {
  case kLateralDirection:
    return GetBitBoardShift<kLateralDirection>(x, y);
  case kVerticalDirection:
    return GetBitBoardShift<kVerticalDirection>(x, y);
  case kLeftDiagonalDirection:
    return GetBitBoardShift<kLeftDiagonalDirection>(x, y);
  case kRightDiagonalDirection: 
    return GetBitBoardShift<kRightDiagonalDirection>(x, y);
  default:
    assert(false);
    return 0;
  }
}

inline void GetBitBoardIndexList(const Cordinate x, const Cordinate y, std::array<size_t, kBoardDirectionNum> * const index_list)
{
  for(const auto direction : GetBoardDirection()){
    (*index_list)[direction] = GetBitBoardIndex(x, y, direction);
  }
}

inline void GetBitBoardShiftList(const Cordinate x, const Cordinate y, std::array<size_t, kBoardDirectionNum> * const shift_list)
{
  for(const auto direction : GetBoardDirection()){
    (*shift_list)[direction] = GetBitBoardShift(x, y, direction);
  }
}

inline const BoardPosition GetBoardPosition(const size_t index, const size_t shift)
{
  return (32 * index + shift / 2);
}

inline const BoardPosition GetBoardPosition(const MovePosition move, const BoardDirection direction)
{
  Cordinate x = 0, y = 0;
  GetMoveCordinate(move, &x, &y);

  const size_t index = GetBitBoardIndex(x, y, direction);
  const size_t shift = GetBitBoardShift(x, y, direction);

  return GetBoardPosition(index, shift);
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

}

#endif
