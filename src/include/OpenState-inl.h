#ifndef OPEN_STATE_INL_H
#define OPEN_STATE_INL_H
#include <cassert>

#include "OpenState.h"

namespace realcore
{

template<OpenStatePattern Pattern>
OpenState<Pattern>::OpenState(const BoardPosition pattern_position)
: pattern_position_(pattern_position)
{
  switch(Pattern)
  {
  case kNextOverline:
  case kNextOpenFourBlack:
  case kNextOpenFourWhite:
    check_position_list_.reserve(0);
    guard_position_list_.reserve(0);
    break;

  case kNextFourBlack:
  case kNextFourWhite:
    check_position_list_.reserve(0);
    guard_position_list_.reserve(1);
    break;

  case kNextSemiThreeBlack:
    check_position_list_.reserve(2);
    guard_position_list_.reserve(4);
    break;
  case kNextSemiThreeWhite:
    check_position_list_.reserve(0);
    guard_position_list_.reserve(4);
    break;
  default:
    assert(false);
  }
}

template<OpenStatePattern Pattern>
inline const std::vector<BoardPosition>& OpenState<Pattern>::GetCheckPositionList() const
{
  return check_position_list_;
}

template<OpenStatePattern Pattern>
inline void OpenState<Pattern>::SetCheckPositionList(const std::vector<BoardPosition> &check_position_list)
{
  check_position_list_ = check_position_list;
}

template<OpenStatePattern Pattern>
inline const std::vector<BoardPosition>& OpenState<Pattern>::GetGuardPositionList() const
{
  return guard_position_list_;
}

template<OpenStatePattern Pattern>
inline void OpenState<Pattern>::SetGuardPositionList(const std::vector<BoardPosition> &guard_position_list)
{
  guard_position_list_ = guard_position_list;
}

}   // namespace realcore

#endif    // OPEN_STATE_INL_H
