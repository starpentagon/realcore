#include "BitSearch.h"

namespace realcore
{
const BitBoard GetStateBit(const std::string &str)
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


}   // namespace realcore