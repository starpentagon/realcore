#ifndef LINE_NEIGHBORHOOD_INL_H
#define LINE_NEIGHBORHOOD_INL_H

#include "MovePatternSearch.h"
#include "OpenState.h"
#include "BoardOpenState.h"
#include "LineNeighborhood.h"

namespace realcore
{
template<PositionState S>
inline void LineNeighborhood::SetCenterState()
{
  static_assert(S == kBlackStone || S == kWhiteStone, "PositionState must be kBlackStone or kWhiteStone");
  
  constexpr size_t kLowerCenter = 14;   // 下位32bitの中心位置
  constexpr size_t kUpperCenter = 46;   // 上位32bitの中心位置
  
#ifndef NDEBUG
  constexpr std::uint64_t center_open_mask = (0b11ULL << kLowerCenter) | (0b11ULL << kUpperCenter);
#endif
  assert((local_bit_board_[0] & center_open_mask) == center_open_mask);
  assert((local_bit_board_[1] & center_open_mask) == center_open_mask);

  constexpr std::uint64_t stone_xor_mask = S == kBlackStone ? 
  ((0b10ULL << kLowerCenter) | (0b10ULL << kUpperCenter)) : 
  ((0b01ULL << kLowerCenter) | (0b01ULL << kUpperCenter));
  
  local_bit_board_[0] ^= stone_xor_mask;
  local_bit_board_[1] ^= stone_xor_mask;
}

inline const BoardDirection LineNeighborhood::GetBoardDirection(const size_t index, const size_t bit_index) const
{
  const auto &board_direction_list = realcore::GetBoardDirection();

  constexpr size_t kMinUpperBitIndex = 32;
  const size_t direction_index = 2 * index + (bit_index < kMinUpperBitIndex ? 0 : 1);

  return board_direction_list[direction_index];
}

template<PlayerTurn P>
const bool LineNeighborhood::IsOpenFour() const
{
  const auto combined_stone_bit = GetPlayerStoneCombinedBit<P>();
  const auto combined_open_bit = GetOpenPositionCombinedBit();

  const auto open_four_bit = SearchOpenFour<P>(combined_stone_bit, combined_open_bit);
  return (open_four_bit != 0);
}

template<PlayerTurn P>
const bool LineNeighborhood::IsFour(MovePosition * const guard_move) const
{
  assert(guard_move != nullptr);

  const auto combined_stone_bit = GetPlayerStoneCombinedBit<P>();
  const auto combined_open_bit = GetOpenPositionCombinedBit();

  std::uint64_t guard_move_bit = 0;
  const auto four_bit = SearchFour<P>(combined_stone_bit, combined_open_bit, &guard_move_bit);

  if(four_bit != 0){
    std::vector<size_t> combined_shift_list;
    GetBitIndexList(guard_move_bit, &combined_shift_list);

    assert(!combined_shift_list.empty());
    const auto guard_board_position = GetBoardPosition(combined_shift_list[0]);
    *guard_move = GetBoardMove(guard_board_position);
    
    return true;
  }

  return false;
}

template<PlayerTurn P>
void LineNeighborhood::GetDoubleFourInfluenceArea(const std::uint64_t four_bit, const std::uint64_t combined_open_bit, const std::uint64_t make_five_move_bit, MoveBitSet * const influence_area) const
{
  if(influence_area == nullptr){
    return;
  }

  // 影響領域を求める
  influence_area->set(move_);   // 着手点

  // 五連を作る位置
  std::vector<size_t> combined_shift_list;
  GetBitIndexList(make_five_move_bit, &combined_shift_list);

  for(const auto combined_shift : combined_shift_list){
    const auto board_position = GetBoardPosition(combined_shift);
    const auto move = GetBoardMove(board_position);

    influence_area->set(move);
  }

  // 黒番の場合は長連筋の位置(X[B4O1]X)
  if(P == kBlackTurn){
    const auto overline_bit = (combined_open_bit & RightShift<1>(four_bit)) | (combined_open_bit & LeftShift<5>(four_bit));
    std::vector<size_t> combined_shift_list;
    GetBitIndexList(overline_bit, &combined_shift_list);

    for(const auto combined_shift : combined_shift_list){
      const auto board_position = GetBoardPosition(combined_shift);
      const auto move = GetBoardMove(board_position);

      influence_area->set(move);
    }
  }
}

template<PlayerTurn P>
const bool LineNeighborhood::IsDoubleFour(MoveBitSet * const influence_area) const
{
  // 「五連にする位置」(= 四の防手位置)の数をカウントする
  const auto combined_stone_bit = GetPlayerStoneCombinedBit<P>();
  const auto combined_open_bit = GetOpenPositionCombinedBit();

  const auto open_four_bit = SearchOpenFour<P>(combined_stone_bit, combined_open_bit);
  std::uint64_t make_five_move_bit = 0;
  const auto four_bit = SearchFour<P>(combined_stone_bit, combined_open_bit, &make_five_move_bit);
  
  // 達四があると五連にする位置が2カ所あるので重複カウントしないように片方をオフにする
  // @see doc/06_forbidden_check/forbidden_check.pptx, 「達四がある場合の四のマッチ方法」
  make_five_move_bit ^= RightShift<1>(open_four_bit);

  if(IsMultipleBit(make_five_move_bit)){
    GetDoubleFourInfluenceArea<P>(four_bit, combined_open_bit, make_five_move_bit, influence_area);
    return true;
  }

  return false;
}

template<PlayerTurn P>
inline const bool LineNeighborhood::IsDoubleFour() const
{
  return IsDoubleFour<P>(nullptr);
}

template<PlayerTurn P>
void LineNeighborhood::GetDoubleSemiThreeInfluenceArea(const std::uint64_t semi_three_bit, const std::uint64_t combined_open_bit, const std::uint64_t next_open_four_bit, MoveBitSet * const influence_area) const
{
  if(influence_area == nullptr){
    return;
  }

  // 影響領域を求める
  influence_area->set(move_);   // 着手点

  // O[B3O1]O, O[W3O1]Oの空点位置
  auto open_bit = next_open_four_bit;
  open_bit |= RightShift<1>(semi_three_bit);
  open_bit |= LeftShift<4>(semi_three_bit);

  std::vector<size_t> combined_shift_list;
  GetBitIndexList(open_bit, &combined_shift_list);

  for(const auto combined_shift : combined_shift_list){
    const auto board_position = GetBoardPosition(combined_shift);
    const auto move = GetBoardMove(board_position);

    influence_area->set(move);
  }

  // 黒番の場合は長連筋XO[B3O1]OXの位置
  if(P == kBlackTurn){
    const auto overline_bit = (combined_open_bit & RightShift<2>(semi_three_bit)) | (combined_open_bit & LeftShift<5>(semi_three_bit));
    std::vector<size_t> combined_shift_list;
    GetBitIndexList(overline_bit, &combined_shift_list);

    for(const auto combined_shift : combined_shift_list){
      const auto board_position = GetBoardPosition(combined_shift);
      const auto move = GetBoardMove(board_position);

      influence_area->set(move);
    }
  }
}

inline const BoardPosition LineNeighborhood::GetBoardPosition(const size_t combined_shift) const
{
  const size_t local_index = (combined_shift % 2 == 0) ? 0 : 1;   // local bit boardのindex
  const size_t local_shift = (combined_shift % 2 == 0) ? combined_shift : combined_shift - 1;   // local bit boardでのshift量
  const BoardDirection board_direction = GetBoardDirection(local_index, local_shift);
  const BoardPosition center_board_position = realcore::GetBoardPosition(move_, board_direction);   // 近傍中心のBoardPosition
  
  constexpr size_t kMinUpperBitIndex = 32;
  constexpr size_t kLowerCenter = 14;   // 下位32bitの中心位置
  constexpr size_t kUpperCenter = 46;   // 上位32bitの中心位置

  const auto center_shift = local_shift < kMinUpperBitIndex ? kLowerCenter : kUpperCenter;    // 近傍中心のShift量
  const auto board_position_diff = GetIndexDifference(center_shift, local_shift) / 2;   // 近傍中心から見た対象位置へのBoardPositionの差 = Shift量の差/2

  return static_cast<BoardPosition>(center_board_position + board_position_diff);
}

template<OpenStatePattern Pattern>
void LineNeighborhood::GetOpenState(const std::uint64_t combined_stone_bit, const std::uint64_t combined_open_bit, BoardOpenState * const board_open_state) const
{
  assert(distance_ == kOpenStateNeighborhoodSize);
  assert(board_open_state != nullptr);
  
  // 近傍中心は着手済で少なくとも4つはbitが立つため0-1bit判定は行わない
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
    
    const size_t open_state_count = (Pattern == kNextOverline || Pattern == kNextOpenFourBlack || Pattern == kNextOpenFourWhite) ? 1 : 2;
    const size_t list_size = board_open_state->GetList(Pattern).size() + open_state_count * bit_index_list.size();
    board_open_state->ReserveList<Pattern>(list_size);

    for(const auto combined_shift : bit_index_list){
      const BoardPosition pattern_position = GetBoardPosition(combined_shift);
      board_open_state->AddOpenState<Pattern>(pattern_index, pattern_position);
    }
  }
}

template<PlayerTurn P>
inline std::uint64_t LineNeighborhood::GetPlayerStoneCombinedBit() const
{
  const auto bit_even = GetPlayerStoneBit<P>(local_bit_board_[0]);
  const auto bit_odd = GetPlayerStoneBit<P>(local_bit_board_[1]);
  
  return GetCombinedBit(bit_even, bit_odd);
}

inline std::uint64_t LineNeighborhood::GetOpenPositionCombinedBit() const
{
  const auto bit_even = GetOpenPositionBit(local_bit_board_[0]);
  const auto bit_odd = GetOpenPositionBit(local_bit_board_[1]);
  
  return GetCombinedBit(bit_even, bit_odd);
}

}   // namespace realcore

#endif    // LINE_NEIGHBORHOOD_INL_H
