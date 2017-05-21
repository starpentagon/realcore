#include "BitSearch.h"

using namespace std;

namespace realcore
{

const StateBit GetStateBit(const string &str)
{
  // 空白を除去
  string filtered_str;

  for(size_t i=0, size=str.length(); i<size; ++i){
    if(str[i] != ' '){
      filtered_str += str[i];
    }
  }

  assert(filtered_str.length() <= 32);
  StateBit state_bit = 0ULL;

  for(size_t i=0, size=filtered_str.length(); i<size; ++i){
    state_bit <<= 2;
    
    if(filtered_str[i] == 'B'){
      state_bit |= kBlackStone;
    }else if(filtered_str[i] == 'W'){
      state_bit |= kWhiteStone;
    }else if(filtered_str[i] == 'O'){
      state_bit |= kOpenPosition;
    }else if(filtered_str[i] == 'X'){
      state_bit |= kOverBoard;
    }else{
      assert(false);
    }
  }

  return state_bit;
}

const string GetStateBitString(StateBit state_bit)
{
  string state_str;
  constexpr uint64_t kLowestStoneMask = 0b11;

  for(size_t i=0; i<32; i++){
    const PositionState state = static_cast<PositionState>(state_bit & kLowestStoneMask);

    if(state == kBlackStone){
      state_str = "B" + state_str;
    }else if(state == kWhiteStone){
      state_str = "W" + state_str;
    }else if(state == kOpenPosition){
      state_str = "O" + state_str;
    }else{
      state_str = "X" + state_str;
    }

    // 8文字ごとにスペース区切りを入れる
    if(i % 8 == 7 && i != 31){
      state_str = " " + state_str;
    }

    state_bit = RightShift<1>(state_bit);
  }

  return state_str;
}

}   // namespace realcore