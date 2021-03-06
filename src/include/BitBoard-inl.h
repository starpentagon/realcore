#ifndef BIT_BOARD_INL_H
#define BIT_BOARD_INL_H

#include "MoveList.h"
#include "Conversion.h"
#include "LineNeighborhood.h"
#include "BoardOpenState.h"
#include "BitBoard.h"

namespace realcore{

inline const PositionState BitBoard::GetState(const MovePosition move) const
{
  // BoardPositionの横方向とMovePositionは同じ値になる
  // see /home/subaru/dev/realcore/doc/01_data_definition/board_definition.xlsx
  const BoardPosition board_position = static_cast<BoardPosition>(move);
  return GetState(board_position);
}

inline const PositionState BitBoard::GetState(const BoardPosition board_position) const
{
  const size_t index = GetBitBoardIndex(board_position);
  const size_t shift = GetBitBoardShift(board_position);

  constexpr StateBit state_mask = 0b11ULL;    // 下位2bit mask
  const StateBit state_bit = (bit_board_[index] >> shift) & state_mask;

  return static_cast<PositionState>(state_bit);
}

template<>
inline void BitBoard::SetState<kOverBoard>(const MovePosition move)
{
  assert(false);
}

template<>
inline void BitBoard::SetState<kBlackStone>(const MovePosition move)
{
  Cordinate x, y;
  GetMoveCordinate(move, &x, &y);

  if(!IsInBoard(x, y)){
    return;
  }

  std::array<size_t, kBoardDirectionNum> index_list;   // StateBit配列のindex
  GetBitBoardIndexList(x, y, &index_list);

  std::array<size_t, kBoardDirectionNum> shift_list;   // StateBit配列のshift量
  GetBitBoardShiftList(x, y, &shift_list);

  constexpr StateBit black_stone_xor_mask = 0b10ULL;    // kOpenPosition(0b11) XOR 0b10(mask) = 0b01(kBlackStone)

  for(size_t i=0; i<kBoardDirectionNum; i++){
    const size_t index = index_list[i];
    const size_t shift = shift_list[i];

    bit_board_[index] ^= (black_stone_xor_mask << shift);    
  }
}

template<>
inline void BitBoard::SetState<kWhiteStone>(const MovePosition move)
{
  Cordinate x, y;
  GetMoveCordinate(move, &x, &y);

  if(!IsInBoard(x, y)){
    return;
  }

  std::array<size_t, kBoardDirectionNum> index_list;   // StateBit配列のindex
  GetBitBoardIndexList(x, y, &index_list);

  std::array<size_t, kBoardDirectionNum> shift_list;   // StateBit配列のshift量
  GetBitBoardShiftList(x, y, &shift_list);
  
  constexpr StateBit white_stone_xor_mask = 0b01ULL;    // kOpenPosition(0b11) XOR 0b01(mask) = 0b10(kWhiteStone)

  for(size_t i=0; i<kBoardDirectionNum; i++){
    const size_t index = index_list[i];
    const size_t shift = shift_list[i];

    bit_board_[index] ^= (white_stone_xor_mask << shift);    
  }
}

template<>
inline void BitBoard::SetState<kOpenPosition>(const MovePosition move)
{
  Cordinate x, y;
  GetMoveCordinate(move, &x, &y);

  if(!IsInBoard(x, y)){
    return;
  }

  std::array<size_t, kBoardDirectionNum> index_list;   // StateBit配列のindex
  GetBitBoardIndexList(x, y, &index_list);

  std::array<size_t, kBoardDirectionNum> shift_list;   // StateBit配列のshift量
  GetBitBoardShiftList(x, y, &shift_list);
  
  constexpr StateBit open_position_or_mask = 0b11ULL;    // Any(0b**) OR 0b11(mask) = 0b11(kOpenPosition)

  for(size_t i=0; i<kBoardDirectionNum; i++){
    const size_t index = index_list[i];
    const size_t shift = shift_list[i];

    bit_board_[index] |= (open_position_or_mask << shift);
  }
}

inline void BitBoard::SetState(const MovePosition move, const PositionState state)
{
  switch(state){
  case kOverBoard:
    SetState<kOverBoard>(move);
    break;
  
  case kBlackStone:
    SetState<kBlackStone>(move);
    break;

  case kWhiteStone:
    SetState<kWhiteStone>(move);
    break;

  case kOpenPosition:
    SetState<kOpenPosition>(move);
    break;
  }
}

template<PositionState State>
void BitBoard::SetState(const MoveBitSet &move_bit_set)
{
  MoveList move_list;
  GetMoveList(move_bit_set, &move_list);

  for(const auto move : move_list){
    SetState<State>(move);
  }
}

template<size_t N>
void BitBoard::GetLineNeighborhoodStateBit(const MovePosition move, std::array<StateBit, kBoardDirectionNum> * const line_neighborhood_list) const
{
  static_assert(1 <= N && N <= 7, "N must be in [1, 7]");
  assert(line_neighborhood_list != nullptr);

  std::fill(line_neighborhood_list->begin(), line_neighborhood_list->end(), 0);

  Cordinate x = 0, y = 0;
  GetMoveCordinate(move, &x, &y);

  if(!IsInBoard(x, y)){
    return;
  }

  std::array<size_t, kBoardDirectionNum> index_list, shift_list;
  GetBitBoardIndexList(x, y, &index_list);
  GetBitBoardShiftList(x, y, &shift_list);

  constexpr size_t kBitNumber = 2 * (2 * N + 1);  // (2 * N + 1)地点（指定の地点 + 左右にN路ずつ） * 2bit/地点
  constexpr uint64_t kLineNeighborhoodMask = GetConsectiveBit<kBitNumber>();  // (2*N+1)地点の状態を取り出すマスク

  for(auto direction : GetBoardDirection()){
    const size_t index = index_list[direction];
    const size_t shift = shift_list[direction];

    constexpr size_t kMaskShift = 2 * N;    // moveを中心とした左右対称なマスクを生成するためN個の状態(2 * N bit)分を右シフト

    // moveを中心としたマスクを生成
    uint64_t neighborhood_mask = kLineNeighborhoodMask;

    if(shift >= kMaskShift){
      neighborhood_mask <<= shift - kMaskShift;
    }else{
      neighborhood_mask >>= kMaskShift - shift;
    }

    const StateBit neighborhood_state_bit = (bit_board_[index] & neighborhood_mask);

    // 14-15bit目がmove位置になるように揃える
    constexpr size_t kCenterAlignment = 14;

    if(shift >= kCenterAlignment){
      (*line_neighborhood_list)[direction] = neighborhood_state_bit >> (shift - kCenterAlignment);
    }else{
      (*line_neighborhood_list)[direction] = neighborhood_state_bit << (kCenterAlignment - shift);
    }
  }
}

template<PlayerTurn P>
const bool BitBoard::IsOpenFourMove(const MovePosition move) const
{
  if(!IsInBoardMove(move)){
    return false;
  }
  
  assert(GetState(move) == kOpenPosition);

  // 黒の達四(XOBBBBOX)には長さ5, 白の達四(OWWWWO)には長さ4の直線近傍を見れば良い
  constexpr size_t kFourCheck = (P == kBlackTurn) ? 5 : 4;
  LineNeighborhood line_neighbor(move, kFourCheck, *this);

  constexpr PositionState S = GetPlayerStone(P);
  line_neighbor.template SetCenterState<S>();

  return line_neighbor.template IsOpenFour<P>();
}

inline const bool BitBoard::IsOpenFourMove(const bool is_black_turn, const MovePosition move) const
{
  if(is_black_turn){
    return IsOpenFourMove<kBlackTurn>(move);
  }else{
    return IsOpenFourMove<kWhiteTurn>(move);
  }
}

template<PlayerTurn P>
const bool BitBoard::IsFourMove(const MovePosition move, MovePosition * const guard_move) const
{
  if(!IsInBoardMove(move)){
    return false;
  }
  
  assert(GetState(move) == kOpenPosition);

  // 黒の四(X[B4O1]X)には長さ5, 白の四([W4O1])には長さ4の直線近傍を見れば良い
  constexpr size_t kFourCheck = (P == kBlackTurn) ? 5 : 4;
  LineNeighborhood line_neighbor(move, kFourCheck, *this);

  constexpr PositionState S = GetPlayerStone(P);
  line_neighbor.template SetCenterState<S>();

  return line_neighbor.template IsFour<P>(guard_move);
}

template<PlayerTurn P>
const bool BitBoard::IsFourMoveOnBoard(const MovePosition move, MovePosition * const guard_move) const
{
  if(!IsInBoardMove(move)){
    return false;
  }
  
  assert(GetState(move) == GetPlayerStone(P));

  // 黒の四(X[B4O1]X)には長さ5, 白の四([W4O1])には長さ4の直線近傍を見れば良い
  constexpr size_t kFourCheck = (P == kBlackTurn) ? 5 : 4;
  LineNeighborhood line_neighbor(move, kFourCheck, *this);

  return line_neighbor.IsFour<P>(guard_move);
}

inline const bool BitBoard::IsFourMoveOnBoard(const bool is_black_turn, const MovePosition move, MovePosition * const guard_move) const
{
  if(is_black_turn){
    return IsFourMoveOnBoard<kBlackTurn>(move, guard_move);
  }else{
    return IsFourMoveOnBoard<kWhiteTurn>(move, guard_move);
  }
}

template<PlayerTurn P>
const bool BitBoard::IsDoubleFourMove(const MovePosition move, MoveBitSet * const influence_area) const
{
  if(!IsInBoardMove(move)){
    return false;
  }

  assert(GetState(move) == kOpenPosition);

  // 四々のチェックは長さ5の直線近傍を見れば良い
  static constexpr size_t kDoubleFourCheck = 5;
  LineNeighborhood line_neighbor(move, kDoubleFourCheck, *this);

  static constexpr PositionState S = GetPlayerStone(P);
  line_neighbor.SetCenterState<S>();

  return line_neighbor.IsDoubleFour<P>(influence_area);
}

template<PlayerTurn P>
inline const bool BitBoard::IsDoubleFourMove(const MovePosition move) const
{
  return IsDoubleFourMove<P>(move, nullptr);
}

template<PlayerTurn P>
const bool BitBoard::IsDoubleSemiThreeMove(const MovePosition move) const
{
  if(!IsInBoardMove(move)){
    return false;
  }

  assert(GetState(move) == kOpenPosition);

  // 見かけの三のチェックは長さ5の直線近傍を見れば良い
  static constexpr size_t kSemiThreeCheck = 5;
  LineNeighborhood line_neighbor(move, kSemiThreeCheck, *this);

  static constexpr PositionState S = GetPlayerStone(P);
  line_neighbor.SetCenterState<S>();

  return line_neighbor.IsDoubleSemiThreeMove<P>();

}

template<OpenStatePattern Pattern>
inline void BitBoard::AddOpenState(const size_t pattern_search_index, const BoardPosition pattern_position, BoardOpenState * const board_open_state) const
{
  assert(board_open_state != nullptr);
  board_open_state->AddOpenState<Pattern>(pattern_search_index, pattern_position);
}

template<OpenStatePattern Pattern>
void BitBoard::GetOpenState(const size_t index, const std::uint64_t combined_stone_bit, const std::uint64_t combined_open_bit, BoardOpenState * const board_open_state) const
{
  constexpr bool is_multiple_stone_pattern = (Pattern == kNextOverline) || (Pattern == kNextOpenFourBlack) || (Pattern == kNextOpenFourWhite) ||
    (Pattern == kNextFourBlack) || (Pattern == kNextFourWhite) || (Pattern == kNextSemiThreeBlack) || (Pattern == kNextSemiThreeWhite) || (Pattern == kNextPointOfSwordBlack) || (Pattern == kNextPointOfSwordWhite);

  if(is_multiple_stone_pattern && (combined_stone_bit == 0 || IsSingleBit(combined_stone_bit))){
    return;
  }

  constexpr size_t kPatternNum = GetOpenStatePatternNum(Pattern);
  std::array<std::uint64_t, kPatternNum> pattern_search{{0}};
  SearchOpenStatePattern<Pattern>(combined_stone_bit, combined_open_bit, &pattern_search);

  for(size_t pattern_index=0; pattern_index<kPatternNum; pattern_index++){
    const auto search_bit = pattern_search[pattern_index];

    if(search_bit == 0){
      continue;
    }
    
    std::vector<size_t> bit_index_list;
    GetBitIndexList(search_bit, &bit_index_list);

    for(const auto combined_shift : bit_index_list){
      const size_t bit_board_index = (combined_shift % 2 == 0) ? index : index + 1;
      const size_t bit_board_shift = (combined_shift % 2 == 0) ? combined_shift : combined_shift - 1;

      const BoardPosition pattern_position = GetBoardPosition(bit_board_index, bit_board_shift);
      AddOpenState<Pattern>(pattern_index, pattern_position, board_open_state);
    }
  }
}

inline void BitBoard::EnumerateForbiddenMoves(MoveBitSet * const forbidden_move_set) const
{
  BoardOpenState board_open_state;
  GetBoardOpenState(kUpdateForbiddenCheck, &board_open_state);
  
  EnumerateForbiddenMoves(board_open_state, forbidden_move_set);
}

template<PlayerTurn P>
void BitBoard::EnumerateDoubleFourMoves(const BoardOpenState &board_open_state, MoveBitSet * const double_four_move_set) const
{
  assert(double_four_move_set != nullptr);
  assert(double_four_move_set->none());
  
  constexpr auto kOpenFourPattern = (P == kBlackTurn) ? kNextOpenFourBlack : kNextOpenFourWhite;

  assert(board_open_state.GetUpdateOpenStateFlag().test(kOpenFourPattern));
  const auto& next_open_four_list = board_open_state.GetList(kOpenFourPattern);

  // 達四があると四ノビパターンが２回マッチされてしまう
  // ->達四の空点位置とパターン位置が一致する四はスキップしてカウントする。
  // ->達四の空点位置とパターン位置を記録しておく
  std::vector<int> open_four_key_list;
  open_four_key_list.reserve(next_open_four_list.size());

  for(const auto &open_state : next_open_four_list){
    const auto pattern_position = open_state.GetPatternPosition();
    const auto open_position = open_state.GetOpenPosition();
    const int open_four_key = (static_cast<int>(open_position) << 16) | static_cast<int>(pattern_position);
    open_four_key_list.emplace_back(open_four_key);
  }

  // 同一空点に五連を作る位置が２ヶ所以上あるかを調べる
  std::array<int, kMoveNum> next_five_position_table;
  next_five_position_table.fill(-1);

  constexpr auto kFourPattern = (P == kBlackTurn) ? kNextFourBlack : kNextFourWhite;
  assert(board_open_state.GetUpdateOpenStateFlag().test(kFourPattern));
  const auto& next_four_list = board_open_state.GetList(kFourPattern);

  for(const auto &open_state : next_four_list){
    const auto pattern_position = open_state.GetPatternPosition();
    const auto open_position = open_state.GetOpenPosition();
    const int open_four_key = (static_cast<int>(open_position) << 16) | static_cast<int>(pattern_position);

    // 達四と空点位置, パターン位置が一致する四はスキップする
    std::vector<int>::const_iterator find_it = find(open_four_key_list.begin(), open_four_key_list.end(), open_four_key);

    if(find_it != open_four_key_list.end()){
      // 達四にマッチした四パターンのためスキップする
      continue;
    }

    const auto next_five_position = open_state.GetCheckPosition();   // 防手位置 = 次に五連を作る位置
    const auto move = GetBoardMove(open_position);

    const bool is_double_four = next_five_position_table[move] >= 0 && next_five_position_table[move] != static_cast<int>(next_five_position);
    next_five_position_table[move] = next_five_position;

    if(is_double_four){
      double_four_move_set->set(move);
    }
  }
}

template<PlayerTurn P>
void BitBoard::EnumerateDoubleSemiThreeMoves(const BoardOpenState &board_open_state, MoveBitSet * const double_semi_three_move_set) const
{
  assert(double_semi_three_move_set != nullptr);
  assert(double_semi_three_move_set->none());

  constexpr auto kPattern = (P == kBlackTurn) ? kNextSemiThreeBlack : kNextSemiThreeWhite;
  assert(board_open_state.GetUpdateOpenStateFlag().test(kPattern));
  const auto& next_semi_three_list = board_open_state.GetList(kPattern);

  // 見かけの三々点
  // 2方向以上で見かけの三々になる指し手位置を求める
  std::array<std::bitset<kBoardDirectionNum>, kMoveNum> move_direction_semi_three;

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
      double_semi_three_move_set->set(move);
    }
  }
}

