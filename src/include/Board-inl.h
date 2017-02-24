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

  constexpr BitBoard state_mask = 0b11ULL;    // 下位2bit mask
  const BitBoard state_bit = (bit_board_[index] >> shift) & state_mask;

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

  std::array<BoardPosition, kBoardDirectionNum> index_list;   // BitBoard配列のindex
  GetBitBoardIndexList(x, y, &index_list);

  std::array<BoardPosition, kBoardDirectionNum> shift_list;   // BitBoard配列のshift量
  GetBitBoardShiftList(x, y, &shift_list);

  constexpr BitBoard black_stone_xor_mask = 0b10ULL;    // kOpenPosition(0b11) XOR 0b10(mask) = 0b01(kBlackStone)

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

  std::array<BoardPosition, kBoardDirectionNum> index_list;   // BitBoard配列のindex
  GetBitBoardIndexList(x, y, &index_list);

  std::array<BoardPosition, kBoardDirectionNum> shift_list;   // BitBoard配列のshift量
  GetBitBoardShiftList(x, y, &shift_list);
  
  constexpr BitBoard white_stone_xor_mask = 0b01ULL;    // kOpenPosition(0b11) XOR 0b01(mask) = 0b10(kWhiteStone)

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

  std::array<BoardPosition, kBoardDirectionNum> index_list;   // BitBoard配列のindex
  GetBitBoardIndexList(x, y, &index_list);

  std::array<BoardPosition, kBoardDirectionNum> shift_list;   // BitBoard配列のshift量
  GetBitBoardShiftList(x, y, &shift_list);
  
  constexpr BitBoard open_position_or_mask = 0b11ULL;    // Any(0b**) OR 0b11(mask) = 0b11(kOpenPosition)

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
  assert(index < kBitBoardNum);

  return index;
}

inline const size_t Board::GetBitBoardShift(const BoardPosition board_position) const
{
  const size_t shift_val = 2 * (board_position % 32);
  return shift_val;
}

inline void Board::GetBitBoardIndexList(const Cordinate x, const Cordinate y, std::array<size_t, kBoardDirectionNum> *index_list) const
{
  assert(IsInBoard(x, y));

  (*index_list)[kLateralDirection] = y / 2;
  (*index_list)[kVerticalDirection] = x / 2 + 8;
  (*index_list)[kLeftDiagonalDirection] = ((x + y - 2) % 16) / 2 + 16;
  (*index_list)[kRightDiagonalDirection] = ((y - x + 14) % 16) / 2 + 24;
}

inline void Board::GetBitBoardShiftList(const Cordinate x, const Cordinate y, std::array<size_t, kBoardDirectionNum> *shift_list) const
{
  assert(IsInBoard(x, y));

  (*shift_list)[kLateralDirection] = 2 * x + 32 * (y % 2);
  (*shift_list)[kVerticalDirection] = 2 * y + 32 * (x % 2);
  (*shift_list)[kLeftDiagonalDirection] = 2 * (y - 1) + 32 * ((x + y) % 2);
  (*shift_list)[kRightDiagonalDirection] = 2 * (y - 1) + 32 * ((x + y) % 2);
}

constexpr bool IsInBoard(const Cordinate x, const Cordinate y)
{
  assert(0 <= x && x <= static_cast<Cordinate>(kBoardLineNum));
  assert(0 <= y && y <= static_cast<Cordinate>(kBoardLineNum));

  const bool x_in_board = x != 0;
  const bool y_in_board = y != 0;

  return x_in_board && y_in_board;
}

}   // namespace realcore

#endif    // BOARD_INL_H
