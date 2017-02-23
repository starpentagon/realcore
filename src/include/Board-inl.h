#ifndef BOARD_INL_H
#define BOARD_INL_H

#include <array>
#include <cassert>

#include "Move.h"
#include "Board.h"

namespace realcore
{

inline PositionState Board::GetState(const MovePosition move) const
{
  return kOpenPosition;
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
  return index;
}

inline const size_t Board::GetBitBoardShift(const BoardPosition board_position) const
{
  const size_t shift_val = 2 * (board_position % 32);
  return shift_val;
}

constexpr bool IsInBoard(const Cordinate x, const Cordinate y)
{
  const bool x_in_board = 1 <= x && x <= kBoardLineNum;
  const bool y_in_board = 1 <= y && y <= kBoardLineNum;

  return x_in_board && y_in_board;
}

}   // namespace realcore

#endif    // BOARD_INL_H
