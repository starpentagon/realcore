#ifndef BIT_BOARD_INL_H
#define BIT_BOARD_INL_H

#include "Move.h"
#include "BitBoard.h"
#include "LineNeighborhood.h"

namespace realcore{

inline const size_t GetBitBoardIndex(const BoardPosition board_position)
{
  const size_t index = board_position / 32;
  assert(index < kBitBoardElementNum);

  return index;
}

template<BoardDirection kDirection>
inline const size_t GetBitBoardIndex(const Cordinate x, const Cordinate y)
{
  assert(IsInBoard(x, y));

  switch(kDirection)
  {
  case kLateralDirection:
    return y / 2;
  case kVerticalDirection:
    return x / 2 + 8;
  case kLeftDiagonalDirection:
    return ((x + y - 2) % 16) / 2 + 16;
  case kRightDiagonalDirection: 
    return ((y - x + 14) % 16) / 2 + 24;
  default:
    assert(false);
    return 0;
  }
}

inline const size_t GetBitBoardIndex(const Cordinate x, const Cordinate y, const BoardDirection direction)
{
  switch(direction)
  {
  case kLateralDirection:
    return GetBitBoardIndex<kLateralDirection>(x, y);
  case kVerticalDirection:
    return GetBitBoardIndex<kVerticalDirection>(x, y);
  case kLeftDiagonalDirection:
    return GetBitBoardIndex<kLeftDiagonalDirection>(x, y);
  case kRightDiagonalDirection: 
    return GetBitBoardIndex<kRightDiagonalDirection>(x, y);
  default:
    assert(false);
    return 0;
  }
}

inline const size_t GetBitBoardShift(const BoardPosition board_position)
{
  const size_t shift_val = 2 * (board_position % 32);
  return shift_val;
}

template<BoardDirection kDirection>
inline const size_t GetBitBoardShift(const Cordinate x, const Cordinate y)
{
  assert(IsInBoard(x, y));

  switch(kDirection)
  {
  case kLateralDirection:
    return 2 * x + 32 * (y % 2);
  case kVerticalDirection:
    return 2 * y + 32 * (x % 2);
  case kLeftDiagonalDirection:
  case kRightDiagonalDirection: 
    return 2 * (y - 1) + 32 * ((x + y) % 2);
  default:
    assert(false);
    return 0;
  }
}

inline const size_t GetBitBoardShift(const Cordinate x, const Cordinate y, const BoardDirection direction)
{
  switch(direction)
  {
  case kLateralDirection:
    return GetBitBoardShift<kLateralDirection>(x, y);
  case kVerticalDirection:
    return GetBitBoardShift<kVerticalDirection>(x, y);
  case kLeftDiagonalDirection:
    return GetBitBoardShift<kLeftDiagonalDirection>(x, y);
  case kRightDiagonalDirection: 
    return GetBitBoardShift<kRightDiagonalDirection>(x, y);
  default:
    assert(false);
    return 0;
  }
}

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

inline void GetBitBoardIndexList(const Cordinate x, const Cordinate y, std::array<size_t, kBoardDirectionNum> * const index_list)
{
  for(const auto direction : GetBoardDirection()){
    (*index_list)[direction] = GetBitBoardIndex(x, y, direction);
  }
}

inline void GetBitBoardShiftList(const Cordinate x, const Cordinate y, std::array<size_t, kBoardDirectionNum> * const shift_list)
{
  for(const auto direction : GetBoardDirection()){
    (*shift_list)[direction] = GetBitBoardShift(x, y, direction);
  }
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

inline const BoardPosition GetBoardPosition(const size_t index, const size_t shift)
{
  return (32 * index + shift / 2);
}

inline const BoardDirection GetBoardDirection(const BoardPosition board_position)
{
  constexpr size_t kMaxLateralBoardPosition = 255;
  constexpr size_t kMaxVerticalBoardPosition = 511;
  constexpr size_t kMaxLeftDiagonalBoardPosition = 767;
  constexpr size_t kMaxRightDiagonalBoardPosition = 1023;

  if(board_position <= kMaxLateralBoardPosition){
    return kLateralDirection;
  }else if(board_position <= kMaxVerticalBoardPosition){
    return kVerticalDirection;
  }else if(board_position <= kMaxLeftDiagonalBoardPosition){
    return kLeftDiagonalDirection;
  }else if(board_position <= kMaxRightDiagonalBoardPosition){
    return kRightDiagonalDirection;
  }else{
    assert(false);
    return kLateralDirection;
  }
}

inline const void GetBoardCordinate(const BoardPosition board_position, Cordinate * const x, Cordinate * const y)
{
  assert(x != nullptr);
  assert(y != nullptr);
  
  const BoardDirection board_direction = GetBoardDirection(board_position);

  if(board_direction == kLateralDirection){
    *x = board_position % 16;
    *y = board_position / 16;
  }else if(board_direction == kVerticalDirection){
    *x = (board_position - 256) / 16;
    *y = (board_position - 256) % 16;
  }else if(board_direction == kLeftDiagonalDirection){
    *y = (board_position - 512 + 1) % 16;
    *x = ((board_position - 512) / 16 - *y + 18) % 16;
  }else if(board_direction == kRightDiagonalDirection){
    *y = (board_position - 768 + 1) % 16;
    *x = (*y - (board_position - 768) / 16 + 30) % 16;
  }else{
    assert(false);
  }
}

inline const MovePosition GetBoardMove(const BoardPosition board_position)
{
  Cordinate x = 0, y = 0;
  GetBoardCordinate(board_position, &x, &y);

  if(!IsInBoard(x, y))
  {
    return kInvalidMove;
  }

  return GetMove(x, y);
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
  LineNeighborhood<kFourCheck> line_neighbor(move, *this);

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
  LineNeighborhood<kFourCheck> line_neighbor(move, *this);

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
  LineNeighborhood<kFourCheck> line_neighbor(move, *this);

  return line_neighbor.template IsFour<P>(guard_move);
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
  LineNeighborhood<kDoubleFourCheck> line_neighbor(move, *this);

  constexpr PositionState S = GetPlayerStone(P);
  line_neighbor.template SetCenterState<S>();

  return line_neighbor.template IsDoubleFour<P>();
}

}

#endif    // BIT_BOARD_INL_H
