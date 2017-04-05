#ifndef MOVE_PATTERN_SEARCH_INL_H
#define MOVE_PATTERN_SEARCH_INL_H

#include "MovePatternSearch.h"

namespace realcore
{

// 長連
inline const bool IsOverline(const std::uint64_t black_bit)
{
  constexpr size_t kOverlineStone = 6;    // 6個以上連続する黒石は長連
  const std::uint64_t overline_bit = GetConsectiveStoneBit<kOverlineStone>(black_bit);
  const bool is_overline = overline_bit != 0;

  return is_overline;
}

// 長連点
inline const std::uint64_t SearchNextOverline(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::uint64_t * const open_state_bit)
{
  assert(open_state_bit != nullptr);

  // [B3O1]パターンを検索する
  constexpr size_t kPatternSize = 4;
  std::array<uint64_t, kPatternSize> pattern_bit_list;
  GetStoneWithOneOpenBit<kPatternSize>(stone_bit, open_bit, &pattern_bit_list);
  
  std::uint64_t next_overline = 0;
  *open_state_bit = 0;

  for(size_t i=0; i<kPatternSize; i++){
    auto next_overline_pattern = pattern_bit_list[i];    // [B3O1]
    next_overline_pattern &= LeftShift<1>(stone_bit);   // [B3O1]B
    next_overline_pattern &= RightShift<4>(stone_bit);    // B[B3O1]B

    next_overline |= next_overline_pattern;
    *open_state_bit |= next_overline_pattern << (2 * i);
  }
  
  return next_overline;
}

// 達四
template<PlayerTurn P>
inline const std::uint64_t SearchOpenFour(const std::uint64_t stone_bit, const std::uint64_t open_bit)
{
  constexpr size_t kFourStone = 4;
  std::uint64_t four_stone_bit = GetConsectiveStoneBit<kFourStone>(stone_bit);  // 4個以上連続する石のフラグ

  std::uint64_t open_four_bit = open_bit;      // O
  open_four_bit &= RightShift<1>(four_stone_bit);  // BBBBO, WWWWO
  open_four_bit &= RightShift<5>(open_bit);        // OBBBBO, OWWWWO

  // 長連筋をマスクする(XOBBBBOX, X\ne B)
  std::uint64_t overline_mask = ~(0ULL);
  
  if(P == kBlackTurn){
    overline_mask = ~LeftShift<1>(stone_bit) & ~RightShift<6>(stone_bit);
  }

  open_four_bit &= overline_mask;

  return open_four_bit;
}

// 達四点
template<PlayerTurn P>
inline const std::uint64_t SearchNextOpenFour(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::uint64_t * const open_state_bit)
{
  assert(open_state_bit != nullptr);

  // [B3O1][W3O1]パターンを検索する
  constexpr size_t kPatternSize = 4;
  std::array<uint64_t, kPatternSize> pattern_bit_list;
  GetStoneWithOneOpenBit<kPatternSize>(stone_bit, open_bit, &pattern_bit_list);
  
  // 長連筋をマスクする(XO[B3O1]OX, X\ne B)
  std::uint64_t overline_mask = ~(0ULL);
  
  if(P == kBlackTurn){
    overline_mask = ~LeftShift<2>(stone_bit) & ~RightShift<5>(stone_bit);
  }

  std::uint64_t next_open_four = 0;
  *open_state_bit = 0;

  for(size_t i=0; i<kPatternSize; i++){
    auto next_open_four_pattern = pattern_bit_list[i];    // [B3O1][W3O1]
    next_open_four_pattern &= LeftShift<1>(open_bit);     // [B3O1]O, [W3O1]O
    next_open_four_pattern &= RightShift<4>(open_bit);    // O[B3O1]O, O[W3O1]O
    next_open_four_pattern &= overline_mask;              // XO[B3O1]O, O[W3O1]O

    next_open_four |= next_open_four_pattern;
    *open_state_bit |= next_open_four_pattern << (2 * i);
  }
  
  return next_open_four;
}

// 四
template<PlayerTurn P>
inline const std::uint64_t SearchFour(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::uint64_t * const guard_move_bit)
{
  assert(guard_move_bit != nullptr);
  *guard_move_bit = 0;

  // [B4O1][W4O1]パターンを検索する
  constexpr size_t kPatternSize = 5;
  std::array<uint64_t, kPatternSize> pattern_bit_list;
  GetStoneWithOneOpenBit<kPatternSize>(stone_bit, open_bit, &pattern_bit_list);
  
  // 長連筋をマスクする(X[B4O1]X, X\ne B)
  std::uint64_t overline_mask = ~(0ULL);

  if(P == kBlackTurn){
    overline_mask = ~LeftShift<1>(stone_bit) & ~RightShift<5>(stone_bit);
  }

  std::uint64_t four_bit = 0;
  
  for(size_t i=0; i<kPatternSize; i++){
    const std::uint64_t pattern_bit = pattern_bit_list[i] & overline_mask;
    
    four_bit |= pattern_bit;
    *guard_move_bit |= pattern_bit << (2 * i);
  } 

  return four_bit;
}

template<PlayerTurn P>
inline const std::uint64_t SearchFour(const std::uint64_t stone_bit, const std::uint64_t open_bit)
{
  std::uint64_t guard_move_bit = 0;
  return SearchFour<P>(stone_bit, open_bit, &guard_move_bit);
}

// 三
template<PlayerTurn P>
inline const std::uint64_t SearchSemiThree(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::uint64_t * const next_open_four_bit)
{
  assert(next_open_four_bit != nullptr);
  assert(*next_open_four_bit == 0);

  // [B3O1][W3O1]パターンを検索する
  constexpr size_t kPatternSize = 4;
  std::array<std::uint64_t, kPatternSize> pattern_bit_list;
  GetStoneWithOneOpenBit<kPatternSize>(stone_bit, open_bit, &pattern_bit_list);

  // O[(B|W|O|X)4]Oのマスク
  const std::uint64_t open_mask = RightShift<5>(open_bit) & open_bit;

  // 長連筋をマスクする(XO[B3O1]OX, X\ne B)
  std::uint64_t overline_mask = ~(0ULL);
  
  if(P == kBlackTurn){
    overline_mask = ~LeftShift<1>(stone_bit) & ~RightShift<6>(stone_bit);
  }

  std::uint64_t three_bit = 0;

  for(size_t i=0; i<kPatternSize; i++){
    auto three_pattern_bit = RightShift<1>(pattern_bit_list[i]);  // [B301][W3O1]
    three_pattern_bit &= open_mask;     // O[B3O1]O, O[W3O1]O
    three_pattern_bit &= overline_mask; // XO[B3O1]OX

    three_bit |= three_pattern_bit;

    // 達四をつくる位置を設定
    // three_pattern_bitはO[(B|W)3O1]Oの右端のOに1が立ち、[(B|W)3O1]のOの位置は右からi番目
    const size_t next_open_four_shift = (i + 1) * 2;
    *next_open_four_bit |= three_pattern_bit << next_open_four_shift;
  }

  return three_bit;
}

}   // namespace realcore

#endif  // MOVE_PATTERN_SEARCH_INL_H