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
  board_open_state_list_.reserve(kMoveNum);
  board_open_state_list_.emplace_back();
}

Board::Board(const UpdateOpenStateFlag &update_flag)
{
  board_open_state_list_.reserve(kMoveNum);
  board_open_state_list_.emplace_back(update_flag);
}

Board::Board(const Board &board)
{
  *this = board;
}

Board::Board(const MoveList &move_list)
{
  board_open_state_list_.emplace_back();

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

  if(board_1.board_open_state_list_ != board_2.board_open_state_list_){
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
  board_to->board_open_state_list_ = board_from.board_open_state_list_;
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
  assert(IsNormalMove(is_black_turn, move));
  
  if(is_black_turn){
    bit_board_.SetState<kBlackStone>(move);
  }else{
    bit_board_.SetState<kWhiteStone>(move);
  }

  move_list_ += move;

  const auto &current_board_open_state = board_open_state_list_.back();
  board_open_state_list_.emplace_back(current_board_open_state, is_black_turn, move, bit_board_);
}

void Board::UndoMove()
{
  assert(!move_list_.empty());

  const auto move = move_list_.GetLastMove();
  bit_board_.SetState<kOpenPosition>(move);
  --move_list_;

  board_open_state_list_.pop_back();
}

void Board::EnumerateForbiddenMoves(MoveBitSet * const forbidden_move_set) const
{
  const bool is_black_turn = move_list_.IsBlackTurn();

  if(!is_black_turn){
    // 白番は禁手がないため抜ける
    return;
  }

  const auto& board_open_state = board_open_state_list_.back();
  bit_board_.EnumerateForbiddenMoves(board_open_state, forbidden_move_set);
}

const bool IsNormalSequence(const MoveList &move_list){
  Board board;
  bool is_black_turn = true;

  for(size_t i=0, size=move_list.size(); i<size; i++){
    const auto move = move_list[i];

    if(!board.IsNormalMove(is_black_turn, move)){
      return false;
    }

    const bool is_terminate = board.IsTerminateMove(is_black_turn, move);

    if(is_terminate){
      if(i + 1 == size){
        return true;
      }else{
        return false;
      }
    }

    board.MakeMove(move);
    is_black_turn = !is_black_turn;
  }

  return true;
}

}   // namespace realcore
