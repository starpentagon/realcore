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

const bool IsNonTerminateNormalSequence(const MoveList &move_list){
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

const bool MakeNonTerminateNormalSequence(const MoveList &original_move_list, MoveList * const modified_move_list)
{
  assert(modified_move_list != nullptr);
  bool is_black_turn = true;
  MoveBitSet black_remain, white_remain;
  MoveList black_move_list, white_move_list;

  for(const auto move : original_move_list){
    if(is_black_turn){
      black_remain.set(move);
      black_move_list += move;
    }else{
      white_remain.set(move);
      white_move_list += move;
    }

    is_black_turn = !is_black_turn;
  }

  // 黒白同数 or 黒石 = 白石 + 1であること
  bool is_balanced = black_remain.count() == white_remain.count();
  is_balanced |= black_remain.count() == white_remain.count() + 1;

  if(!is_balanced){
    return false;
  }

  // 長連筋の両端にあたる黒石は優先的に打ち、長連筋間の白石は優先度を下げるため、その位置を求める
  MoveBitSet on_overline_black, on_overline_white;

  for(const auto move_from : black_move_list){
    if(on_overline_black[move_from]){
      continue;
    }

    for(const auto move_to : black_move_list){
      // 長連筋かチェック
      const auto board_distance = CalcBoardDistance(move_from, move_to);

      if(board_distance != 5){
        continue;
      }

      int black_stone_count = 0;

      for(const auto black_move : black_move_list){
        const auto distance_move_from = CalcBoardDistance(black_move, move_from);
        const auto distance_move_to = CalcBoardDistance(black_move, move_to);

        const bool is_in_overline = (distance_move_from <= 4) && (distance_move_to <= 4);
        black_stone_count += is_in_overline;
      }

      if(black_stone_count != 3){
        continue;
      }

      // move_from, move_toは長連筋の両端
      on_overline_black.set(move_from);
      on_overline_black.set(move_to);

      // move_from <-> move_to間の白石は優先度を下げる
      for(const auto white_move : white_move_list){
        const auto distance_move_from = CalcBoardDistance(white_move, move_from);
        const auto distance_move_to = CalcBoardDistance(white_move, move_to);

        const bool is_in_overline = (distance_move_from <= 4) && (distance_move_to <= 4);

        if(!is_in_overline){
          continue;
        }

        on_overline_white.set(white_move);
      }
    }
  }

  set<string> transposition_set;
  const bool is_modified = MakeNonTerminateNormalSequence(black_remain, white_remain, on_overline_black, on_overline_white, &transposition_set, modified_move_list);

  return is_modified;
}

const bool MakeNonTerminateNormalSequence(const MoveBitSet &black_remain, const MoveBitSet &white_remain, const MoveBitSet &on_overline_black, const MoveBitSet &on_overline_white, set<string> * const transposition_set, MoveList * const modified_move_list)
{
  assert(modified_move_list != nullptr);
  assert(transposition_set != nullptr);

  if(black_remain.none() && white_remain.none()){
    return true;
  }

  const auto remain_bit = black_remain | white_remain;
  
  if(transposition_set->find(remain_bit.to_string()) != transposition_set->end()){
    return false;
  }

  Board board(*modified_move_list);
  const bool is_black_turn = modified_move_list->IsBlackTurn();
  MoveBitSet remain_position = is_black_turn ? black_remain : white_remain;

  MoveList candidate_move_list;

  MovePosition four_guard_move;
  const bool is_opponent_four = board.IsOpponentFour(&four_guard_move);

  if(is_opponent_four){
    if(!remain_position[four_guard_move]){
      // 四ノビ防手がもともとの指し手リストに含まれていないため修正できない
      return false;
    }

    candidate_move_list = four_guard_move;
  }else{
    GetMoveList(remain_position, &candidate_move_list);
  }

  // 直近手(初期局面の場合は天元)に近い手を優先的に調べる
  const MovePosition last_move = modified_move_list->empty() ? kMoveHH : modified_move_list->GetLastMove();
  SortByNearMove(last_move, &candidate_move_list);

  // 四ノビの発生を抑制するために長連筋にある石の優先度を変更する
  const MoveBitSet &on_overline = is_black_turn ? on_overline_black : on_overline_white;

  MoveBitSet four_move_bit;
  board.EnumerateFourMoves(is_black_turn, &four_move_bit);

  vector<MoveValue> move_priority_list;
  move_priority_list.reserve(candidate_move_list.size());

  for(size_t i=0, size=candidate_move_list.size(); i<size; i++){
    const auto move = candidate_move_list[i];
    int64_t move_priority = i;

    if(move == kMoveHH){
      // 天元は最優先
      move_priority -= kInBoardMoveNum * kInBoardMoveNum;
    }

    if(on_overline[move]){
      if(is_black_turn){
        // 長連筋の両端を優先的に着手し、四ノビの発生を抑える
        move_priority -= kInBoardMoveNum;
      }else{
        // 長連筋に割り込む手の優先度を下げ、四ノビの発生を抑える
        move_priority += kInBoardMoveNum;
      }
    }

    if(four_move_bit[move]){
      // 四ノビの優先度を最大限下げる
      move_priority += kInBoardMoveNum * kInBoardMoveNum;
    }

    move_priority_list.emplace_back(move, move_priority);
  }

  AscendingSort(&move_priority_list);
  candidate_move_list.clear();

  for(const auto move_value : move_priority_list){
    candidate_move_list += move_value.first;
  }

  for(const auto move : candidate_move_list)
  {
    if(!board.IsNormalMove(is_black_turn, move)){
      continue;
    }

    if(board.IsTerminateMove(is_black_turn, move)){
      continue;
    }

    MoveBitSet child_black_remain = black_remain, child_white_remain = white_remain;

    if(is_black_turn){
      child_black_remain.reset(move);
    }else{
      child_white_remain.reset(move);
    }

    *modified_move_list += move;

    const auto is_modified = MakeNonTerminateNormalSequence(child_black_remain, child_white_remain, on_overline_black, on_overline_white, transposition_set, modified_move_list);

    if(is_modified){
      return true;
    }

    --(*modified_move_list);
  }

  cout << modified_move_list->str() << endl;
  transposition_set->insert(remain_bit.to_string());
  return false;
}

}   // namespace realcore
