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
const bool BitBoard::IsForbiddenMove<kBlackTurn>(const MovePosition move, MoveBitSet * const downward_influence_area, MoveBitSet * const upward_influence_area) const
{
  if(!IsInBoardMove(move)){
    return false;
  }
  
  assert(GetState(move) == kOpenPosition);
  assert((downward_influence_area == nullptr && upward_influence_area == nullptr) || (downward_influence_area != nullptr && upward_influence_area != nullptr));

  // 禁手チェックはmoveの長さ5の直線近傍をチェックすれば十分
  // @see doc/06_forbidden_check/forbidden_check.pptx
  constexpr size_t kForbiddenCheck = 5;
  LineNeighborhood line_neighbor(move, kForbiddenCheck, *this);

  line_neighbor.SetCenterState<kBlackStone>();

  // 直線近傍の禁手チェック
  vector<BoardPosition> next_open_four_list;
  const ForbiddenCheckState forbidden_state = line_neighbor.ForbiddenCheck(&next_open_four_list, downward_influence_area, upward_influence_area);

  if(forbidden_state == kForbiddenMove){
    if(upward_influence_area != nullptr){
      upward_influence_area->reset();
    }

    return true;
  }else if(forbidden_state == kNonForbiddenMove){
    if(downward_influence_area != nullptr){
      downward_influence_area->reset();
    }

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
    bool is_forbidden = false;

    if(downward_influence_area == nullptr && upward_influence_area == nullptr){
      is_forbidden = board.IsForbiddenMove<kBlackTurn>(next_open_four_move, nullptr, nullptr);
    }else{
      MoveBitSet upward_three;    // 達四点が禁手成立 -> 不成立になると三が生じる
      MoveBitSet downward_three;  // 達四点が否禁 -> 禁手となると三が生じない
      is_forbidden = board.IsForbiddenMove<kBlackTurn>(next_open_four_move, &upward_three, &downward_three);

      if(is_forbidden){
        // 三になっていない
        *upward_influence_area |= upward_three;
      }else{
        // 三になっている
        *downward_influence_area |= downward_three;
      }
    }

    if(!is_forbidden){
      // 達四を作る位置が禁点でなければ三
      three_count++;

      if(three_count == 2){
        if(upward_influence_area != nullptr){
          upward_influence_area->reset();
        }

        return true;
      }

      checked_direction.set(direction);
    }
  }

  if(downward_influence_area != nullptr){
    downward_influence_area->reset();
  }

  return false;
}

template<>
const bool BitBoard::IsForbiddenMove<kBlackTurn>(const MovePosition move) const
{
  return IsForbiddenMove<kBlackTurn>(move, nullptr, nullptr);
}

template<>
const bool BitBoard::IsForbiddenMove<kWhiteTurn>(const MovePosition move, MoveBitSet * const downward_influence_area, MoveBitSet * const upward_influence_area) const
{
  // 白番に禁手はない
  return false;
}

template<>
const bool BitBoard::IsForbiddenMove<kWhiteTurn>(const MovePosition move) const
{
  return false;
}

void BitBoard::GetBoardOpenState(const UpdateOpenStateFlag &update_flag, BoardOpenState * const board_open_state) const
{
  assert(board_open_state->empty());

  for(size_t index=0; index<kBitBoardElementNum; index+=2)
  {
    const auto state_bit_even = bit_board_[index];
    const auto state_bit_odd = bit_board_[index+1];

    // 黒/白/空点ビットは２bit区切りの下位のみ設定されるため上位bitを使いbit board配列の2つをまとめてチェックする
    const auto black_bit_even = GetBlackStoneBit(state_bit_even);
    const auto black_bit_odd = GetBlackStoneBit(state_bit_odd);
    const auto combined_black_bit =  GetCombinedBit(black_bit_even, black_bit_odd);

    const auto white_bit_even = GetWhiteStoneBit(state_bit_even);
    const auto white_bit_odd = GetWhiteStoneBit(state_bit_odd);
    const auto combined_white_bit = GetCombinedBit(white_bit_even, white_bit_odd);
    
    const auto open_bit_even = GetOpenPositionBit(state_bit_even);
    const auto open_bit_odd = GetOpenPositionBit(state_bit_odd);
    const auto combined_open_bit = GetCombinedBit(open_bit_even, open_bit_odd);

    if(update_flag[kNextOverline]){
      GetOpenState<kNextOverline>(index, combined_black_bit, combined_open_bit, board_open_state);
    }

    if(update_flag[kNextOpenFourBlack]){
      GetOpenState<kNextOpenFourBlack>(index, combined_black_bit, combined_open_bit, board_open_state);
    }

    if(update_flag[kNextOpenFourWhite]){
      GetOpenState<kNextOpenFourWhite>(index, combined_white_bit, combined_open_bit, board_open_state);
    }

    if(update_flag[kNextFourBlack]){
      GetOpenState<kNextFourBlack>(index, combined_black_bit, combined_open_bit, board_open_state);
    }

    if(update_flag[kNextFourWhite]){
      GetOpenState<kNextFourWhite>(index, combined_white_bit, combined_open_bit, board_open_state);
    }

    if(update_flag[kNextSemiThreeBlack]){
      GetOpenState<kNextSemiThreeBlack>(index, combined_black_bit, combined_open_bit, board_open_state);
    }

    if(update_flag[kNextSemiThreeWhite]){
      GetOpenState<kNextSemiThreeWhite>(index, combined_white_bit, combined_open_bit, board_open_state);
    }

    if(update_flag[kNextPointOfSwordBlack]){
      GetOpenState<kNextPointOfSwordBlack>(index, combined_black_bit, combined_open_bit, board_open_state);
    }

    if(update_flag[kNextPointOfSwordWhite]){
      GetOpenState<kNextPointOfSwordWhite>(index, combined_white_bit, combined_open_bit, board_open_state);
    }
  }
}

