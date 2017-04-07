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

void Board::EnumerateForbiddenMoves(MoveBitSet * const forbidden_move_set) const
{
  const bool is_black_turn = move_list_.IsBlackTurn();

  if(!is_black_turn){
    // 白番は禁手がないため抜ける
    return;
  }

  assert(forbidden_move_set != nullptr);
  assert(forbidden_move_set->none());

  const auto& board_open_state = board_open_state_stack_.top();

  {
    // 長連点
    const auto& next_overline_list = board_open_state.GetNextOverline();
    
    for(const auto &open_state : next_overline_list){
      const auto open_position = open_state.GetOpenPosition();
      const auto move = GetBoardMove(open_position);

      forbidden_move_set->set(move);
    }
  }
  {
    // 四々点
    const auto& next_open_four_list = board_open_state.GetNextOpenFourBlack();

    // 達四があると四ノビパターンが２回マッチされてしまう
    // ->達四の空点位置とパターン位置が一致する四はスキップしてカウントする。
    // ->達四の空点位置とパターン位置を記録しておく
    vector<int> open_four_key_list;
    open_four_key_list.reserve(next_open_four_list.size());

    for(const auto &open_state : next_open_four_list){
      // XO[B3O1]OXの一番の右のOの位置を五連を作る位置の一つとして記録する
      const auto pattern_position = open_state.GetPatternPosition();
      const auto open_position = open_state.GetOpenPosition();
      const int open_four_key = (static_cast<int>(open_position) << 16) | static_cast<int>(pattern_position);
      open_four_key_list.emplace_back(open_four_key);
    }

    // 同一空点に五連を作る位置が２ヶ所以上あるかを調べる
    array<int, kMoveNum> next_five_position_table;
    next_five_position_table.fill(-1);

    const auto& next_four_list = board_open_state.GetNextFourBlack();

    for(const auto &open_state : next_four_list){
      const auto pattern_position = open_state.GetPatternPosition();
      const auto open_position = open_state.GetOpenPosition();
      const int open_four_key = (static_cast<int>(open_position) << 16) | static_cast<int>(pattern_position);

      // 達四と空点位置, パターン位置が一致する四はスキップする
      vector<int>::const_iterator find_it = find(open_four_key_list.begin(), open_four_key_list.end(), open_four_key);

      if(find_it != open_four_key_list.end()){
        // 達四にマッチした四パターンのためスキップする
        continue;
      }

      const auto next_five_position = open_state.GetGuardPositionList()[0];   // 防手位置 = 次に五連を作る位置
      const auto move = GetBoardMove(open_position);

      const bool is_double_four = next_five_position_table[move] >= 0 && next_five_position_table[move] != static_cast<int>(next_five_position);
      next_five_position_table[move] = next_five_position;

      if(is_double_four){
        forbidden_move_set->set(move);
      }
    }
  }
  
  // 三々点
  const auto& next_semi_three_list = board_open_state.GetNextSemiThreeBlack();
  vector<MovePosition> multi_semi_three_move_list;
  multi_semi_three_move_list.reserve(next_semi_three_list.size() / 2);    // 三々点１つに三ノビ点は２つはあるため三々点は三ノビ点の半数以下
    
  {
    // 見かけの三々点
    // 2方向以上で見かけの三々になる指し手位置を求める
    array<bitset<kBoardDirectionNum>, kMoveNum> move_direction_semi_three;

    for(const auto &open_state : next_semi_three_list){
      const auto open_position = open_state.GetOpenPosition();
      const auto move = GetBoardMove(open_position);
      const auto direction = GetBoardDirection(open_position);

      move_direction_semi_three[move].set(direction);
    }

    for(const auto &open_state : next_semi_three_list){
      const auto open_position = open_state.GetOpenPosition();
      const auto move = GetBoardMove(open_position);

      if(move_direction_semi_three[move].count() >= 2){
        multi_semi_three_move_list.emplace_back(move);
        move_direction_semi_three[move].reset();
      }
    }
  }

  if(multi_semi_three_move_list.empty()){
    return;
  }

  // 「見かけの三々」が「三々」かチェックする
  array<bitset<kBoardDirectionNum>, kMoveNum> move_direction_three;
  BitBoard check_bit_board(bit_board_);

  for(const auto &open_state : next_semi_three_list){
    const auto open_position = open_state.GetOpenPosition();
    const auto move = GetBoardMove(open_position);

    vector<MovePosition>::const_iterator find_it = find(multi_semi_three_move_list.begin(), multi_semi_three_move_list.end(), move);
    
    if(find_it == multi_semi_three_move_list.end()){
      // 「見かけの三々」点ではない
      continue;
    }
    
    const auto direction = GetBoardDirection(open_position);
    
    if(move_direction_three[move].test(direction)){
      // チェック済の方向
      continue;
    }

    check_bit_board.SetState<kBlackStone>(move);

    for(const auto check_position : open_state.GetCheckPositionList()){
      const auto check_move = GetBoardMove(check_position);
      const bool is_forbidden = check_bit_board.IsForbiddenMove<kBlackTurn>(check_move);

      if(is_forbidden){
        continue;
      }
      
      move_direction_three[move].set(direction);

      if(move_direction_three[move].count() >= 2){
        forbidden_move_set->set(move);
      }

      break;
    }

    check_bit_board.SetState<kOpenPosition>(move);
  }
}

}   // namespace realcore
