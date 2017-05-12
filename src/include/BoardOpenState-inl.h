#ifndef BOARD_OPEN_STATE_INL_H
#define BOARD_OPEN_STATE_INL_H

#include <iostream>
#include <vector>

#include "OpenState.h"
#include "LineNeighborhood.h"
#include "BoardOpenState.h"

namespace realcore
{

inline const BoardOpenState& BoardOpenState::operator=(const BoardOpenState &rhs)
{
  if(this != &rhs){
    Copy(rhs, this);
  }

  return *this;
}

inline const bool BoardOpenState::operator==(const BoardOpenState &rhs) const
{
  return IsEqual(*this, rhs);
}

inline const bool BoardOpenState::operator!=(const BoardOpenState &rhs) const
{
  return !(*this == rhs);
}

inline const std::vector<OpenState>& BoardOpenState::GetList(const OpenStatePattern pattern) const
{
  return open_state_list_[pattern];
}

template<>
inline void BoardOpenState::AddOpenState<kNextOverline>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextOverline;
  const BoardPosition open_position = GetOpenBoardPosition(pattern_position, pattern_search_index);
  open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
}

template<>
inline void BoardOpenState::AddOpenState<kNextOpenFourBlack>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextOpenFourBlack;
  const BoardPosition open_position = GetOpenBoardPosition(pattern_position, pattern_search_index);
  open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
}

template<>
inline void BoardOpenState::AddOpenState<kNextOpenFourWhite>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextOpenFourWhite;
  const BoardPosition open_position = GetOpenBoardPosition(pattern_position, pattern_search_index);
  open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
}

template<>
inline void BoardOpenState::AddOpenState<kNextFourBlack>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextFourBlack;

  {
    const size_t open_index = GetLessIndexOfTwo(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
  {
    const size_t open_index = GetGreaterIndexOfTwo(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
}

template<>
inline void BoardOpenState::AddOpenState<kNextFourWhite>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextFourWhite;

  {
    const size_t open_index = GetLessIndexOfTwo(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
  {
    const size_t open_index = GetGreaterIndexOfTwo(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
}

template<>
inline void BoardOpenState::AddOpenState<kNextSemiThreeBlack>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextSemiThreeBlack;

  {
    const size_t open_index = GetLessIndexOfTwo(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
  {
    const size_t open_index = GetGreaterIndexOfTwo(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
}

template<>
inline void BoardOpenState::AddOpenState<kNextSemiThreeWhite>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextSemiThreeWhite;

  {
    const size_t open_index = GetLessIndexOfTwo(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
  {
    const size_t open_index = GetGreaterIndexOfTwo(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
}

template<>
inline void BoardOpenState::AddOpenState<kNextPointOfSwordBlack>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextPointOfSwordBlack;

  {
    const size_t open_index = GetMinIndexOfThree(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
  {
    const size_t open_index = GetMedianIndexOfThree(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
  {
    const size_t open_index = GetMaxIndexOfThree(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
}

template<>
inline void BoardOpenState::AddOpenState<kNextPointOfSwordWhite>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextPointOfSwordWhite;

  {
    const size_t open_index = GetMinIndexOfThree(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
  {
    const size_t open_index = GetMedianIndexOfThree(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
  {
    const size_t open_index = GetMaxIndexOfThree(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
}

template<>
inline void BoardOpenState::AddOpenState<kNextTwoBlack>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextTwoBlack;

  {
    const size_t open_index = GetMinIndexOfThree(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
  {
    const size_t open_index = GetMedianIndexOfThree(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
  {
    const size_t open_index = GetMaxIndexOfThree(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
}

template<>
inline void BoardOpenState::AddOpenState<kNextTwoWhite>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextTwoWhite;

  {
    const size_t open_index = GetMinIndexOfThree(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
  {
    const size_t open_index = GetMedianIndexOfThree(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
  {
    const size_t open_index = GetMaxIndexOfThree(pattern_search_index);
    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position, pattern_search_index);
  }
}

template<PlayerTurn P>
void BoardOpenState::ClearInfluencedOpenState(const std::vector<OpenState> &open_state_list, const MovePosition move, std::vector<OpenState> * const cleared_open_state_list) const
{
  assert(cleared_open_state_list != nullptr);
  assert(cleared_open_state_list->empty());

  cleared_open_state_list->reserve(open_state_list.size());

  for(const auto &open_state : open_state_list){
    const bool is_influenced = open_state.IsInfluenceMove<P>(move);
    
    if(is_influenced){
      continue;
    }

    cleared_open_state_list->emplace_back(open_state);
  }
}

inline void BoardOpenState::ClearInfluencedOpenState(const bool is_black_turn, const std::vector<OpenState> &open_state_list, const MovePosition move, std::vector<OpenState> * const cleared_open_state_list) const
{
  if(is_black_turn){
    ClearInfluencedOpenState<kBlackTurn>(open_state_list, move, cleared_open_state_list);
  }else{
    ClearInfluencedOpenState<kWhiteTurn>(open_state_list, move, cleared_open_state_list);
  }
}

inline const bool BoardOpenState::empty() const
{
  for(const auto pattern : GetAllOpenStatePattern()){
    if(!open_state_list_[pattern].empty()){
      return false;
    }
  }

  return true;
}

template<OpenStatePattern Pattern>
void BoardOpenState::ReserveList(const size_t list_size)
{
  open_state_list_[Pattern].reserve(list_size);
}

inline const UpdateOpenStateFlag& BoardOpenState::GetUpdateOpenStateFlag() const
{
  return update_flag_;
}

}   // namespace realcore

#endif    // BOARD_OPEN_STATE_INL_H