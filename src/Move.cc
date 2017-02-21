#include <cassert>

#include "Move.h"

using namespace std;

namespace realcore
{

const array<MovePosition, kMoveNum> &GetAllMove()
{
  static array<MovePosition, kMoveNum> all_move_list{
      {
        #include "MoveDefinition.h"
      }};

  return all_move_list;
}

const array<MovePosition, kValidMoveNum> &GetAllValidMove()
{
  static array<MovePosition, kValidMoveNum> all_valid_move_list{
      {
        #include "InBoardMoveList.h"
        kNullMove
      }};

  return all_valid_move_list;
}

const array<MovePosition, kInBoardMoveNum> &GetAllInBoardMove()
{
  static array<MovePosition, kInBoardMoveNum> all_in_board_move_list{
      {
        #include "InBoardMoveList.h"
      }};

  return all_in_board_move_list;
}

const bool IsInvalidMove(const MovePosition move)
{
  return move == kInvalidMove;
}

const bool IsNullMove(const MovePosition move)
{
  return move == kNullMove;
}

const bool IsInBoardMove(const MovePosition move)
{
  Cordinate x = 0, y = 0;
  GetMoveCordinate(move, &x, &y);

  bool is_in_board = 1 <= x && x <= kBoardLineNum;
  is_in_board &= 1 <= y && y <= kBoardLineNum;

  return is_in_board;
}

const bool IsValidMove(const MovePosition move)
{
  return IsInBoardMove(move) || IsNullMove(move);
}

void GetMoveCordinate(const MovePosition move, Cordinate *x, Cordinate *y)
{
  assert(x != nullptr && y != nullptr);

  *x = move % 16;
  *y = move / 16;
}

}   // namespace realcore
