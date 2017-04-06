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
  board_open_state_stack_.emplace();
}

Board::Board(const Board &board)
{
  *this = board;
}

Board::Board(const MoveList &move_list)
{
  board_open_state_stack_.emplace();

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

  if(board_1.board_open_state_stack_ != board_2.board_open_state_stack_){
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
  board_to->board_open_state_stack_ = board_from.board_open_state_stack_;
}

const Board& Board::operator=(const Board &board)
{
  if(this != &board){
    Copy(board, this);
  }

  return *this;
}

void Board::MakeMove(const MovePosition move)
{
  const bool is_black_turn = move_list_.IsBlackTurn();

  if(is_black_turn){
    bit_board_.SetState<kBlackStone>(move);
  }else{
    bit_board_.SetState<kWhiteStone>(move);
  }

  move_list_ += move;

  const auto &current_board_open_state = board_open_state_stack_.top();
  board_open_state_stack_.emplace(current_board_open_state);
  board_open_state_stack_.top().Update(is_black_turn, move, bit_board_);
}

void Board::UndoMove()
{
  assert(!move_list_.empty());

  const auto move = move_list_.GetLastMove();
  bit_board_.SetState<kOpenPosition>(move);
  --move_list_;

  board_open_state_stack_.pop();
}

}   // namespace realcore
