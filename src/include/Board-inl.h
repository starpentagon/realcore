#ifndef BOARD_INL_H
#define BOARD_INL_H

#include <array>
#include <cassert>

#include "Move.h"
#include "Board.h"

namespace realcore
{

inline const PositionState Board::GetState(const MovePosition move) const
{
  const BoardPosition board_position = GetReadBoardPosition(move, kLateralDirection);
  return GetState(board_position);
}

inline const PositionState Board::GetState(const BoardPosition board_position) const
{
  assert(!IsUndefinedBoardPosition(board_position) && !IsWriteOnlyBoardPosition(board_position));

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
  const auto &direction_list = GetBoardDirection();
  constexpr BitBoard black_stone_xor_mask = 0b10ULL;    // kOpenPosition(0b11) XOR 0b10(mask) = 0b01(kBlackStone)

  for(const auto direction : direction_list){
    const BoardPosition board_position = GetWriteBoardPosition(move, direction);

    assert(!IsReadOnlyBoardPosition(board_position));
    assert(IsWriteOnlyBoardPosition(board_position) || GetState(board_position) == kOpenPosition);

    const size_t index = GetBitBoardIndex(board_position);
    const size_t shift = GetBitBoardShift(board_position);

    bit_board_[index] ^= (black_stone_xor_mask << shift);
  }
}

template<>
inline void Board::SetState<kWhiteStone>(const MovePosition move)
{
  const auto &direction_list = GetBoardDirection();
  constexpr BitBoard white_stone_xor_mask = 0b01ULL;    // kOpenPosition(0b11) XOR 0b01(mask) = 0b10(kWhiteStone)

  for(const auto direction : direction_list){
    const BoardPosition board_position = GetWriteBoardPosition(move, direction);

    assert(!IsReadOnlyBoardPosition(board_position));
    assert(IsWriteOnlyBoardPosition(board_position) || GetState(board_position) == kOpenPosition);

    const size_t index = GetBitBoardIndex(board_position);
    const size_t shift = GetBitBoardShift(board_position);

    bit_board_[index] ^= (white_stone_xor_mask << shift);
  }
}

template<>
inline void Board::SetState<kOpenPosition>(const MovePosition move)
{
  const auto &direction_list = GetBoardDirection();
  constexpr BitBoard open_position_or_mask = 0b11ULL;    // Any(0b**) OR 0b11(mask) = 0b11(kOpenPosition)

  for(const auto direction : direction_list){
    const BoardPosition board_position = GetWriteBoardPosition(move, direction);

    assert(!IsUndefinedBoardPosition(board_position) && !IsReadOnlyBoardPosition(board_position));

    const size_t index = GetBitBoardIndex(board_position);
    const size_t shift = GetBitBoardShift(board_position);

    bit_board_[index] |= (open_position_or_mask << shift);
  }
}

inline const BoardPosition Board::GetReadBoardPosition(const MovePosition move, const BoardDirection direction) const
{
  typedef std::array<std::uint8_t, kMoveNum> BoardPositionTable;

  // 指し手位置に対応する読込用BoardPosition Table
  static const BoardPositionTable read_board_position_lateral{{
    #include "def/ReadBoardPositionLateral.h"
  }};

  static const BoardPositionTable read_board_position_vertical{{
    #include "def/ReadBoardPositionVertical.h"
  }};

  static const BoardPositionTable read_board_position_left_diagonal{{
    #include "def/ReadBoardPositionLeftDiagonal.h"
  }};

  static const BoardPositionTable read_board_position_right_diagonal{{
    #include "def/ReadBoardPositionRightDiagonal.h"
  }};

  static const std::array<const BoardPositionTable *, kBoardDirectionNum> read_board_position{{
    &read_board_position_lateral, &read_board_position_vertical, &read_board_position_left_diagonal, &read_board_position_right_diagonal
  }};

  // 各方向のoffset
  static const std::array<BoardPosition, kBoardDirectionNum> direction_offset{{
    1, 257, 513, 769
  }};

  const BoardPositionTable &board_position_table = *(read_board_position[direction]);
  const BoardPosition board_position = board_position_table[move] + direction_offset[direction];

  return board_position;
}

inline const BoardPosition Board::GetWriteBoardPosition(const MovePosition move, const BoardDirection direction) const
{
  typedef std::array<std::uint8_t, kMoveNum> BoardPositionTable;

  // 指し手位置に対応する書込用BoardPosition Table
  static const BoardPositionTable write_board_position_lateral{{
    #include "def/WriteBoardPositionLateral.h"
  }};

  static const BoardPositionTable write_board_position_vertical{{
    #include "def/WriteBoardPositionVertical.h"
  }};
  
  static const BoardPositionTable write_board_position_left_diagonal{{
    #include "def/WriteBoardPositionLeftDiagonal.h"
  }};

  static const BoardPositionTable write_board_position_right_diagonal{{
    #include "def/WriteBoardPositionRightDiagonal.h"
  }};

  static const std::array<const BoardPositionTable *, kBoardDirectionNum> write_board_position{{
    &write_board_position_lateral, &write_board_position_vertical, &write_board_position_left_diagonal, &write_board_position_right_diagonal
  }};

  // 各方向のoffset
  static const std::array<BoardPosition, kBoardDirectionNum> direction_offset{{
    1, 257, 513, 769
  }};

  const BoardPositionTable &board_position_table = *(write_board_position[direction]);
  const BoardPosition board_position = board_position_table[move] + direction_offset[direction];

  return board_position;
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

inline const bool Board::IsUndefinedBoardPosition(const BoardPosition board_position) const
{
  bool is_undefined = (226 <= board_position && board_position <= 239);
  is_undefined |= (482 <= board_position && board_position <= 495);

  return is_undefined;
}

inline const bool Board::IsReadOnlyBoardPosition(const BoardPosition board_position) const
{
  bool is_read_only = board_position == 224 || board_position == 480;
  is_read_only |= board_position == 704 || board_position == 960;

  return is_read_only;
}

inline const bool Board::IsWriteOnlyBoardPosition(const BoardPosition board_position) const
{
  bool is_write_only = board_position == 225 || board_position == 481;
  is_write_only |= board_position == 736 || board_position == 992;

  return is_write_only;
}

constexpr bool IsInBoard(const Cordinate x, const Cordinate y)
{
  const bool x_in_board = 1 <= x && x <= kBoardLineNum;
  const bool y_in_board = 1 <= y && y <= kBoardLineNum;

  return x_in_board && y_in_board;
}

}   // namespace realcore

#endif    // BOARD_INL_H
