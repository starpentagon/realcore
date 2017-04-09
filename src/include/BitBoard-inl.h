#ifndef BIT_BOARD_INL_H
#define BIT_BOARD_INL_H

#include "Move.h"
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

template<PlayerTurn P>
const bool BitBoard::IsDoubleFourMove(const MovePosition move) const
{
  if(!IsInBoardMove(move)){
    return false;
  }

  assert(GetState(move) == kOpenPosition);

  // 四々のチェックは長さ5の直線近傍を見れば良い
  constexpr size_t kDoubleFourCheck = 5;
  LineNeighborhood line_neighbor(move, kDoubleFourCheck, *this);

  constexpr PositionState S = GetPlayerStone(P);
  line_neighbor.SetCenterState<S>();

  return line_neighbor.IsDoubleFour<P>();
}

template<PlayerTurn P>
void BitBoard::GetOpenStateOpenFour(const size_t index, const std::uint64_t stone_bit, const std::uint64_t open_bit, BoardOpenState * const board_open_state) const
{
  assert(board_open_state != nullptr);

  if(stone_bit == 0 || IsSingleBit(stone_bit)){
    return;
  }

  constexpr size_t kPatternNum = kFourStonePattern;
  std::array<std::uint64_t, kPatternNum> pattern_search{{0}};
  SearchNextOpenFour<P>(stone_bit, open_bit, &pattern_search);

  for(size_t pattern_index=0; pattern_index<kPatternNum; pattern_index++){
    const auto search_bit = pattern_search[pattern_index];

    if(search_bit == 0){
      continue;
    }
    
    std::vector<size_t> bit_index_list;
    GetBitIndexList(search_bit, &bit_index_list);

    for(const auto shift : bit_index_list){
      const BoardPosition pattern_position = GetBoardPosition(index, shift);
      const BoardPosition open_position = GetOpenBoardPosition(pattern_position, pattern_index);
      board_open_state->AddNextOpenFour<P>(open_position, pattern_position);
    }
  }
}

template<PlayerTurn P>
void BitBoard::GetOpenStateFour(const size_t index, const std::uint64_t stone_bit, const std::uint64_t open_bit, BoardOpenState * const board_open_state) const
{
  assert(board_open_state != nullptr);

  if(stone_bit == 0 || IsSingleBit(stone_bit)){
    return;
  }
  
  constexpr size_t kPatternNum = kTwoOfFivePattern;
  std::array<std::uint64_t, kPatternNum> pattern_search{{0}};
  SearchNextFour<P>(stone_bit, open_bit, &pattern_search);

  for(size_t pattern_index=0; pattern_index<kPatternNum; pattern_index++){
    const auto search_bit = pattern_search[pattern_index];

    if(search_bit == 0){
      continue;
    }
    
    std::vector<size_t> bit_index_list;
    GetBitIndexList(search_bit, &bit_index_list);

    for(const auto shift : bit_index_list){
      const BoardPosition pattern_position = GetBoardPosition(index, shift);

      {
        const size_t open_index = GetLessIndexOfTwo(pattern_index);
        const size_t guard_index = GetGreaterIndexOfTwo(pattern_index);

        const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
        const BoardPosition guard_position = GetOpenBoardPosition(pattern_position, guard_index);

        board_open_state->AddNextFour<P>(open_position, pattern_position, guard_position);
      }
      {
        const size_t open_index = GetGreaterIndexOfTwo(pattern_index);
        const size_t guard_index = GetLessIndexOfTwo(pattern_index);

        const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
        const BoardPosition guard_position = GetOpenBoardPosition(pattern_position, guard_index);

        board_open_state->AddNextFour<P>(open_position, pattern_position, guard_position);
      }
    }
  }
}

template<PlayerTurn P>
void BitBoard::GetOpenStateSemiThree(const size_t index, const std::uint64_t stone_bit, const std::uint64_t open_bit, BoardOpenState * const board_open_state) const
{
  assert(board_open_state != nullptr);

  if(stone_bit == 0 || IsSingleBit(stone_bit)){
    return;
  }
  
  constexpr size_t kPatternNum = kTwoOfFourPattern;
  std::array<std::uint64_t, kPatternNum> pattern_search{{0}};
  SearchNextSemiThree<P>(stone_bit, open_bit, &pattern_search);

  for(size_t pattern_index=0; pattern_index<kPatternNum; pattern_index++){
    const auto search_bit = pattern_search[pattern_index];

    if(search_bit == 0){
      continue;
    }

    std::vector<size_t> bit_index_list;
    GetBitIndexList(search_bit, &bit_index_list);

    for(const auto shift : bit_index_list){
      const BoardPosition pattern_position = GetBoardPosition(index, shift);
      const BoardPosition left_side_guard_position = pattern_position + 4;  // O[B2O2]Oの左端のO
      const BoardPosition right_side_guard_position = pattern_position - 1;  // O[B2O2]Oの右端のO

      {
        const size_t open_index = GetLessIndexOfTwo(pattern_index);
        const size_t check_index = GetGreaterIndexOfTwo(pattern_index);

        const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
        const BoardPosition check_position = GetOpenBoardPosition(pattern_position, check_index);
        const GuardPositionList guard_position_list{{check_position, right_side_guard_position, left_side_guard_position}};

        board_open_state->AddNextSemiThree<P>(open_position, pattern_position, check_position, guard_position_list);
      }
      {
        const size_t open_index = GetGreaterIndexOfTwo(pattern_index);
        const size_t check_index = GetLessIndexOfTwo(pattern_index);

        const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
        const BoardPosition check_position = GetOpenBoardPosition(pattern_position, check_index);
        const GuardPositionList guard_position_list{{check_position, right_side_guard_position, left_side_guard_position}};

        board_open_state->AddNextSemiThree<P>(open_position, pattern_position, check_position, guard_position_list);
      }
    }
  }
}

inline void BitBoard::EnumerateForbiddenMoves(MoveBitSet * const forbidden_move_set) const
{
  BoardOpenState board_open_state;
  GetBoardOpenState(&board_open_state);
  
  EnumerateForbiddenMoves(board_open_state, forbidden_move_set);
}

}

#endif    // BIT_BOARD_INL_H
