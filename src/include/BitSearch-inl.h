#ifndef BIT_SEARCH_INL_H
#define BIT_SEARCH_INL_H

#include <cassert>
#include <string>

#include "RealCore.h"
#include "BitSearch.h"

namespace realcore
{

//! @brief 状態(2bit)の上位bitをマスクする定数
constexpr BitBoard kUpperBitMask = 0b0101010101010101010101010101010101010101010101010101010101010101;

inline constexpr BitBoard GetBlackStoneBit(const BitBoard bit_board)
{
  return (~bit_board >> 1) & bit_board & kUpperBitMask;
}

inline constexpr BitBoard GetWhiteStoneBit(const BitBoard bit_board)
{
  return (bit_board >> 1) & (~bit_board) & kUpperBitMask;
}

inline constexpr BitBoard GetOpenPositionBit(const BitBoard bit_board)
{
  return (bit_board >> 1) & bit_board & kUpperBitMask;
}

template<std::size_t N>
const inline BitBoard GetConsectiveStoneBit(const BitBoard stone_bit)
{
  static_assert(1 <= N && N <= kBoardLineNum, "N must be in [1, kBoardLineNum].");
  BitBoard consective_bit = stone_bit;

  for(size_t shift=1; shift<N; ++shift){
    consective_bit &= stone_bit >> (2 * shift);
  }

  return consective_bit;
}

template<std::size_t N>
inline void GetStoneWithOneOpenBit(const BitBoard stone_bit, const BitBoard open_bit, std::array<BitBoard, N> * const pattern_bit_list)
{
  assert(pattern_bit_list != nullptr);

  for(size_t open_index=0; open_index<N; ++open_index){
    // open_indexのみOで残りが黒石 or 白石のパターンを検索する
    BitBoard search_bit = open_index == 0 ? open_bit : stone_bit;

    for(size_t shift=1; shift<N; ++shift){
      const BitBoard check_bit = (shift == open_index) ? open_bit : stone_bit;
      search_bit &= check_bit >> (2 * shift);
    }

    (*pattern_bit_list)[open_index] = search_bit;
  }
}

inline size_t GetNumberOfTrailingZeros(const BitBoard bit_board)
{
  const int64_t signed_bit = static_cast<int64_t>(bit_board);
  const uint64_t rightmost_bit = signed_bit & (-signed_bit);
  constexpr uint64_t kDeBruijnSequence = 0x03F566ED27179461ULL;
  const uint64_t shifted_DeBruijn = kDeBruijnSequence * rightmost_bit;
  const size_t truncated_DeBruijn = shifted_DeBruijn >> 58;

  static const std::array<size_t, 64> kDeBruijnMapping{{
    #include "def/DeBruijnMapping.h"
  }};

  return kDeBruijnMapping[truncated_DeBruijn];
}

}   // namespace realcore

#endif    // BIT_SEARCH_INL_H