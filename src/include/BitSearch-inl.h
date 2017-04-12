#ifndef BIT_SEARCH_INL_H
#define BIT_SEARCH_INL_H

#include <climits>
#include <cassert>
#include <string>

#include "BitSearch.h"

namespace realcore
{

//! @brief 状態(2bit)の上位bitをクリアする定数
constexpr std::uint64_t kUpperBitMask = 0b0101010101010101010101010101010101010101010101010101010101010101;

template<size_t N>
inline constexpr StateBit RightShift(const StateBit state_bit)
{
  static_assert(0 <= N && N <= 31, "N must be in [0, 31]");
  return state_bit >> (2 * N);
}

template<size_t N>
inline constexpr StateBit LeftShift(const StateBit state_bit)
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

template<PlayerTurn P>
inline constexpr std::uint64_t GetPlayerStoneBit(const StateBit state_bit)
{
  return P == kBlackTurn ? GetBlackStoneBit(state_bit) : GetWhiteStoneBit(state_bit);
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

inline const BoardPosition GetOpenBoardPosition(const BoardPosition pattern_position, const size_t open_index)
{
  return pattern_position + open_index;
}

inline const size_t GetLessIndexOfTwo(const size_t index)
{
  assert(index < kTwoOfFivePattern);
  
  static const std::array<size_t, kTwoOfFivePattern> less_index{{
    0,
    0, 1,
    0, 1, 2,
    0, 1, 2, 3,
  }};

  return less_index[index];
}

inline const size_t GetGreaterIndexOfTwo(const size_t index)
{
  assert(index < kTwoOfFivePattern);
  
  static const std::array<size_t, kTwoOfFivePattern> greater_index{{
    1,
    2, 2,
    3, 3, 3,
    4, 4, 4, 4
  }};

  return greater_index[index];
}

template<std::size_t N>
inline void GetStoneWithTwoOpenBit(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, N> * const pattern_bit_list)
{
  static_assert(N == kTwoOfFourPattern || N == kTwoOfFivePattern, "N must be kTwoOfFourPattern(= 6) or kTwoOfFivePattern(= 10)");
  assert(pattern_bit_list != nullptr);

  // パターン長M(=[B3O2]なら5, [B2O2]なら4)
  constexpr size_t M = N == kTwoOfFourPattern ? 4 : 5;

  for(size_t open_index=0; open_index<N; ++open_index){
    // open_indexのみOで残りが黒石 or 白石のパターンを検索する
    size_t open_less_index = GetLessIndexOfTwo(open_index);
    size_t open_greater_index = GetGreaterIndexOfTwo(open_index);

    std::uint64_t search_bit = open_less_index == 0 ? open_bit : stone_bit;

    for(size_t shift=1; shift<M; ++shift){
      const std::uint64_t check_bit = (shift == open_less_index || shift == open_greater_index) ? open_bit : stone_bit;
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
  
  //! @note index_list->reserve(64)としてからemplace_backするより、いったんarrayに入れてからコピーした方が10%程度高速
  std::array<std::uint64_t, 64> index_array;
  size_t index_count = 0;

  while(bit != 0){
    const std::uint64_t rightmost_bit = GetRightmostBit(bit);
    const size_t rightmost_bit_index = GetNumberOfTrailingZeros(bit, rightmost_bit);
    index_array[index_count] = rightmost_bit_index;
    ++index_count;

    bit ^= rightmost_bit;
  }

  index_list->reserve(index_count);

  for(size_t i=0; i<index_count; i++){
    index_list->emplace_back(index_array[i]);
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

inline const bool IsMultipleBit(const std::uint64_t bit)
{
  if(bit == 0 || IsSingleBit(bit)){
    return false;
  }

  return true;
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

inline const std::uint64_t GetOpenBitInPattern(const size_t index, const std::uint64_t pattern_bit)
{
  return pattern_bit << (2 * index);
}

inline constexpr std::uint64_t GetCombinedBit(std::uint64_t bit_even, std::uint64_t bit_odd)
{
  return bit_even | (bit_odd << 1);
}

}   // namespace realcore

#endif    // BIT_SEARCH_INL_H