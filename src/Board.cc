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
  assert(!IsTerminateMove(is_black_turn, move));
  
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

const bool Board::IsOpponentFour(MovePosition * const guard_move) const
{
  if(move_list_.empty()){
    return false;
  }

  const auto last_move = move_list_.GetLastMove();
  const bool is_opponent_black = !move_list_.IsBlackTurn();

  return bit_board_.IsFourMoveOnBoard(is_opponent_black, last_move, guard_move);
}

template<>
const bool Board::TerminateCheck<kBlackTurn>(MovePosition * const terminating_move) const
{
  assert(terminating_move != nullptr);

  MovePosition guard_move;
  const bool is_opponent_four = IsOpponentFour(&guard_move);

  if(is_opponent_four){
    const bool is_terminate_move = IsTerminateMove<kBlackTurn>(guard_move);

    // 四ノビ防手が禁手の場合は1手前で終端するのでここでは禁手チェックはしない
    *terminating_move = guard_move;
    return is_terminate_move;
  }

  // 禁手ではない達四がないかチェックする
  const auto& board_open_state = board_open_state_list_.back();

  MoveBitSet open_four_bit;
  bit_board_.EnumerateOpenFourMoves<kBlackTurn>(board_open_state, &open_four_bit);

  if(open_four_bit.none()){
    return false;
  }

  // 達四点が存在する場合は禁手かどうかチェックする
  MoveList open_four_move_list;
  GetMoveList(open_four_bit, &open_four_move_list);

  for(const auto move : open_four_move_list){
    if(!bit_board_.IsForbiddenMove<kBlackTurn>(move)){
      *terminating_move = move;
      return true;
    }
  }

  return false;
}

template<>
const bool Board::TerminateCheck<kWhiteTurn>(MovePosition * const terminating_move) const
{
  assert(terminating_move != nullptr);

  MovePosition guard_move;
  const bool is_opponent_four = IsOpponentFour(&guard_move);

  if(is_opponent_four){
    const bool is_terminate_move = IsTerminateMove<kWhiteTurn>(guard_move);

    // 四ノビ防手が禁手の場合は1手前で終端するのでここでは禁手チェックはしない
    *terminating_move = guard_move;
    return is_terminate_move;
  }

  // 四ノビする手を列挙し、終端手かどうかチェックする
  const auto& board_open_state = board_open_state_list_.back();

  vector<MovePair> four_list;
  bit_board_.EnumerateFourMoves<kWhiteTurn>(board_open_state, &four_list);

  for(const auto &four_pair : four_list){
    if(IsTerminateMove<kWhiteTurn>(four_pair)){
      *terminating_move = four_pair.first;
      return true;
    }
  }

  return false;
}

const bool IsNormalNonTerminateSequence(const MoveList &move_list){
  Board board;
  bool is_black_turn = true;

  for(const auto move : move_list){
    if(!board.IsNormalMove(is_black_turn, move)){
      return false;
    }

    const bool is_terminate = board.IsTerminateMove(is_black_turn, move);

    if(is_terminate){
      return false;
    }

    board.MakeMove(move);
    is_black_turn = !is_black_turn;
  }

  return true;
}

}   // namespace realcore
