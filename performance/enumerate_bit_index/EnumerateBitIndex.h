#ifndef ENUMERATE_BIT_INDEX_H
#define ENUMERATE_BIT_INDEX_H

#include <iostream>
#include <cstdint>
#include <vector>

#include "BitSearch.h"

inline void ScanBitSequence(uint64_t bit_sequence, std::vector<size_t> * const index_list)
{
  size_t index = 0;
  constexpr uint64_t lowest_bit_mask = 1ULL;
  
  while(bit_sequence != 0){
    if((bit_sequence & lowest_bit_mask) != 0){
      index_list->push_back(index);
    }

    index++;
    bit_sequence >>= 1;
  }
}

inline void ShiftMap(uint64_t bit_sequence, std::vector<size_t> * const index_list)
{
  // 8bit value -> index listの変換テーブル
  static std::vector<size_t> value_index_table[256];
  static bool initialized = false;

  if(!initialized){
    for(uint64_t value=0; value<256; value++){
      ScanBitSequence(value, &value_index_table[value]);
    }

    initialized = true;
  }

  // 下位8bit mask
  constexpr std::uint64_t eight_bit_mask = 0b11111111;
  size_t shift_num = 0;

  while(bit_sequence != 0){
    const std::uint64_t value = bit_sequence & eight_bit_mask;

    for(auto index : value_index_table[value]){
      index_list->push_back(index + shift_num);
    }

    bit_sequence >>= 8;
    shift_num += 8;
  }
}

inline void EnumerateRightmostBit(uint64_t bit_sequence, std::vector<size_t> * const index_list)
{
  while(bit_sequence != 0){
    const std::uint64_t rightmost_bit = realcore::GetRightmostBit(bit_sequence);
    const size_t rightmost_bit_index = realcore::GetNumberOfTrailingZeros(bit_sequence, rightmost_bit);
    index_list->push_back(rightmost_bit_index);

    bit_sequence ^= rightmost_bit;
  }
}

#endif    // ENUMERATE_BIT_INDEX_H