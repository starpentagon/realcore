#include <sstream>
#include <bitset>

#include "MoveList.h"
#include "LineNeighborhood.h"
#include "BoardOpenState.h"
#include "BitBoard.h"

using namespace std;

namespace realcore
{

BitBoard::BitBoard()
: bit_board_{{
  #include "def/BitBoardDefinition.h"
  }}
{
}

BitBoard::BitBoard(const BitBoard &bit_board)
{
  *this = bit_board;
}

BitBoard::BitBoard(const MoveList &move_list)
: bit_board_{{
  #include "def/BitBoardDefinition.h"
  }}
{
  bool is_black_turn = true;

  for(const auto move : move_list){
    if(is_black_turn){
      SetState<kBlackStone>(move);
    }else{
      SetState<kWhiteStone>(move);
    }

    is_black_turn = !is_black_turn;
  }
}

bool IsEqual(const BitBoard &bit_board_1, const BitBoard &bit_board_2)
{
  if(bit_board_1.bit_board_ != bit_board_2.bit_board_){
    return false;
  }

  return true;
}

const bool BitBoard::operator==(const BitBoard &bit_board) const{
  return IsEqual(*this, bit_board);
}

const bool BitBoard::operator!=(const BitBoard &bit_board) const{
  return !(*this == bit_board);
}

void Copy(const BitBoard &bit_board_from, BitBoard * const bit_board_to)
{
  assert(bit_board_to != nullptr);

  bit_board_to->bit_board_ = bit_board_from.bit_board_;
}

const BitBoard& BitBoard::operator=(const BitBoard &bit_board)
{
  if(this != &bit_board){
    Copy(bit_board, this);
  }

  return *this;
}

template<>
const bool BitBoard::IsForbiddenMove<kBlackTurn>(const MovePosition move) const
{
  if(!IsInBoardMove(move)){
    return false;
  }
  
  assert(GetState(move) == kOpenPosition);

  // 禁手チェックはmoveの長さ5の直線近傍をチェックすれば十分
  // @see doc/06_forbidden_check/forbidden_check.pptx
  constexpr size_t kForbiddenCheck = 5;
  LineNeighborhood line_neighbor(move, kForbiddenCheck, *this);

  line_neighbor.SetCenterState<kBlackStone>();

  // 直線近傍の禁手チェック
  vector<BoardPosition> next_open_four_list;
  const ForbiddenCheckState forbidden_state = line_neighbor.ForbiddenCheck(&next_open_four_list);

  if(forbidden_state == kForbiddenMove){
    return true;
  }else if(forbidden_state == kNonForbiddenMove){
    return false;
  }

  // 見かけの三々が存在する(kPossibleForbiddenMove)
  // 達四を作る位置が禁手かどうかを再帰的にチェックする
  BitBoard board(*this);
  board.SetState<kBlackStone>(move);

  size_t three_count = 0;
  bitset<kBoardDirectionNum> checked_direction;
  
  for(const auto board_position : next_open_four_list){
    const auto direction = GetBoardDirection(board_position);

    if(checked_direction[direction]){
      continue;
    }

    const MovePosition next_open_four_move = GetBoardMove(board_position);
    const bool is_forbidden = board.IsForbiddenMove<kBlackTurn>(next_open_four_move);

    if(!is_forbidden){
      // 達四を作る位置が禁点でなければ三
      three_count++;

      if(three_count == 2){
        return true;
      }

      checked_direction.set(direction);
    }
  }

  return false;
}

template<>
const bool BitBoard::IsForbiddenMove<kWhiteTurn>(const MovePosition move) const
{
  // 白番に禁手はない
  return false;
}

void BitBoard::GetBoardOpenState(BoardOpenState * const board_open_state) const
{
  assert(board_open_state->empty());

  for(size_t index=0; index<kBitBoardElementNum; index++)
  {
    const auto state_bit = bit_board_[index];
    const auto black_bit = GetBlackStoneBit(state_bit);
    const auto white_bit = GetWhiteStoneBit(state_bit);
    const auto open_bit = GetOpenPositionBit(state_bit);

    GetOpenStateOverline(index, black_bit, open_bit, board_open_state);

    GetOpenStateOpenFour<kBlackTurn>(index, black_bit, open_bit, board_open_state);
    GetOpenStateOpenFour<kWhiteTurn>(index, white_bit, open_bit, board_open_state);

    GetOpenStateFour<kBlackTurn>(index, black_bit, open_bit, board_open_state);
    GetOpenStateFour<kWhiteTurn>(index, white_bit, open_bit, board_open_state);

    GetOpenStateSemiThree<kBlackTurn>(index, black_bit, open_bit, board_open_state);
    GetOpenStateSemiThree<kWhiteTurn>(index, white_bit, open_bit, board_open_state);
  }
}

void BitBoard::GetOpenStateOverline(const size_t index, const std::uint64_t stone_bit, const std::uint64_t open_bit, BoardOpenState * const board_open_state) const
{
  assert(board_open_state != nullptr);

  if(stone_bit == 0 || IsSingleBit(stone_bit)){
    return;
  }
  
  std::array<std::uint64_t, kFourStonePattern> overline_search{{0}};
  SearchNextOverline(stone_bit, open_bit, &overline_search);

  for(size_t pattern_index=0; pattern_index<kFourStonePattern; pattern_index++){
    const auto search_bit = overline_search[pattern_index];
    std::vector<size_t> bit_index_list;
    
    GetBitIndexList(search_bit, &bit_index_list);

    for(const auto shift : bit_index_list){
      const BoardPosition pattern_position = GetBoardPosition(index, shift);
      const BoardPosition open_position = GetOpenBoardPosition(pattern_position, pattern_index);
      board_open_state->AddNextOverline(open_position, pattern_position);
    }
  }
}

void BitBoard::EnumerateForbiddenMoves(const BoardOpenState &board_open_state, MoveBitSet * const forbidden_move_set) const
{
  assert(forbidden_move_set != nullptr);
  assert(forbidden_move_set->none());

  {
    // 長連点
    const auto& next_overline_list = board_open_state.GetList(kNextOverline);
    
    for(const auto &open_state : next_overline_list){
      const auto open_position = open_state.GetOpenPosition();
      const auto move = GetBoardMove(open_position);

      forbidden_move_set->set(move);
    }
  }
  {
    // 四々点
    const auto& next_open_four_list = board_open_state.GetList(kNextOpenFourBlack);

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

    const auto& next_four_list = board_open_state.GetList(kNextFourBlack);

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
  const auto& next_semi_three_list = board_open_state.GetList(kNextSemiThreeBlack);
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
  BitBoard check_bit_board(*this);

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

    const auto check_position = open_state.GetCheckPosition();
    const auto check_move = GetBoardMove(check_position);

    check_bit_board.SetState<kBlackStone>(move);
    const bool is_forbidden = check_bit_board.IsForbiddenMove<kBlackTurn>(check_move);
    check_bit_board.SetState<kOpenPosition>(move);

    if(is_forbidden){
      continue;
    }
    
    move_direction_three[move].set(direction);

    if(move_direction_three[move].count() >= 2){
      forbidden_move_set->set(move);
    }
  }
}

const string BitBoard::str() const
{
  static array<string, kBoardLineNum + 1> cordinate_str{{
    "", "A ", "B ", "C ", "D ", "E ", "F ", "G ", "H ", "I ", "J ", "K ", "L ", "M ", "N ", "O "
  }};

  stringstream ss;

  // x座標
  ss << "  ";
  for(Cordinate x=1; x<=static_cast<Cordinate>(kBoardLineNum); ++x){
    ss << cordinate_str[x];
  }

  ss << endl;

  for(Cordinate y=1; y<=static_cast<Cordinate>(kBoardLineNum); ++y){
    ss << cordinate_str[y];

    for(Cordinate x=1; x<=static_cast<Cordinate>(kBoardLineNum); ++x){      
      const MovePosition move = GetMove(x, y);
      const PositionState state = GetState(move);

      string state_str = ". ";

      if(state == kBlackStone){
        state_str = "x ";
      }else if(state == kWhiteStone){
        state_str = "o ";
      }else{
        // 盤端
        bool is_corner = (x == 1 && y == 1) || (x == kBoardLineNum && y == 1);
        is_corner |= (x == 1 && y == kBoardLineNum) || (x == kBoardLineNum && y == kBoardLineNum);

        bool is_star = (x == 4 && y == 4) || (x == 8 && y == 8) || (x == 12 && y == 4);
        is_star |= (x == 4 && y == 12) || (x == 12 && y == 12);

        if(is_corner){
          state_str = "+ ";
        }else if(x == 1 || x == kBoardLineNum){
          state_str = "| ";
        }else if(y == 1 || y == kBoardLineNum){
          state_str = "--";
        }else if(is_star){
          state_str = "* ";
        }
      }

      ss << state_str;
    }

    ss << cordinate_str[y];
    ss << endl;
  }

  // x座標
  ss << "  ";
  for(Cordinate x=1; x<=static_cast<Cordinate>(kBoardLineNum); ++x){
    ss << cordinate_str[x];
  }

  ss << endl;
  return ss.str();
}

}   // namespace realcore
