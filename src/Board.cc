#include <sstream>
#include <bitset>

#include "Move.h"
#include "MoveList.h"
#include "LineNeighborhood.h"
#include "Board.h"

using namespace std;

namespace realcore
{

Board::Board()
{
}

Board::Board(const Board &board)
{
  *this = board;
}

Board::Board(const MoveList &move_list)
{
  for(const auto move : move_list){
    MakeMove(move);
  }
}

bool IsEqual(const Board &board_1, const Board &board_2)
{
  if(board_1.bit_board_ != board_2.bit_board_){
    return false;
  }

  if(board_1.move_list_ != board_2.move_list_){
    return false;
  }

  return true;
}

const bool Board::operator==(const Board &board) const{
  return IsEqual(*this, board);
}

const bool Board::operator!=(const Board &board) const{
  return !(*this == board);
}

void Copy(const Board &board_from, Board * const board_to)
{
  assert(board_to != nullptr);

  board_to->bit_board_ = board_from.bit_board_;
  board_to->move_list_ = board_from.move_list_;
}

const Board& Board::operator=(const Board &board)
{
  if(this != &board){
    Copy(board, this);
  }

  return *this;
}
}   // namespace realcore
