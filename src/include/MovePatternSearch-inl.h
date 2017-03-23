#ifndef MOVE_PATTERN_SEARCH_INL_H
#define MOVE_PATTERN_SEARCH_INL_H

#include "MovePatternSearch.h"

namespace realcore
{

inline const bool IsOverline(const std::uint64_t black_bit)
{
  constexpr size_t kOverlineStone = 6;    // 6個以上連続する黒石は長連
  const std::uint64_t overline_bit = GetConsectiveStoneBit<kOverlineStone>(black_bit);
  const bool is_overline = overline_bit != 0;

  return is_overline;
}

// 達四(白番)
template<>
inline const std::uint64_t SearchOpenFour<kWhiteTurn>(const std::uint64_t stone_bit, const std::uint64_t open_bit)
{
  constexpr size_t kFourStone = 4;
  std::uint64_t four_stone_bit = GetConsectiveStoneBit<kFourStone>(stone_bit);  // 4個以上連続する石のフラグ

  std::uint64_t open_four_bit = open_bit;      // O
  open_four_bit &= four_stone_bit >> (1 * 2);  // WWWWO
  open_four_bit &= open_bit >> (5 * 2);        // OWWWWO

  return open_four_bit;
}

// 達四(黒番)
template<>
inline const std::uint64_t SearchOpenFour<kBlackTurn>(const std::uint64_t stone_bit, const std::uint64_t open_bit)
{
  std::uint64_t open_four_bit = SearchOpenFour<kWhiteTurn>(stone_bit, open_bit);    // OBBBBO
  open_four_bit &= ~(stone_bit << (1 * 2));    // OBBBBOX, X\ne B
  open_four_bit &= ~(stone_bit >> (6 * 2));    // XOBBBBOX, X\ne B

  return open_four_bit;
}

// 四(白番)
template<>
inline const std::uint64_t SearchFour<kWhiteTurn>(const std::uint64_t stone_bit, const std::uint64_t open_bit)
{
  // [W4O1]パターンを検索する
  constexpr size_t kPatternSize = 5;
  std::array<uint64_t, kPatternSize> pattern_bit_list;
  GetStoneWithOneOpenBit<kPatternSize>(stone_bit, open_bit, &pattern_bit_list);
  
  std::uint64_t four_bit = 0;
  
  for(const auto four_pattern_bit : pattern_bit_list)
  {
    four_bit |= four_pattern_bit;
  }

  return four_bit;
}

// 四(黒番)
template<>
inline const std::uint64_t SearchFour<kBlackTurn>(const std::uint64_t stone_bit, const std::uint64_t open_bit)
{
  // [B4O1]パターンを検索する
  const uint64_t four_pattern_bit = SearchFour<kWhiteTurn>(stone_bit, open_bit);

  // 長連筋をマスクする(X[B4O1]X, X\ne B)
  const uint64_t overline_mask = ~(stone_bit << (1 * 2)) & ~(stone_bit >> (5 * 2));
  std::uint64_t four_bit = four_pattern_bit & overline_mask;

  return four_bit;
}

// 三(黒番)
template<>
inline const std::uint64_t SearchSemiThree<kBlackTurn>(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::uint64_t * const next_open_four_bit)
{
  return 1;
}

// 三(白番)
template<>
inline const std::uint64_t SearchSemiThree<kWhiteTurn>(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::uint64_t * const next_open_four_bit)
{
  return 2;
}

}   // namespace realcore

#endif  // MOVE_PATTERN_SEARCH_INL_H