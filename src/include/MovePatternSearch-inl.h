#ifndef MOVE_PATTERN_SEARCH_INL_H
#define MOVE_PATTERN_SEARCH_INL_H

#include <algorithm>

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

inline constexpr size_t GetOpenStatePatternNum(const OpenStatePattern pattern)
{
  switch(pattern)
  {
  case kNextOverline:       // B[B3O1]B
  case kNextOpenFourBlack:  // XO[B3O1]OX
  case kNextOpenFourWhite:  // O[W3O1]O
    return 4;

  case kNextFourBlack:      // X[B3O2]X
  case kNextFourWhite:      // [W3O2]
    return 10;
  case kNextSemiThreeBlack: // XO[B2O2]OX
  case kNextSemiThreeWhite: // O[W2O2]O
    return 6;
  default:
    assert(false);
    return 0;
  }
}

template<>
inline void SearchOpenStatePattern<kNextOverline>(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, GetOpenStatePatternNum(kNextOverline)> * const pattern_search_bit_list)
{
  SearchNextOverline(stone_bit, open_bit, pattern_search_bit_list);
}

template<>
inline void SearchOpenStatePattern<kNextOpenFourBlack>(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, GetOpenStatePatternNum(kNextOpenFourBlack)> * const pattern_search_bit_list)
{
  SearchNextOpenFour<kBlackTurn>(stone_bit, open_bit, pattern_search_bit_list);
}

template<>
inline void SearchOpenStatePattern<kNextOpenFourWhite>(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, GetOpenStatePatternNum(kNextOpenFourWhite)> * const pattern_search_bit_list)
{
  SearchNextOpenFour<kWhiteTurn>(stone_bit, open_bit, pattern_search_bit_list);
}

template<>
inline void SearchOpenStatePattern<kNextFourBlack>(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, GetOpenStatePatternNum(kNextFourBlack)> * const pattern_search_bit_list)
{
  SearchNextFour<kBlackTurn>(stone_bit, open_bit, pattern_search_bit_list);
}

template<>
inline void SearchOpenStatePattern<kNextFourWhite>(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, GetOpenStatePatternNum(kNextFourWhite)> * const pattern_search_bit_list)
{
  SearchNextFour<kWhiteTurn>(stone_bit, open_bit, pattern_search_bit_list);
}

template<>
inline void SearchOpenStatePattern<kNextSemiThreeBlack>(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, GetOpenStatePatternNum(kNextSemiThreeBlack)> * const pattern_search_bit_list)
{
  SearchNextSemiThree<kBlackTurn>(stone_bit, open_bit, pattern_search_bit_list);
}

template<>
inline void SearchOpenStatePattern<kNextSemiThreeWhite>(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, GetOpenStatePatternNum(kNextSemiThreeWhite)> * const pattern_search_bit_list)
{
  SearchNextSemiThree<kWhiteTurn>(stone_bit, open_bit, pattern_search_bit_list);
}

// 長連点
inline void SearchNextOverline(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, kFourStonePattern> * const pattern_search_bit_list)
{
  assert(pattern_search_bit_list != nullptr);
  assert(*std::min_element(pattern_search_bit_list->begin(), pattern_search_bit_list->end()) == 0);
  assert(*std::max_element(pattern_search_bit_list->begin(), pattern_search_bit_list->end()) == 0);

  // [B3O1]パターンを検索する
  GetStoneWithOneOpenBit<kFourStonePattern>(stone_bit, open_bit, pattern_search_bit_list);
  
  for(size_t i=0; i<kFourStonePattern; i++){
    (*pattern_search_bit_list)[i] &= LeftShift<1>(stone_bit);    // [B3O1]B
    (*pattern_search_bit_list)[i] &= RightShift<4>(stone_bit);   // B[B3O1]B
  }
}

// 達四
template<PlayerTurn P>
inline const std::uint64_t SearchOpenFour(const std::uint64_t stone_bit, const std::uint64_t open_bit)
{
  std::uint64_t open_four_bit = GetConsectiveStoneBit<kFourStonePattern>(stone_bit);  // 4個以上連続する石のフラグ
  open_four_bit &= LeftShift<1>(open_bit);    // BBBBO, WWWWO
  open_four_bit &= RightShift<4>(open_bit);        // OBBBBO, OWWWWO

  // 長連筋をマスクする(XOBBBBOX, X\ne B)
  if(P == kBlackTurn){
    std::uint64_t overline_mask = ~(0ULL);
    overline_mask = ~LeftShift<2>(stone_bit) & ~RightShift<5>(stone_bit);
    open_four_bit &= overline_mask;
  }

  return open_four_bit;
}

// 達四点
template<PlayerTurn P>
inline void SearchNextOpenFour(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, kFourStonePattern> * const pattern_search_bit_list){
  assert(pattern_search_bit_list != nullptr);
  assert(*std::min_element(pattern_search_bit_list->begin(), pattern_search_bit_list->end()) == 0);
  assert(*std::max_element(pattern_search_bit_list->begin(), pattern_search_bit_list->end()) == 0);

  // [B3O1][W3O1]パターンを検索する
  GetStoneWithOneOpenBit<kFourStonePattern>(stone_bit, open_bit, pattern_search_bit_list);
  
  // 長連筋をマスクする(XO[B3O1]OX, X\ne B)
  std::uint64_t overline_mask = ~(0ULL);
  
  if(P == kBlackTurn){
    overline_mask = ~LeftShift<2>(stone_bit) & ~RightShift<5>(stone_bit);
  }

  for(size_t i=0; i<kFourStonePattern; i++){
    (*pattern_search_bit_list)[i] &= LeftShift<1>(open_bit);     // [B3O1]O, [W3O1]O
    (*pattern_search_bit_list)[i] &= RightShift<4>(open_bit);    // O[B3O1]O, O[W3O1]O
    (*pattern_search_bit_list)[i] &= overline_mask;              // XO[B3O1]OX, O[W3O1]O
  }
}