template<PlayerTurn P>
const bool BitBoard::GetOpenFourGuard(const BoardOpenState &board_open_state, MoveBitSet * const guard_move_set) const
{
  constexpr auto kPattern = (P == kBlackTurn) ? kNextOpenFourWhite : kNextOpenFourBlack;
  constexpr auto Q = GetOpponentTurn(P);

  assert(board_open_state.GetUpdateOpenStateFlag().test(kPattern));
  assert(guard_move_set != nullptr);
  assert(guard_move_set->none());
  
  const auto& open_state_list = board_open_state.GetList(kPattern);

  if(open_state_list.empty()){
    return false;
  }

  // 相手の達四点が存在する => 防手を生成する
  guard_move_set->flip();

  bool is_open_four = false;

  for(const auto &open_state : open_state_list){
    const auto open_position = open_state.GetOpenPosition();
    const auto move = GetBoardMove(open_position);

    MoveBitSet downward_influence_area, black_upward_influence_area, white_upward_influence_area;
    const auto is_forbidden = IsForbiddenMove<Q>(move, &downward_influence_area, &black_upward_influence_area, &white_upward_influence_area);

    if(is_forbidden){
      continue;
    }

    // XO[B3O1]OX or O[W3O1]Oの防手位置を生成する
    // O[W3O1]Oに対する黒番の防手 or XO[B3O1]OXに対する白番の防手のため長連筋の防手はない => 長連筋の空点チェックは不要
    MoveBitSet guard_bit;

    std::vector<BoardPosition> guard_position_list;
    open_state.GetInfluenceArea<P>(&guard_position_list);

    for(const auto guard_position : guard_position_list){
      const auto guard_move = GetBoardMove(guard_position);
      guard_bit.set(guard_move);
    }

    if(Q == kBlackTurn){
      // 否禁を禁手にする位置を防手位置に加える
      guard_bit |= white_upward_influence_area;
    }

    (*guard_move_set) &= guard_bit;
    is_open_four = true;
  }

  if(!is_open_four){
    guard_move_set->reset();
  }

  return is_open_four;  
}