void BitBoard::EnumerateForbiddenMoves(const BoardOpenState &board_open_state, MoveBitSet * const forbidden_move_set) const
{
  assert(board_open_state.GetUpdateOpenStateFlag().test(kNextOverline));
  assert(board_open_state.GetUpdateOpenStateFlag().test(kNextOpenFourBlack));
  assert(board_open_state.GetUpdateOpenStateFlag().test(kNextFourBlack));
  assert(board_open_state.GetUpdateOpenStateFlag().test(kNextSemiThreeBlack));
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
    MoveBitSet double_four_bit;
    EnumerateDoubleFourMoves<kBlackTurn>(board_open_state, &double_four_bit);
    
    (*forbidden_move_set) |= double_four_bit;
  }
  
  // 三々点
  {
    MoveBitSet double_semi_three_bit;
    EnumerateDoubleSemiThreeMoves<kBlackTurn>(board_open_state, &double_semi_three_bit);

    if(double_semi_three_bit.none()){
      return;
    }

    MoveList multi_semi_three_move_list;
    GetMoveList(double_semi_three_bit, &multi_semi_three_move_list);

    // 「見かけの三々」が「三々」かチェックする
    array<bitset<kBoardDirectionNum>, kMoveNum> move_direction_three;
    BitBoard check_bit_board(*this);

    assert(board_open_state.GetUpdateOpenStateFlag().test(kNextSemiThreeBlack));
    const auto& next_semi_three_list = board_open_state.GetList(kNextSemiThreeBlack);

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
}

template<>
void BitBoard::EnumerateOpenFourMoves<kBlackTurn>(const BoardOpenState &board_open_state, MoveBitSet * const open_four_move_set) const
{
  assert(board_open_state.GetUpdateOpenStateFlag().test(kNextOpenFourBlack));
  assert(open_four_move_set != nullptr);
  assert(open_four_move_set->none());

  const auto& next_open_four_list = board_open_state.GetList(kNextOpenFourBlack);

  for(const auto &open_state : next_open_four_list){
    const auto open_position = open_state.GetOpenPosition();
    const auto move = GetBoardMove(open_position);

    open_four_move_set->set(move);
  }
}

template<>
void BitBoard::EnumerateOpenFourMoves<kWhiteTurn>(const BoardOpenState &board_open_state, MoveBitSet * const open_four_move_set) const
{
  assert(board_open_state.GetUpdateOpenStateFlag().test(kNextOpenFourWhite));
  assert(open_four_move_set != nullptr);
  assert(open_four_move_set->none());

  const auto& next_open_four_list = board_open_state.GetList(kNextOpenFourWhite);

  for(const auto &open_state : next_open_four_list){
    const auto open_position = open_state.GetOpenPosition();
    const auto move = GetBoardMove(open_position);

    open_four_move_set->set(move);
  }
}

template<>
void BitBoard::EnumerateFourMoves<kBlackTurn>(const BoardOpenState &board_open_state, MoveBitSet * const four_move_set) const
{
  assert(board_open_state.GetUpdateOpenStateFlag().test(kNextFourBlack));
  assert(four_move_set != nullptr);
  assert(four_move_set->none());

  const auto& next_four_list = board_open_state.GetList(kNextFourBlack);

  for(const auto &open_state : next_four_list){
    const auto open_position = open_state.GetOpenPosition();
    const auto move = GetBoardMove(open_position);

    four_move_set->set(move);
  }
}

