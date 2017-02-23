#include <cassert>

#include "Move.h"
#include "Board.h"

using namespace std;

namespace realcore
{

const array<MovePosition, kMoveNum>& GetAllMove()
{
  static const array<MovePosition, kMoveNum> all_move_list{
      {
        #include "MoveDefinition.h"
      }};

  return all_move_list;
}

const array<MovePosition, kValidMoveNum>& GetAllValidMove()
{
  static const array<MovePosition, kValidMoveNum> all_valid_move_list{
      {
        #include "InBoardMoveList.h"
        kNullMove
      }};

  return all_valid_move_list;
}

const array<MovePosition, kInBoardMoveNum>& GetAllInBoardMove()
{
  static const array<MovePosition, kInBoardMoveNum> all_in_board_move_list{
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

  return IsInBoard(x, y);
}

const bool IsValidMove(const MovePosition move)
{
  return IsInBoardMove(move) || IsNullMove(move);
}

void GetMoveCordinate(const MovePosition move, Cordinate * const x, Cordinate * const y)
{
  assert(x != nullptr && y != nullptr);

  *x = move % 16;
  *y = move / 16;
}

const MovePosition GetMove(const Cordinate x, const Cordinate y)
{
  assert(IsInBoard(x, y));
  
  const MovePosition move = static_cast<MovePosition>(16 * y + x);
  return move;
}

}   // namespace realcore
