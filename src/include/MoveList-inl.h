#ifndef MOVE_LIST_INL_H
#define MOVE_LIST_INL_H

#include <cassert>
#include "BitSearch.h"
#include "Move.h"
#include "MoveList.h"

namespace realcore
{
inline const MoveList& MoveList::operator=(const MoveList &move_list)
{
  if(this != &move_list){
    move_list_.clear();
    ReserveInitial(move_list.size());

    *this += move_list;
  }

  return *this;
}

inline const MoveList& MoveList::operator=(const MovePosition move)
{
  move_list_.clear();
  ReserveInitial(1);

  *this += move;
  return *this;
}

inline const MoveList& MoveList::operator+=(const MoveList &move_list)
{
  for(const auto move : move_list){
    *this += move;
  }

  return *this;
}

inline const MoveList& MoveList::operator+=(const MovePosition move)
{
  move_list_.emplace_back(move);
  return *this;
}

inline const bool MoveList::operator==(const MoveList &move_list) const
{
  size_t list_size = size();
  
  if(list_size != move_list.size()){
    return false;
  }

  for(size_t i=0; i<list_size; i++){
    if(move_list_[i] != move_list[i]){
      return false;
    }
  }

  return true;
}

inline const bool MoveList::operator!=(const MoveList &move_list) const
{
  // todo 実装
  return !(*this == move_list);
}

inline const MovePosition MoveList::operator[](const size_t index) const
{
  assert(index < size());
  return move_list_[index];
}

inline MovePosition& MoveList::operator[](const size_t index)
{
  assert(index < size());
  return move_list_[index];
}

inline const MoveList& MoveList::operator--()
{
  assert(!move_list_.empty());
  
  move_list_.pop_back();
  return *this;
}

inline const MovePosition MoveList::GetLastMove() const
{
  if(move_list_.empty())
    return kInvalidMove;
  
  return move_list_.back();
}

inline std::vector<MovePosition>::const_iterator MoveList::begin() const{
  return move_list_.begin();
}

inline std::vector<MovePosition>::const_iterator MoveList::end() const{
  return move_list_.end();
}

inline const size_t MoveList::size() const{
  return move_list_.size();
}

inline void MoveList::clear(){
  move_list_.clear();
}

inline const bool MoveList::empty() const{
  return move_list_.empty();
}

inline const size_t MoveList::CalcInitialReserveSize(const size_t initial_list_size) const
{
  // 領域の再確保を抑制するため初期化時のリストより長めの領域を確保しておく
  size_t reserve_list_size = initial_list_size + 16;
  reserve_list_size += 8 - (reserve_list_size % 8);   // 「8 - (8で割った余り)」を加算して8の倍数にする

  return reserve_list_size;
}

inline const bool MoveList::IsBlackTurn() const{
  return move_list_.size() % 2 == 0;
}

inline void GetMoveList(const MoveBitSet &move_bit_set, MoveList *move_list)
{
  assert(move_list != nullptr);
  constexpr MoveBitSet bit_mask(0xFFFFFFFFFFFFFFFF);

  for(size_t i=0; i<4; i++){
    const auto shift_num = 64 * i;
    const auto value = ((move_bit_set >> shift_num) & bit_mask).to_ullong();
    
    if(value == 0){
      continue;
    }

    std::vector<size_t> index_list;
    GetBitIndexList(value, &index_list);

    for(const auto index : index_list){
      *move_list += static_cast<MovePosition>(index + shift_num);
    }
  }
}

inline const MoveBitSet& GetInBoardMoveBitSet(){
  static MoveBitSet in_board_move_bit;
  
  if(in_board_move_bit.none()){
    for(const auto move : GetAllInBoardMove()){
      in_board_move_bit.set(move);
    }
  }

  return in_board_move_bit;
}

}

#endif  // MOVE_LIST_INL_H