inline void BitBoard::GetBoardStateBit(std::array<StateBit, 8> * const board_info) const
{
  assert(board_info != nullptr);
  
  for(size_t i=0; i<8; i++){
    (*board_info)[i] = bit_board_[i];
  }
}

template<PlayerTurn P>
void BitBoard::EnumerateMiseMoves(const BoardOpenState &board_open_state, MoveBitSet * const mise_move_set, MoveBitSet * const multi_mise_move_set) const
{
  assert(mise_move_set != nullptr);
  assert(mise_move_set->none());
  assert(multi_mise_move_set == nullptr || multi_mise_move_set->none());

  constexpr OpenStatePattern kTwoPattern = (P == kBlackTurn) ? kNextTwoBlack : kNextTwoWhite;
  constexpr OpenStatePattern kThreePattern = (P == kBlackTurn) ? kNextSemiThreeBlack : kNextSemiThreeWhite;
  constexpr OpenStatePattern kPointOfSwordPattern = (P == kBlackTurn) ? kNextPointOfSwordBlack : kNextPointOfSwordWhite;

  // 両ミセ(複数の方向にミセ手の焦点がある)を判定するためにミセ手位置から見たミセ手の焦点の方向を記録する
  std::array<int8_t, kMoveNum> mise_direction_table;

  if(multi_mise_move_set != nullptr){
    mise_direction_table.fill(-1);
  }

  // 二ノビ点の三を作る位置が四ノビ点になるケース
  MoveBitSet four_bit;
  EnumerateFourMoves<P>(board_open_state, &four_bit);

  assert(board_open_state.GetUpdateOpenStateFlag().test(kTwoPattern));
  const auto& two_open_state_list = board_open_state.GetList(kTwoPattern);

  for(const auto &open_state : two_open_state_list){
    std::array<BoardPosition, 2> semi_three_position_list{{0}};
    open_state.GetSemiThreePosition(&semi_three_position_list);

    for(const auto make_three_position : semi_three_position_list){
      const auto make_three_move = GetBoardMove(make_three_position);

      if(four_bit[make_three_move]){
        const auto open_position = open_state.GetOpenPosition();
        const auto mise_move = GetBoardMove(open_position);

        mise_move_set->set(mise_move);

        if(multi_mise_move_set != nullptr){
          const auto mise_direction = GetBoardDirection(open_position);
          const bool is_multi_direciton = mise_direction_table[mise_move] != -1 && mise_direction_table[mise_move] != mise_direction;

          if(is_multi_direciton){
            multi_mise_move_set->set(mise_move);
          }

          mise_direction_table[mise_move] = mise_direction;
        }
      }
    }
  }

  // 剣先点の四を作る位置が三ノビ点になるケース
  assert(board_open_state.GetUpdateOpenStateFlag().test(kThreePattern));
  const auto& three_open_state_list = board_open_state.GetList(kThreePattern);

  std::array<std::bitset<kBoardDirectionNum>, kMoveNum> make_three_flag;

  for(const auto &three_open_state : three_open_state_list){
    const auto three_position = three_open_state.GetOpenPosition();
    const auto three_move = GetBoardMove(three_position);
    const auto three_direction = GetBoardDirection(three_position);
    
    std::bitset<kBoardDirectionNum> make_three_bit(0b1111);
    make_three_bit.reset(three_direction);

    make_three_flag[three_move] |= make_three_bit;
  }

  assert(board_open_state.GetUpdateOpenStateFlag().test(kPointOfSwordPattern));
  const auto& sword_open_state_list = board_open_state.GetList(kPointOfSwordPattern);

  for(const auto &open_state : sword_open_state_list){
    std::array<BoardPosition, 2> four_position_list{{0}};
    open_state.GetFourPosition(&four_position_list);

    for(const auto make_four_position : four_position_list){
      const auto make_four_move = GetBoardMove(make_four_position);
      const auto make_four_direction = GetBoardDirection(make_four_position);
      const auto &make_mise_flag = make_three_flag[make_four_move];

      // 四ノビする方向と別方向に三ができるかチェックする
      if(!make_mise_flag[make_four_direction]){
        continue;
      }

      const auto open_position = open_state.GetOpenPosition();
      const auto mise_move = GetBoardMove(open_position);

      mise_move_set->set(mise_move);

      if(multi_mise_move_set != nullptr){
        const auto mise_direction = GetBoardDirection(open_position);
        const bool is_multi_direciton = mise_direction_table[mise_move] != -1 && mise_direction_table[mise_move] != mise_direction;

        if(is_multi_direciton){
          multi_mise_move_set->set(mise_move);
        }

        mise_direction_table[mise_move] = mise_direction;
      }
    }
  }
}

