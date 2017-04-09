#ifndef OPEN_STATE_INL_H
#define OPEN_STATE_INL_H

#include <cassert>

#include "Move.h"
#include "Conversion.h"
#include "OpenState.h"

namespace realcore
{

template<OpenStatePattern Pattern>
OpenState<Pattern>::OpenState(const BoardPosition open_position, const BoardPosition pattern_position)
: open_position_(open_position), pattern_position_(pattern_position), check_position_list_{{0}}, guard_position_list_{{0}} 
{
}

template<OpenStatePattern Pattern>
OpenState<Pattern>::OpenState(const OpenState<Pattern> &open_state)
{
  *this = open_state;
}

template<OpenStatePattern Pattern>
inline const BoardPosition OpenState<Pattern>::GetOpenPosition() const
{
  return open_position_;
}

template<OpenStatePattern Pattern>
inline const BoardPosition OpenState<Pattern>::GetPatternPosition() const
{
  return pattern_position_;
}

template<OpenStatePattern Pattern>
inline const CheckPositionList& OpenState<Pattern>::GetCheckPositionList() const
{
  return check_position_list_;
}

template<OpenStatePattern Pattern>
inline void OpenState<Pattern>::SetCheckPositionList(const CheckPositionList &check_position_list)
{
  check_position_list_ = check_position_list;
}

template<OpenStatePattern Pattern>
inline const GuardPositionList& OpenState<Pattern>::GetGuardPositionList() const
{
  return guard_position_list_;
}

template<OpenStatePattern Pattern>
inline void OpenState<Pattern>::SetGuardPositionList(const GuardPositionList &guard_position_list)
{
  guard_position_list_ = guard_position_list;
}

template<OpenStatePattern Pattern>
template<PlayerTurn P>
inline const bool OpenState<Pattern>::IsInfluenceMove(const MovePosition move) const
{
  assert(IsInBoardMove(move));

  const BoardDirection direction = GetBoardDirection(pattern_position_);
  const BoardPosition move_board_position = GetBoardPosition(move, direction);
  
  //! パターンのマッチ位置を起点とした各パターンごとの影響領域にmove_board_positionが入っているかチェックする
  //! @see doc/05_move_pattern/move_pattern.pptx, 空点状態の影響領域
  const int difference = static_cast<int>(move_board_position) - static_cast<int>(pattern_position_);

  switch(Pattern){
  case kNextOverline:
    return (P == kBlackTurn) ? false : (0 <= difference && difference <= 3);
  case kNextOpenFourBlack:
    return (P == kBlackTurn) ? (-2 <= difference && difference <= 5) : (-1 <= difference && difference <= 4);
  case kNextOpenFourWhite:
    return (P == kBlackTurn) ? (-1 <= difference && difference <= 4) : (-1 <= difference && difference <= 4);
  case kNextFourBlack:
    return (P == kBlackTurn) ? (-1 <= difference && difference <= 5) : (0 <= difference && difference <= 4);
  case kNextFourWhite:
    return (P == kBlackTurn) ? (0 <= difference && difference <= 4) : (0 <= difference && difference <= 4);
  case kNextSemiThreeBlack:
    return (P == kBlackTurn) ? (-2 <= difference && difference <= 5) : (-1 <= difference && difference <= 4);
  case kNextSemiThreeWhite:
    return (P == kBlackTurn) ? (-1 <= difference && difference <= 4) : (-1 <= difference && difference <= 4);
  default:
    assert(false);
    return false;
  }
}

template<OpenStatePattern Pattern>
bool IsEqual(const OpenState<Pattern> &open_state_1, const OpenState<Pattern> &open_state_2)
{
  if(open_state_1.open_position_ != open_state_2.open_position_){
    return false;
  }

  if(open_state_1.pattern_position_ != open_state_2.pattern_position_){
    return false;
  }

  if(open_state_1.check_position_list_ != open_state_2.check_position_list_){
    return false;
  }

  if(open_state_1.guard_position_list_ != open_state_2.guard_position_list_){
    return false;
  }

  return true;
}

template<OpenStatePattern Pattern>
inline const bool OpenState<Pattern>::operator==(const OpenState<Pattern> &open_state) const
{
  return IsEqual(*this, open_state);
}

template<OpenStatePattern Pattern>
inline const bool OpenState<Pattern>::operator!=(const OpenState<Pattern> &open_state) const
{
  return !(*this == open_state);
}

template<OpenStatePattern Pattern>
void Copy(const OpenState<Pattern> &open_state_from, OpenState<Pattern> * const open_state_to)
{
  open_state_to->open_position_ = open_state_from.open_position_;
  open_state_to->pattern_position_ = open_state_from.pattern_position_;
  open_state_to->check_position_list_ = open_state_from.check_position_list_;
  open_state_to->guard_position_list_ = open_state_from.guard_position_list_;
}

template<OpenStatePattern Pattern>
inline const OpenState<Pattern>& OpenState<Pattern>::operator=(const OpenState<Pattern> &open_state)
{
  if(this != &open_state){
    Copy(open_state, this);
  }

  return *this;
}

inline constexpr PlayerTurn GetPatternPlayerTurn(const OpenStatePattern pattern)
{
  return ((pattern == kNextOpenFourWhite) || (pattern == kNextFourWhite) || (pattern == kNextSemiThreeWhite)) ? kWhiteTurn : kBlackTurn;
}
}   // namespace realcore

#endif    // OPEN_STATE_INL_H