// 四
template<PlayerTurn P>
inline const std::uint64_t SearchFour(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::uint64_t * const guard_move_bit)
{
  assert(guard_move_bit != nullptr);
  *guard_move_bit = 0;

  // [B4O1][W4O1]パターンを検索する
  std::array<uint64_t, kFiveStonePattern> pattern_bit_list;
  GetStoneWithOneOpenBit<kFiveStonePattern>(stone_bit, open_bit, &pattern_bit_list);
  
  // 長連筋をマスクする(X[B4O1]X, X\ne B)
  std::uint64_t overline_mask = ~(0ULL);

  if(P == kBlackTurn){
    overline_mask = ~LeftShift<1>(stone_bit) & ~RightShift<5>(stone_bit);
  }

  std::uint64_t four_bit = 0;
  
  for(size_t i=0; i<kFiveStonePattern; i++){
    const std::uint64_t pattern_bit = pattern_bit_list[i] & overline_mask;
    
    four_bit |= pattern_bit;
    *guard_move_bit |= GetOpenBitInPattern(i, pattern_bit);
  } 

  return four_bit;
}

template<PlayerTurn P>
inline const std::uint64_t SearchFour(const std::uint64_t stone_bit, const std::uint64_t open_bit)
{
  std::uint64_t guard_move_bit = 0;
  return SearchFour<P>(stone_bit, open_bit, &guard_move_bit);
}

template<PlayerTurn P>
inline void SearchNextFour(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, kTwoOfFivePattern> * const pattern_search_bit_list)
{
  assert(pattern_search_bit_list != nullptr);
  assert(*std::min_element(pattern_search_bit_list->begin(), pattern_search_bit_list->end()) == 0);
  assert(*std::max_element(pattern_search_bit_list->begin(), pattern_search_bit_list->end()) == 0);

  // [B3O2][W3O2]パターンを検索する
  GetStoneWithTwoOpenBit<kTwoOfFivePattern>(stone_bit, open_bit, pattern_search_bit_list);
  
  if(P == kBlackTurn){
    // 長連筋をマスクする(X[B3O2]X, X\ne B)
    const std::uint64_t overline_mask = ~LeftShift<1>(stone_bit) & ~RightShift<5>(stone_bit);

    for(size_t i=0; i<kTwoOfFivePattern; i++){
      (*pattern_search_bit_list)[i] &= overline_mask;              // X[B3O2]X, [W3O2]
    }
  }
}

// 三
template<PlayerTurn P>
inline const std::uint64_t SearchSemiThree(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::uint64_t * const next_open_four_bit)
{
  assert(next_open_four_bit != nullptr);
  assert(*next_open_four_bit == 0);

  // [B3O1][W3O1]パターンを検索する
  std::array<std::uint64_t, kFourStonePattern> pattern_bit_list;
  GetStoneWithOneOpenBit<kFourStonePattern>(stone_bit, open_bit, &pattern_bit_list);

  // O[(B|W|O|X)4]Oのマスク
  const std::uint64_t open_mask = RightShift<4>(open_bit) & LeftShift<1>(open_bit);

  // 長連筋をマスクする(XO[B3O1]OX, X\ne B)
  std::uint64_t overline_mask = ~(0ULL);
  
  if(P == kBlackTurn){
    overline_mask = ~LeftShift<2>(stone_bit) & ~RightShift<5>(stone_bit);
  }

  std::uint64_t three_bit = 0;

  for(size_t i=0; i<kFourStonePattern; i++){
    auto three_pattern_bit = pattern_bit_list[i];  // [B301][W3O1]
    three_pattern_bit &= open_mask;     // O[B3O1]O, O[W3O1]O
    three_pattern_bit &= overline_mask; // XO[B3O1]OX

    three_bit |= three_pattern_bit;

    // 達四をつくる位置を設定
    *next_open_four_bit |= GetOpenBitInPattern(i, three_pattern_bit);
  }

  return three_bit;
}

template<PlayerTurn P>
inline void SearchNextSemiThree(const std::uint64_t stone_bit, const std::uint64_t open_bit, std::array<std::uint64_t, kTwoOfFourPattern> * const pattern_search_bit_list)
{
  assert(pattern_search_bit_list != nullptr);
  assert(*std::min_element(pattern_search_bit_list->begin(), pattern_search_bit_list->end()) == 0);
  assert(*std::max_element(pattern_search_bit_list->begin(), pattern_search_bit_list->end()) == 0);

  // [B2O2][W2O2]パターンを検索する
  GetStoneWithTwoOpenBit<kTwoOfFourPattern>(stone_bit, open_bit, pattern_search_bit_list);

  // 長連筋をマスクする(XO[B2O2]OX, X\ne B)
  std::uint64_t overline_mask = ~(0ULL);
  
  if(P == kBlackTurn){
    overline_mask = ~LeftShift<2>(stone_bit) & ~RightShift<5>(stone_bit);
  }
  
  for(size_t i=0; i<kTwoOfFourPattern; i++){
    (*pattern_search_bit_list)[i] &= LeftShift<1>(open_bit);    // [B2O2]O, [W2O2]O
    (*pattern_search_bit_list)[i] &= RightShift<4>(open_bit);   // O[B2O2]O, O[W2O2]O
    (*pattern_search_bit_list)[i] &= overline_mask;             // XO[B2O2]OX, O[W2O2]O
  }
}

}   // namespace realcore

#endif  // MOVE_PATTERN_SEARCH_INL_H