template<PlayerTurn P>
inline const bool BitBoard::IsFiveStones() const
{
  static constexpr size_t kFiveStones = 5;

  for(const auto state_bit : bit_board_){
    const auto stone_bit = GetPlayerStoneBit<P>(state_bit);
    const auto search_bit = GetConsectiveStoneBit<kFiveStones>(stone_bit);
    
    if(search_bit != 0){
      return true;
    }
  }

  return false;
}

template<PlayerTurn P>
inline const bool BitBoard::IsFiveStones(const MovePosition move) const
{
  static constexpr PositionState S = GetPlayerStone(P);
  LineNeighborhood line_neighbor(move, kOpenStateNeighborhoodSize, *this);
  line_neighbor.SetCenterState<S>();

  return line_neighbor.IsFive<P>();
}

template<PlayerTurn P>
inline const bool BitBoard::IsOverline() const
{
  static constexpr size_t kSixStones = 6;

  for(const auto state_bit : bit_board_){
    const auto stone_bit = GetPlayerStoneBit<P>(state_bit);
    const auto search_bit = GetConsectiveStoneBit<kSixStones>(stone_bit);
    
    if(search_bit != 0){
      return true;
    }
  }

  return false;
}

template<PlayerTurn P>
inline const bool BitBoard::IsOverline(const MovePosition move) const
{
  static constexpr PositionState S = GetPlayerStone(P);
  LineNeighborhood line_neighbor(move, kOpenStateNeighborhoodSize, *this);
  line_neighbor.SetCenterState<S>();

  return line_neighbor.IsOverline<P>();
}

}

#endif    // BIT_BOARD_INL_H
