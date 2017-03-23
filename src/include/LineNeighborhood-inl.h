#ifndef LINE_NEIGHBORHOOD_INL_H
#define LINE_NEIGHBORHOOD_INL_H

#include "MovePatternSearch.h"
#include "Board.h"
#include "LineNeighborhood.h"

namespace realcore
{

template<size_t N>
LineNeighborhood<N>::LineNeighborhood(const MovePosition move, const Board &board)
: local_bit_board_{{0}}
{
  std::array<StateBit, kBoardDirectionNum> line_neighborhood;
  board.GetLineNeighborhoodStateBit<N>(move, &line_neighborhood);

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
  
  constexpr std::uint64_t center_open_mask = (0b11ULL << kLowerCenter) | (0b11ULL << kUpperCenter);
  assert((local_bit_board_[0] & center_open_mask) == center_open_mask);
  assert((local_bit_board_[1] & center_open_mask) == center_open_mask);

  constexpr std::uint64_t stone_xor_mask = S == kBlackStone ? 
  ((0b10ULL << kLowerCenter) | (0b10ULL << kUpperCenter)) : 
  ((0b01ULL << kLowerCenter) | (0b01ULL << kUpperCenter));
  
  local_bit_board_[0] ^= stone_xor_mask;
  local_bit_board_[1] ^= stone_xor_mask;
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

  return kNonForbiddenMove;
}

}   // namespace realcore

#endif    // LINE_NEIGHBORHOOD_INL_H
