#ifndef LINE_NEIGHBORHOOD_INL_H
#define LINE_NEIGHBORHOOD_INL_H

#include "MovePatternSearch.h"
#include "BitBoard.h"
#include "LineNeighborhood.h"

namespace realcore
{

template<size_t N>
LineNeighborhood<N>::LineNeighborhood(const MovePosition move, const BitBoard &bit_board)
: local_bit_board_{{0}}, move_(move)
{
  std::array<StateBit, kBoardDirectionNum> line_neighborhood;
  bit_board.GetLineNeighborhoodStateBit<N>(move, &line_neighborhood);

  local_bit_board_[0] |= line_neighborhood[kLateralDirection];
  local_bit_board_[0] |= line_neighborhood[kVerticalDirection] << 32ULL;
  local_bit_board_[1] |= line_neighborhood[kLeftDiagonalDirection];
  local_bit_board_[1] |= line_neighborhood[kRightDiagonalDirection] << 32ULL;
}

template<size_t N>
template<PositionState S>
inline void LineNeighborhood<N>::SetCenterState()
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

template<size_t N>
inline const BoardDirection LineNeighborhood<N>::GetBoardDirection(const size_t index, const size_t bit_index) const
{
  const auto &board_direction_list = realcore::GetBoardDirection();

  constexpr size_t kMinUpperBitIndex = 32;
  const size_t direction_index = 2 * index + (bit_index < kMinUpperBitIndex ? 0 : 1);

  return board_direction_list[direction_index];
}

template<size_t N>
const ForbiddenCheckState LineNeighborhood<N>::ForbiddenCheck(std::vector<BoardPosition> * const next_open_four_list) const
{
  assert(next_open_four_list != nullptr);
  assert(next_open_four_list->empty());

  std::array<std::uint64_t, kLocalBitBoardNum> local_black_bit{{0}};
  std::array<std::uint64_t, kLocalBitBoardNum> local_open_bit{{0}};

  for(size_t i=0; i<kLocalBitBoardNum; i++){
    const auto state_bit = local_bit_board_[i];

    local_black_bit[i] = GetBlackStoneBit(state_bit);
    local_open_bit[i] = GetOpenPositionBit(state_bit);
  }

  // 長連
  for(const auto black_bit : local_black_bit){
    if(IsOverline(black_bit)){
      return kForbiddenMove;
    }
  }

  // 四々
  std::array<std::uint64_t, kLocalBitBoardNum> four_bit{{0}};

  for(size_t i=0; i<kLocalBitBoardNum; i++){
    const auto black_bit = local_black_bit[i];
    const auto open_bit = local_open_bit[i];

    const auto open_four_bit = SearchOpenFour<kBlackTurn>(black_bit, open_bit);
    four_bit[i] = SearchFour<kBlackTurn>(black_bit, open_bit);

    // 達四があると四のパターンが2カ所マッチするので片方をオフにする
    four_bit[i] ^= open_four_bit;
  }

  if(IsMultipleBit(four_bit[0], four_bit[1])){
    return kForbiddenMove;
  }

  // 見かけの三々
  std::array<std::uint64_t, kLocalBitBoardNum> semi_three_bit{{0}};
  std::array<std::uint64_t, kLocalBitBoardNum> next_open_four_bit{{0}};

  for(size_t i=0; i<kLocalBitBoardNum; i++){
    const auto black_bit = local_black_bit[i];
    const auto open_bit = local_open_bit[i];

    semi_three_bit[i] = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &(next_open_four_bit[i]));
  }

  if(!IsMultipleBit(semi_three_bit[0], semi_three_bit[1])){
    return kNonForbiddenMove;
  }

  // 見かけの三々が2つ以上存在する
  Cordinate x = 0, y = 0;
  GetMoveCordinate(move_, &x, &y);

  std::array<size_t, kBoardDirectionNum> index_list;
  GetBitBoardIndexList(x, y, &index_list);

  std::array<size_t, kBoardDirectionNum> shift_list;
  GetBitBoardShiftList(x, y, &shift_list);

  constexpr size_t kMinUpperBitIndex = 32;
  constexpr size_t kLowerCenter = 14;   // 下位32bitの中心位置
  constexpr size_t kUpperCenter = 46;   // 上位32bitの中心位置

  for(size_t index=0; index<kLocalBitBoardNum; index++){
    std::vector<size_t> bit_index_list;
    bit_index_list.reserve(4);   // 一方向で高々2つ * 2方向

    GetBitIndexList(next_open_four_bit[index], &bit_index_list);

    for(const auto bit_index : bit_index_list){
      const auto direction = GetBoardDirection(index, bit_index);
      const auto index = index_list[direction];
      const auto shift = shift_list[direction];
      
      // BitBoard配列での達四位置に対応するシフト量を求める
      const auto center_shift = bit_index < kMinUpperBitIndex ? kLowerCenter : kUpperCenter;
      const auto open_four_shift = shift + GetIndexDifference(center_shift, bit_index);

      const auto board_position = GetBoardPosition(index, open_four_shift);
      next_open_four_list->push_back(board_position);
    }
  }

  return kPossibleForbiddenMove;
}

}   // namespace realcore

#endif    // LINE_NEIGHBORHOOD_INL_H
