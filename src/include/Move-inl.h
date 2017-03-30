#ifndef MOVE_INL_H
#define MOVE_INL_H

#include <cassert>
#include <array>

#include "Move.h"
#include "Board.h"

namespace realcore
{

inline void GetMoveCordinate(const MovePosition move, Cordinate * const x, Cordinate * const y)
{
  assert(x != nullptr && y != nullptr);

  *x = move % 16;
  *y = move / 16;
}

inline const std::array<MovePosition, kMoveNum>& GetAllMove()
{
  static const std::array<MovePosition, kMoveNum> all_move_list{
      {
        #include "def/MoveDefinition.h"
      }};

  return all_move_list;
}

inline const std::array<MovePosition, kValidMoveNum>& GetAllValidMove()
{
  static const std::array<MovePosition, kValidMoveNum> all_valid_move_list{
      {
        #include "def/InBoardMoveList.h"
        kNullMove
      }};

  return all_valid_move_list;
}

inline const std::array<MovePosition, kInBoardMoveNum>& GetAllInBoardMove()
{
  static const std::array<MovePosition, kInBoardMoveNum> all_in_board_move_list{
      {
        #include "def/InBoardMoveList.h"
      }};

  return all_in_board_move_list;
}

inline const bool IsInvalidMove(const MovePosition move)
{
  return move == kInvalidMove;
}

inline const bool IsNullMove(const MovePosition move)
{
  return move == kNullMove;
}

inline const bool IsInBoardMove(const MovePosition move)
{
  Cordinate x = 0, y = 0;
  GetMoveCordinate(move, &x, &y);

  return IsInBoard(x, y);
}

inline const bool IsValidMove(const MovePosition move)
{
  return IsInBoardMove(move) || IsNullMove(move);
}

inline const MovePosition GetMove(const Cordinate x, const Cordinate y)
{
  assert(IsInBoard(x, y));
  
  const MovePosition move = static_cast<MovePosition>(16 * y + x);
  return move;
}

inline const MovePosition GetSymmetricMove(const MovePosition move, const BoardSymmetry symmetry)
{
  if(!IsInBoardMove(move)){
    return move;
  }

  Cordinate move_x = 0, move_y = 0;
  GetMoveCordinate(move, &move_x, &move_y);

  constexpr int kCordinateCenter = 8;
  const int x = static_cast<int>(move_x) - kCordinateCenter;
  const int y = static_cast<int>(move_y) - kCordinateCenter;

  int symmetric_x = 0, symmetric_y = 0;
  
  switch(symmetry){
  case kIdenticalSymmetry:
    symmetric_x = x;
    symmetric_y = y;
    break;

  case kHorizontalSymmetry:
    symmetric_x = -x;
    symmetric_y = y;
    break;

  case kVerticalSymmetry:
    symmetric_x = x;
    symmetric_y = -y;
    break;

  case kCentricSymmetry:
    symmetric_x = -x;
    symmetric_y = -y;
    break;

  case kDiagonalSymmetry1:
    symmetric_x = y;
    symmetric_y = x;
    break;

  case kDiagonalSymmetry2:
    symmetric_x = -y;
    symmetric_y = x;
    break;

  case kDiagonalSymmetry3:
    symmetric_x = y;
    symmetric_y = -x;
    break;

  case kDiagonalSymmetry4:
    symmetric_x = -y;
    symmetric_y = -x;
    break;
  }

  return GetMove(symmetric_x + kCordinateCenter, symmetric_y + kCordinateCenter);
}

}   // realcore

#endif    // MOVE_INL_H