template<>
void BitBoard::EnumerateFourMoves<kBlackTurn>(const BoardOpenState &board_open_state, vector<MovePair> * const four_move_list) const
{
  assert(board_open_state.GetUpdateOpenStateFlag().test(kNextFourBlack));
  assert(four_move_list != nullptr);
  assert(four_move_list->empty());

  const auto& next_four_list = board_open_state.GetList(kNextFourBlack);
  four_move_list->reserve(next_four_list.size());

  // 四ノビ位置と防手位置が同じペアが生成されるのを防ぐため生成した位置を記録しておく
  vector<int> attack_guard_key_list;
  attack_guard_key_list.reserve(next_four_list.size());

  for(const auto &open_state : next_four_list){
    const auto open_position = open_state.GetOpenPosition();
    const auto move = GetBoardMove(open_position);

    const auto guard_position = open_state.GetCheckPosition();
    const auto guard_move = GetBoardMove(guard_position);

    const int attack_guard_key = (move << 16) | guard_move;
    const auto find_it = find(attack_guard_key_list.begin(), attack_guard_key_list.end(), attack_guard_key);

    if(find_it != attack_guard_key_list.end()){
      continue;
    }

    attack_guard_key_list.emplace_back(attack_guard_key);
    four_move_list->emplace_back(move, guard_move);
  }
}

template<>
void BitBoard::EnumerateFourMoves<kWhiteTurn>(const BoardOpenState &board_open_state, MoveBitSet * const four_move_set) const
{
  assert(board_open_state.GetUpdateOpenStateFlag().test(kNextFourWhite));
  assert(four_move_set != nullptr);
  assert(four_move_set->none());

  const auto& next_four_list = board_open_state.GetList(kNextFourWhite);

  for(const auto &open_state : next_four_list){
    const auto open_position = open_state.GetOpenPosition();
    const auto move = GetBoardMove(open_position);

    four_move_set->set(move);
  }
}

template<>
void BitBoard::EnumerateFourMoves<kWhiteTurn>(const BoardOpenState &board_open_state, vector<MovePair> * const four_move_list) const
{
  assert(board_open_state.GetUpdateOpenStateFlag().test(kNextFourWhite));
  assert(four_move_list != nullptr);
  assert(four_move_list->empty());

  const auto& next_four_list = board_open_state.GetList(kNextFourWhite);
  four_move_list->reserve(next_four_list.size());

  // 四ノビ位置と防手位置が同じペアが生成されるのを防ぐため生成した位置を記録しておく
  vector<int> attack_guard_key_list;
  attack_guard_key_list.reserve(next_four_list.size());

  for(const auto &open_state : next_four_list){
    const auto open_position = open_state.GetOpenPosition();
    const auto move = GetBoardMove(open_position);

    const auto guard_position = open_state.GetCheckPosition();
    const auto guard_move = GetBoardMove(guard_position);

    const int attack_guard_key = (move << 16) | guard_move;
    const auto find_it = find(attack_guard_key_list.begin(), attack_guard_key_list.end(), attack_guard_key);

    if(find_it != attack_guard_key_list.end()){
      continue;
    }

    attack_guard_key_list.emplace_back(attack_guard_key);
    four_move_list->emplace_back(move, guard_move);
  }
}

template<>
void BitBoard::EnumerateSemiThreeMoves<kBlackTurn>(const BoardOpenState &board_open_state, MoveBitSet * const semi_three_move_set) const
{
  constexpr auto kPattern = kNextSemiThreeBlack;
  assert(board_open_state.GetUpdateOpenStateFlag().test(kPattern));
  assert(semi_three_move_set != nullptr);
  assert(semi_three_move_set->none());

  const auto& open_state_list = board_open_state.GetList(kPattern);

  for(const auto &open_state : open_state_list){
    const auto open_position = open_state.GetOpenPosition();
    const auto move = GetBoardMove(open_position);

    semi_three_move_set->set(move);
  }
}

template<>
void BitBoard::EnumerateSemiThreeMoves<kWhiteTurn>(const BoardOpenState &board_open_state, MoveBitSet * const semi_three_move_set) const
{
  constexpr auto kPattern = kNextSemiThreeWhite;
  assert(board_open_state.GetUpdateOpenStateFlag().test(kPattern));
  assert(semi_three_move_set != nullptr);
  assert(semi_three_move_set->none());

  const auto& open_state_list = board_open_state.GetList(kPattern);

  for(const auto &open_state : open_state_list){
    const auto open_position = open_state.GetOpenPosition();
    const auto move = GetBoardMove(open_position);

    semi_three_move_set->set(move);
  }
}

template<>
void BitBoard::EnumeratePointOfSwordMoves<kBlackTurn>(const BoardOpenState &board_open_state, MoveBitSet * const point_of_sword_move_set) const
{
  constexpr auto kPattern = kNextPointOfSwordBlack;
  assert(board_open_state.GetUpdateOpenStateFlag().test(kPattern));
  assert(point_of_sword_move_set != nullptr);
  assert(point_of_sword_move_set->none());

  const auto& open_state_list = board_open_state.GetList(kPattern);

  for(const auto &open_state : open_state_list){
    const auto open_position = open_state.GetOpenPosition();
    const auto move = GetBoardMove(open_position);

    point_of_sword_move_set->set(move);
  }
}

