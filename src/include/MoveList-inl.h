#ifndef MOVE_LIST_INL_H
#define MOVE_LIST_INL_H

#include <cassert>
#include "MoveList.h"

namespace realcore
{
  inline const MoveList& MoveList::operator=(const MoveList &move_list)
  {
    if(this != &move_list){
      move_list_.clear();

      const size_t reserve_size = CalcInitialReserveSize(move_list.size());
      move_list_.reserve(reserve_size);

      *this += move_list;
    }

    return *this;
  }

  inline const MoveList& MoveList::operator=(const MovePosition move)
  {
    move_list_.clear();

    const size_t reserve_size = CalcInitialReserveSize(1);
    move_list_.reserve(reserve_size);

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
    move_list_.push_back(move);
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
}

#endif  // MOVE_LIST_INL_H
