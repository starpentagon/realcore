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

Board::Board(const MoveList &move_list, const UpdateOpenStateFlag &update_flag)
{
  board_open_state_list_.reserve(kMoveNum);
  board_open_state_list_.emplace_back(update_flag);

  for(const auto move : move_list){
    MakeMove(move);
  }
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

  if(board_1.board_move_sequence_ != board_2.board_move_sequence_){
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
  board_to->board_move_sequence_ = board_from.board_move_sequence_;
  board_to->board_open_state_list_ = board_from.board_open_state_list_;
}

const Board& Board::operator=(const Board &board)
{
  if(this != &board){
    Copy(board, this);
  }

  return *this;
}

void Board::MakeMove(const MovePosition move, const UpdateOpenStateFlag &update_flag)
{
  const bool is_black_turn = board_move_sequence_.IsBlackTurn();
  assert(IsNormalMove(move));
  assert(!IsTerminateMove(move));
  
  if(is_black_turn){
    bit_board_.SetState<kBlackStone>(move);
  }else{
    bit_board_.SetState<kWhiteStone>(move);
  }

  board_move_sequence_ += move;

  const auto &current_board_open_state = board_open_state_list_.back();
  board_open_state_list_.emplace_back(current_board_open_state, is_black_turn, move, bit_board_, update_flag);
}

void Board::MakeMove(const MovePosition move)
{
  const auto &current_board_open_state = board_open_state_list_.back();
  MakeMove(move, current_board_open_state.GetUpdateOpenStateFlag());
}

void Board::UndoMove()
{
  assert(!board_move_sequence_.empty());

  const auto move = board_move_sequence_.GetLastMove();
  bit_board_.SetState<kOpenPosition>(move);
  --board_move_sequence_;

  board_open_state_list_.pop_back();
}

const bool Board::IsNormalMove(const MovePosition move) const
{
  // Pass以外の盤外の手は正規手ではない
  if(!IsInBoardMove(move) && move != kNullMove){
    return false;
  }

  // 相手に四があるかチェック
  MovePosition guard_move;
  const auto is_opponent_four = IsOpponentFour(&guard_move);

  if(is_opponent_four){
    // 相手に四がある
    return guard_move == move;
  }

  // 相手に四がない場合、Passは正規手
  if(move == kNullMove){
    return true;
  }

  if(bit_board_.GetState(move) != kOpenPosition){
    return false;
  }

  const bool is_black_turn = board_move_sequence_.IsBlackTurn();

  if(is_black_turn && bit_board_.IsForbiddenMove<kBlackTurn>(move)){
    return false;
  }

  return true;
}

const bool Board::IsTerminateMove(const MovePosition move) const
{
  assert(IsNormalMove(move));

  const bool is_black_turn = board_move_sequence_.IsBlackTurn();

  if(bit_board_.IsOpenFourMove(is_black_turn, move)){
    // 達四が作れる
    return true;
  }

  if(!is_black_turn){
    // 四々が作れるかチェックする
    if(bit_board_.IsDoubleFourMove<kWhiteTurn>(move)){
      return true;
    }

    // 禁手に極められるかチェックする
    MovePosition guard_move;
    const bool is_four = bit_board_.IsFourMove<kWhiteTurn>(move, &guard_move);

    if(is_four){
      BitBoard bit_board(bit_board_);
      bit_board.SetState<kWhiteStone>(move);

      if(bit_board.IsForbiddenMove<kBlackTurn>(guard_move)){
        return true;
      }
    }
  }

  return false;
}

const bool Board::IsTerminateMove(const MovePair &four_pair) const
{
  const auto four_move = four_pair.first;
  const bool is_black_turn = board_move_sequence_.IsBlackTurn();

  assert(IsNormalMove(four_move));

  if(bit_board_.IsOpenFourMove(is_black_turn, four_move)){
    // 達四が作れる
    return true;
  }

  if(!is_black_turn){
    // 四々がつくれる
    if(bit_board_.IsDoubleFourMove<kWhiteTurn>(four_move)){
      return true;
    }

    BitBoard bit_board(bit_board_);
    bit_board.SetState<kWhiteStone>(four_move);

    const auto four_guard = four_pair.second;
  
    if(bit_board.IsForbiddenMove<kBlackTurn>(four_guard)){
      return true;
    }
  }

  return false;
}

void Board::EnumerateForbiddenMoves(MoveBitSet * const forbidden_move_set) const
{
  const bool is_black_turn = board_move_sequence_.IsBlackTurn();

  if(!is_black_turn){
    // 白番は禁手がないため抜ける
    return;
  }

  const auto& board_open_state = board_open_state_list_.back();
  bit_board_.EnumerateForbiddenMoves(board_open_state, forbidden_move_set);
}

const bool Board::IsOpponentFour(MovePosition * const guard_move) const
{
  if(board_move_sequence_.empty()){
    return false;
  }

  const auto last_move = board_move_sequence_.GetLastMove();
  const bool is_opponent_black = !board_move_sequence_.IsBlackTurn();

  return bit_board_.IsFourMoveOnBoard(is_opponent_black, last_move, guard_move);
}

const bool Board::GetTerminateGuard(MoveBitSet * const guard_move_set) const
{
  assert(!IsOpponentFour(nullptr));
  const auto &board_open_state = board_open_state_list_.back();
  const bool is_black_turn = board_move_sequence_.IsBlackTurn();

  if(is_black_turn){
    return bit_board_.GetTerminateGuard<kBlackTurn>(board_open_state, guard_move_set);
  }else{
    return bit_board_.GetTerminateGuard<kWhiteTurn>(board_open_state, guard_move_set);
  }
}

const bool Board::TerminateCheck(MovePosition * const terminating_move) const
{
  if(board_move_sequence_.IsBlackTurn()){
    return TerminateCheckBlack(terminating_move);
  }else{
    return TerminateCheckWhite(terminating_move);
  }
}

const bool Board::TerminateCheckBlack(MovePosition * const terminating_move) const
{
  assert(terminating_move != nullptr);

  MovePosition guard_move;
  const bool is_opponent_four = IsOpponentFour(&guard_move);

  if(is_opponent_four){
    const bool is_terminate_move = IsTerminateMove(guard_move);

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

const bool Board::TerminateCheckWhite(MovePosition * const terminating_move) const
{
  assert(terminating_move != nullptr);

  MovePosition guard_move;
  const bool is_opponent_four = IsOpponentFour(&guard_move);

  if(is_opponent_four){
    const bool is_terminate_move = IsTerminateMove(guard_move);

    // 四ノビ防手が禁手の場合は1手前で終端するのでここでは禁手チェックはしない
    *terminating_move = guard_move;
    return is_terminate_move;
  }

  // 四ノビする手を列挙し、終端手かどうかチェックする
  const auto& board_open_state = board_open_state_list_.back();

  vector<MovePair> four_list;
  bit_board_.EnumerateFourMoves<kWhiteTurn>(board_open_state, &four_list);

  for(const auto &four_pair : four_list){
    if(IsTerminateMove(four_pair)){
      *terminating_move = four_pair.first;
      return true;
    }
  }

  return false;
}

bool Board::IsBoardSymmetric(const BoardSymmetry symmetry) const
{
  array<StateBit, 8> original_bit_array;
  bit_board_.GetBoardStateBit(&original_bit_array);

  MoveList symmetric_move_list;
  GetSymmetricMoveList(board_move_sequence_, symmetry, &symmetric_move_list);
  BitBoard symmetric_board(symmetric_move_list);

  array<StateBit, 8> symmetric_bit_array;
  symmetric_board.GetBoardStateBit(&symmetric_bit_array);

  return original_bit_array == symmetric_bit_array;
}

const bool IsNonTerminateNormalSequence(const MoveList &move_list){
  Board board;
  bool is_black_turn = true;

  for(const auto move : move_list){
    if(!board.IsNormalMove(move)){
      return false;
    }

    const bool is_terminate = board.IsTerminateMove(move);

    if(is_terminate){
      return false;
    }

    board.MakeMove(move);
    is_black_turn = !is_black_turn;
  }

  return true;
}

const bool MakeNonTerminateNormalSequence(const MoveList &original_move_list, MoveList * const modified_move_list)
{
  assert(modified_move_list != nullptr);
  bool is_black_turn = true;
  MoveBitSet black_remain, white_remain;

  for(const auto move : original_move_list){
    if(is_black_turn){
      black_remain.set(move);
    }else{
      white_remain.set(move);
    }

    is_black_turn = !is_black_turn;
  }

  // 黒白同数 or 黒石 = 白石 + 1であること
  bool is_balanced = black_remain.count() == white_remain.count();
  is_balanced |= black_remain.count() == white_remain.count() + 1;

  if(!is_balanced){
    return false;
  }

  map<string, bool> result_map;
  unsigned int call_limit = 500;    // VLM問題集での正規化に要した呼び出し回数は最大138回
  const bool is_modified = MakeNonTerminateNormalSequence(black_remain, white_remain, &result_map, modified_move_list, &call_limit);

  return is_modified;
}

const bool MakeNonTerminateNormalSequence(const MoveBitSet &black_remain, const MoveBitSet &white_remain, map<string, bool> * const result_map, MoveList * const modified_move_list, unsigned int * const call_limit)
{
  assert(result_map != nullptr);
  assert(modified_move_list != nullptr);
  assert(call_limit != nullptr);

  // state-keyを作成する
  MoveList black_remain_list, white_remain_list;

  GetMoveList(black_remain, &black_remain_list);
  GetMoveList(white_remain, &white_remain_list);

  string state_key = black_remain_list.str() + "_" + white_remain_list.str();
  const auto find_it = result_map->find(state_key);

  if(find_it != result_map->end()){
    return find_it->second;
  }

  if(black_remain.none() && white_remain.none()){
    result_map->insert(pair<string, bool>(state_key, true));
    return true;
  }
  
  if(*call_limit == 0){
    return false;
  }

  --(*call_limit);

  Board board(*modified_move_list);
  const bool is_black_turn = modified_move_list->IsBlackTurn();
  MoveBitSet remain_position = is_black_turn ? black_remain : white_remain;

  MoveList candidate_move_list;

  MovePosition four_guard_move;
  const bool is_opponent_four = board.IsOpponentFour(&four_guard_move);

  if(is_opponent_four){
    if(!remain_position[four_guard_move]){
      // 四ノビ防手がもともとの指し手リストに含まれていないため修正できない
      result_map->insert(pair<string, bool>(state_key, false));
      return false;
    }

    candidate_move_list = four_guard_move;
  }else{
    GetMoveList(remain_position, &candidate_move_list);
  }

  // 直近手(初期局面の場合は天元)に近い手を優先的に調べる
  const MovePosition last_move = modified_move_list->empty() ? kMoveHH : modified_move_list->GetLastMove();
  SortByNearMove(last_move, &candidate_move_list);

  for(const auto move : candidate_move_list)
  {
    if(!board.IsNormalMove(move)){
      continue;
    }

    if(board.IsTerminateMove(move)){
      continue;
    }

    MoveBitSet child_black_remain = black_remain, child_white_remain = white_remain;

    if(is_black_turn){
      child_black_remain.reset(move);
    }else{
      child_white_remain.reset(move);
    }

    *modified_move_list += move;

    const auto is_modified = MakeNonTerminateNormalSequence(child_black_remain, child_white_remain, result_map, modified_move_list, call_limit);

    if(is_modified){
      result_map->insert(pair<string, bool>(state_key, true));
      return true;
    }

    --(*modified_move_list);
  }

  result_map->insert(pair<string, bool>(state_key, false));
  return false;
}

}   // namespace realcore
