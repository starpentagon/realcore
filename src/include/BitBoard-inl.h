#ifndef BIT_BOARD_INL_H
#define BIT_BOARD_INL_H

#include "Move.h"
#include "BitBoard.h"

namespace realcore{

inline const size_t GetBitBoardIndex(const BoardPosition board_position)
{
  const size_t index = board_position / 32;
  assert(index < kBitBoardElementNum);

  return index;
}

inline const size_t GetBitBoardShift(const BoardPosition board_position)
{
  const size_t shift_val = 2 * (board_position % 32);
  return shift_val;
}

inline const PositionState GetState(const Bitboard bit_board, const MovePosition move)
{
  // BoardPositionの横方向とMovePositionは同じ値になる
  // see /home/subaru/dev/realcore/doc/01_data_definition/board_definition.xlsx
  const BoardPosition board_position = static_cast<BoardPosition>(move);
  return GetState(bit_board, board_position);
}

inline const PositionState GetState(const Bitboard bit_board, const BoardPosition board_position)
{
  const size_t index = GetBitBoardIndex(board_position);
  const size_t shift = GetBitBoardShift(board_position);

  constexpr StateBit state_mask = 0b11ULL;    // 下位2bit mask
  const StateBit state_bit = (bit_board[index] >> shift) & state_mask;

  return static_cast<PositionState>(state_bit);
}

template<>
inline void SetState<kOverBoard>(Bitboard * const bit_board, const MovePosition move)
{
  assert(false);
}

template<>
inline void SetState<kBlackStone>(const Bitboard bit_board, const MovePosition move)
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
inline void SetState<kWhiteStone>(const Bitboard bit_board, const MovePosition move)
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
inline void SetState<kOpenPosition>(const Bitboard bit_board, const MovePosition move)
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

inline void SetState(const Bitboard bit_board, const MovePosition move, const PositionState state)
{
  switch(state){
  case kOverBoard:
    SetState<kOverBoard>(bit_board, move);
    break;
  
  case kBlackStone:
    SetState<kBlackStone>(bit_board, move);
    break;

  case kWhiteStone:
    SetState<kWhiteStone>(bit_board, move);
    break;

  case kOpenPosition:
    SetState<kOpenPosition>(bit_board, move);
    break;
  }
}
}



#endif    // BIT_BOARD_INL_H
