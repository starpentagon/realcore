#ifndef BIT_SEARCH_INL_H
#define BIT_SEARCH_INL_H

#include <cassert>

#include "RealCore.h"
#include "BitSearch.h"

namespace realcore
{

//! @brief 状態(2bit)の上位bitをマスクする定数
constexpr BitBoard kUpperBitMask = 0b0101010101010101010101010101010101010101010101010101010101010101;

inline BitBoard GetStateBit(const std::string &str)
{
  assert(str.length() <= 32);

  BitBoard state_bit = 0ULL;

  for(size_t i=0, size=str.length(); i<size; ++i){
    state_bit <<= 2;
    
    if(str[i] == 'B'){
      state_bit |= kBlackStone;
    }else if(str[i] == 'W'){
      state_bit |= kWhiteStone;
    }else if(str[i] == 'O'){
      state_bit |= kOpenPosition;
    }else if(str[i] == 'X'){
      state_bit |= kOverBoard;
    }else{
      assert(false);
    }
  }

  return state_bit;
}

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
inline BitBoard GetConsectiveStoneBit(const BitBoard stone_bit)
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
}

}   // namespace realcore

#endif    // BIT_SEARCH_INL_H