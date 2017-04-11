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
  open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position);
}

template<>
inline void BoardOpenState::AddOpenState<kNextOpenFourBlack>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextOpenFourBlack;
  const BoardPosition open_position = GetOpenBoardPosition(pattern_position, pattern_search_index);
  open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position);
}

template<>
inline void BoardOpenState::AddOpenState<kNextOpenFourWhite>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextOpenFourWhite;
  const BoardPosition open_position = GetOpenBoardPosition(pattern_position, pattern_search_index);
  open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position);
}

template<>
inline void BoardOpenState::AddOpenState<kNextFourBlack>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextFourBlack;

  {
    const size_t open_index = GetLessIndexOfTwo(pattern_search_index);
    const size_t guard_index = GetGreaterIndexOfTwo(pattern_search_index);

    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    const BoardPosition guard_position = GetOpenBoardPosition(pattern_position, guard_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position);
    open_state_list_[Pattern].back().SetGuardPositionList({{guard_position}});
  }
  {
    const size_t open_index = GetGreaterIndexOfTwo(pattern_search_index);
    const size_t guard_index = GetLessIndexOfTwo(pattern_search_index);

    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    const BoardPosition guard_position = GetOpenBoardPosition(pattern_position, guard_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position);
    open_state_list_[Pattern].back().SetGuardPositionList({{guard_position}});
  }
}

template<>
inline void BoardOpenState::AddOpenState<kNextFourWhite>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextFourWhite;

  {
    const size_t open_index = GetLessIndexOfTwo(pattern_search_index);
    const size_t guard_index = GetGreaterIndexOfTwo(pattern_search_index);

    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    const BoardPosition guard_position = GetOpenBoardPosition(pattern_position, guard_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position);
    open_state_list_[Pattern].back().SetGuardPositionList({{guard_position}});
  }
  {
    const size_t open_index = GetGreaterIndexOfTwo(pattern_search_index);
    const size_t guard_index = GetLessIndexOfTwo(pattern_search_index);

    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    const BoardPosition guard_position = GetOpenBoardPosition(pattern_position, guard_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position);
    open_state_list_[Pattern].back().SetGuardPositionList({{guard_position}});
  }
}

template<>
inline void BoardOpenState::AddOpenState<kNextSemiThreeBlack>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextSemiThreeBlack;
  const BoardPosition left_side_guard_position = pattern_position + 4;  // O[B2O2]Oの左端のO
  const BoardPosition right_side_guard_position = pattern_position - 1;  // O[B2O2]Oの右端のO

  {
    const size_t open_index = GetLessIndexOfTwo(pattern_search_index);
    const size_t guard_index = GetGreaterIndexOfTwo(pattern_search_index);

    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    const BoardPosition guard_position = GetOpenBoardPosition(pattern_position, guard_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position);
    open_state_list_[Pattern].back().SetCheckPosition(guard_position);
    open_state_list_[Pattern].back().SetGuardPositionList({{guard_position, right_side_guard_position, left_side_guard_position}});
  }
  {
    const size_t open_index = GetGreaterIndexOfTwo(pattern_search_index);
    const size_t guard_index = GetLessIndexOfTwo(pattern_search_index);

    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    const BoardPosition guard_position = GetOpenBoardPosition(pattern_position, guard_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position);
    open_state_list_[Pattern].back().SetCheckPosition(guard_position);
    open_state_list_[Pattern].back().SetGuardPositionList({{guard_position, right_side_guard_position, left_side_guard_position}});
  }
}

template<>
inline void BoardOpenState::AddOpenState<kNextSemiThreeWhite>(const size_t pattern_search_index, const BoardPosition pattern_position)
{
  constexpr OpenStatePattern Pattern = kNextSemiThreeWhite;
  const BoardPosition left_side_guard_position = pattern_position + 4;  // O[B2O2]Oの左端のO
  const BoardPosition right_side_guard_position = pattern_position - 1;  // O[B2O2]Oの右端のO

  {
    const size_t open_index = GetLessIndexOfTwo(pattern_search_index);
    const size_t guard_index = GetGreaterIndexOfTwo(pattern_search_index);

    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    const BoardPosition guard_position = GetOpenBoardPosition(pattern_position, guard_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position);
    open_state_list_[Pattern].back().SetGuardPositionList({{guard_position, right_side_guard_position, left_side_guard_position}});
  }
  {
    const size_t open_index = GetGreaterIndexOfTwo(pattern_search_index);
    const size_t guard_index = GetLessIndexOfTwo(pattern_search_index);

    const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
    const BoardPosition guard_position = GetOpenBoardPosition(pattern_position, guard_index);
    
    open_state_list_[Pattern].emplace_back(Pattern, open_position, pattern_position);
    open_state_list_[Pattern].back().SetGuardPositionList({{guard_position, right_side_guard_position, left_side_guard_position}});
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

}   // namespace realcore

#endif    // BOARD_OPEN_STATE_INL_H