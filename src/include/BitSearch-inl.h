#ifndef BIT_SEARCH_INL_H
#define BIT_SEARCH_INL_H

#include <climits>
#include <cassert>
#include <string>

#include "RealCore.h"
#include "BitSearch.h"

namespace realcore
{

//! @brief 状態(2bit)の上位bitをクリアする定数
constexpr std::uint64_t kUpperBitMask = 0b0101010101010101010101010101010101010101010101010101010101010101;

template<size_t N>
inline const StateBit RightShift(const StateBit state_bit)
{
  static_assert(0 <= N && N <= 31, "N must be in [0, 31]");
  return state_bit >> (2 * N);
}

template<size_t N>
inline const StateBit LeftShift(const StateBit state_bit)
{
  static_assert(0 <= N && N <= 31, "N must be in [0, 31]");
  return state_bit << (2 * N);
}

inline const int GetIndexDifference(const size_t index_from, const size_t index_to)
{
  assert(index_from < INT_MAX);
  assert(index_to < INT_MAX);
  return static_cast<int>(index_to) - static_cast<int>(index_from);
}

inline constexpr std::uint64_t GetBlackStoneBit(const StateBit state_bit)
{
  return (~state_bit >> 1) & state_bit & kUpperBitMask;
}

inline constexpr std::uint64_t GetWhiteStoneBit(const StateBit state_bit)
{
  return (state_bit >> 1) & (~state_bit) & kUpperBitMask;
}

inline constexpr std::uint64_t GetOpenPositionBit(const StateBit state_bit)
{
  return (state_bit >> 1) & state_bit & kUpperBitMask;
}

template<std::size_t N>
const inline std::uint64_t GetConsectiveStoneBit(const std::uint64_t stone_bit)
{
  static_assert(1 <= N && N <= kBoardLineNum, "N must be in [1, kBoardLineNum].");
  std::uint64_t consective_bit = stone_bit;

  for(size_t shift=1; shift<N; ++shift){
    consective_bit &= stone_bit >> (2 * shift);
  }

  return consective_bit;
}

template<std::size_t N>
inline void GetStoneWithOneOpenBit(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, N> * const pattern_bit_list)
{
  assert(pattern_bit_list != nullptr);

  for(size_t open_index=0; open_index<N; ++open_index){
    // open_indexのみOで残りが黒石 or 白石のパターンを検索する
    std::uint64_t search_bit = open_index == 0 ? open_bit : stone_bit;

    for(size_t shift=1; shift<N; ++shift){
      const std::uint64_t check_bit = (shift == open_index) ? open_bit : stone_bit;
      search_bit &= check_bit >> (2 * shift);
    }

    (*pattern_bit_list)[open_index] = search_bit;
  }
}

inline std::uint64_t GetRightmostBit(const std::uint64_t bit)
{
  const int64_t signed_bit = static_cast<int64_t>(bit);
  const std::uint64_t rightmost_bit = static_cast<std::uint64_t>(signed_bit & (-signed_bit));

  return rightmost_bit;
}

inline size_t GetNumberOfTrailingZeros(const std::uint64_t bit)
{
  assert(bit != 0);

  const std::uint64_t rightmost_bit = GetRightmostBit(bit);
  return GetNumberOfTrailingZeros(bit, rightmost_bit);
}

inline size_t GetNumberOfTrailingZeros(const std::uint64_t bit, const std::uint64_t rightmost_bit)
{
  assert(bit != 0);

  constexpr uint64_t kDeBruijnSequence = 0x03F566ED27179461ULL;
  const uint64_t shifted_DeBruijn = kDeBruijnSequence * rightmost_bit;
  const size_t truncated_DeBruijn = shifted_DeBruijn >> 58;

  static const std::array<size_t, 64> kDeBruijnMapping{{
    #include "def/DeBruijnMapping.h"
  }};

  return kDeBruijnMapping[truncated_DeBruijn];
}

inline void GetBitIndexList(std::uint64_t bit, std::vector<size_t> * const index_list)
{
  assert(index_list != nullptr);
  index_list->reserve(64);

  while(bit != 0){
    const std::uint64_t rightmost_bit = GetRightmostBit(bit);
    const size_t rightmost_bit_index = GetNumberOfTrailingZeros(bit, rightmost_bit);
    index_list->push_back(rightmost_bit_index);

    bit ^= rightmost_bit;
  }
}

template<size_t N>
inline constexpr std::uint64_t GetConsectiveBit()
{
  static_assert(1 <= N && N <= 63, "N must be in [1, 63]");
  return (1ULL << N) - 1;
}

inline const bool IsSingleBit(const std::uint64_t bit)
{
  assert(bit != 0);

  // 2の累乗かどうかを判定
  // @see "Ten Ways to Check if an Integer Is a Power Of Two in C: 9. Decrement and Compare"
  // @see (http://www.exploringbinary.com/ten-ways-to-check-if-an-integer-is-a-power-of-two-in-c/)
  return !(bit & (bit - 1));
}

inline const bool IsMultipleBit(const std::uint64_t bit_1, const std::uint64_t bit_2)
{
  // bit_1, bit_2のビット数が2未満になるのは以下の3通り
  if(bit_1 == 0 && bit_2 == 0){
    // bit_1, bit_2ともに0
    return false;
  }else if(bit_1 == 0 && IsSingleBit(bit_2)){
    // bit_1が0, bit_2のビット数が1
    return false;
  }else if(bit_2 == 0 && IsSingleBit(bit_1)){
    // bit_2が0, bit_1のビット数が1
    return false;
  }

  return true;
}

}   // namespace realcore

#endif    // BIT_SEARCH_INL_H