template<>
void BitBoard::EnumeratePointOfSwordMoves<kWhiteTurn>(const BoardOpenState &board_open_state, MoveBitSet * const point_of_sword_move_set) const
{
  constexpr auto kPattern = kNextPointOfSwordWhite;
  assert(board_open_state.GetUpdateOpenStateFlag().test(kPattern));
  assert(point_of_sword_move_set != nullptr);
  assert(point_of_sword_move_set->none());

  const auto& open_state_list = board_open_state.GetList(kPattern);

  for(const auto &open_state : open_state_list){
    const auto open_position = open_state.GetOpenPosition();
    const auto move = GetBoardMove(open_position);

    point_of_sword_move_set->set(move);
  }
}

template<>
const bool BitBoard::GetTerminateGuard<kBlackTurn>(const BoardOpenState &board_open_state, MoveBitSet * const guard_move_set) const
{
  MoveBitSet total_guard_move_set;
  total_guard_move_set.flip();

  // 達四
  MoveBitSet open_four_guard;
  const bool is_open_four = GetOpenFourGuard<kBlackTurn>(board_open_state, &open_four_guard);

  if(is_open_four){
    total_guard_move_set &= open_four_guard;
  }

  // 四々
  MoveBitSet double_four_guard;
  const bool is_double_four = GetDoubleFourGuard(board_open_state, &double_four_guard);

  if(is_double_four){
    total_guard_move_set &= double_four_guard;
  }

  // 極め手
  MoveBitSet make_forbidden_guard;
  const bool is_make_forbidden = GetMakeForbiddenGuard(board_open_state, &make_forbidden_guard);

  if(is_make_forbidden){
    total_guard_move_set &= make_forbidden_guard;
  }

  if(!is_open_four && !is_double_four && !is_make_forbidden){
    return false;
  }

  // 四ノビ防手を生成する
  MoveBitSet four_bit;
  EnumerateFourMoves<kBlackTurn>(board_open_state, &four_bit);

  total_guard_move_set |= four_bit;
  (*guard_move_set) = total_guard_move_set;

  return true;
}

template<>
const bool BitBoard::GetTerminateGuard<kWhiteTurn>(const BoardOpenState &board_open_state, MoveBitSet * const guard_move_set) const
{
  if(!GetOpenFourGuard<kWhiteTurn>(board_open_state, guard_move_set)){
    return false;
  }

  // 四ノビ防手を生成する
  MoveBitSet four_bit;
  EnumerateFourMoves<kWhiteTurn>(board_open_state, &four_bit);

  (*guard_move_set) |= four_bit;

  return true;
}

const bool BitBoard::GetDoubleFourGuard(const BoardOpenState &board_open_state, MoveBitSet * const guard_move_set) const
{
  MoveBitSet double_four_bit;
  EnumerateDoubleFourMoves<kWhiteTurn>(board_open_state, &double_four_bit);
  
  if(double_four_bit.none()){
    return false;
  }

  MoveList double_four_list;
  GetMoveList(double_four_bit, &double_four_list);
  
  assert(guard_move_set != nullptr);
  assert(guard_move_set->none());
  
  guard_move_set->flip();

  for(const auto move : double_four_list){
    MoveBitSet influence_area;
    IsDoubleFourMove<kWhiteTurn>(move, &influence_area);

    *guard_move_set &= influence_area;
  }

  return true;
}

const bool BitBoard::GetMakeForbiddenGuard(const BoardOpenState &board_open_state, MoveBitSet * const guard_move_set) const
{
  vector<MovePair> four_pair_list;
  EnumerateFourMoves<kWhiteTurn>(board_open_state, &four_pair_list);

  if(four_pair_list.empty()){
    return false;
  }

  BitBoard check_bit_board(*this);

  assert(guard_move_set != nullptr);
  assert(guard_move_set->none());
  
  guard_move_set->flip();
  bool is_make_forbidden = false;

  for(const auto &four_pair : four_pair_list){
    const auto four_move = four_pair.first;
    const auto four_guard_move = four_pair.second;

    check_bit_board.SetState<kWhiteStone>(four_move);

    MoveBitSet downward_influence_area, upward_influence_area;
    const bool is_forbidden = check_bit_board.IsForbiddenMove<kBlackTurn>(four_guard_move, &downward_influence_area, &upward_influence_area);

    if(is_forbidden){
      is_make_forbidden = true;

      downward_influence_area.set(four_move);
      *guard_move_set &= downward_influence_area;
    }

    check_bit_board.SetState<kOpenPosition>(four_move);
  }

  if(!is_make_forbidden){
    guard_move_set->reset();
  }

  return is_make_forbidden;
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
