#include "MoveList.h"

namespace realcore
{

MoveList::MoveList()
{
  const size_t reserve_size = CalcInitialReserveSize(0);
  move_list_.reserve(reserve_size);
}

const size_t MoveList::CalcInitialReserveSize(const size_t initial_list_size) const
{
  // 領域の再確保を抑制するため初期化時のリストより長めの領域を確保しておく
  size_t reserve_list_size = initial_list_size + 16;
  reserve_list_size += 8 - (reserve_list_size % 8);   // 「8 - (8で割った余り)」を加算して8の倍数にする

  return reserve_list_size;
}
}   // namespace realcore