#ifndef OPEN_STATE_INL_H
#define OPEN_STATE_INL_H

#include <cassert>

#include "Move.h"
#include "Conversion.h"
#include "OpenState.h"

namespace realcore
{

inline const std::array<OpenStatePattern, OpenStatePatternNum>& GetAllOpenStatePattern()
{
  static const std::array<OpenStatePattern, OpenStatePatternNum> all_open_state_pattern_list{{
    kNextOverline,
    kNextOpenFourBlack,
    kNextOpenFourWhite,
    kNextFourBlack,
    kNextFourWhite,
    kNextSemiThreeBlack,
    kNextSemiThreeWhite,
  }};

  return all_open_state_pattern_list;
}

inline const BoardPosition OpenState::GetOpenPosition() const
{
  return open_position_;
}

inline const BoardPosition OpenState::GetPatternPosition() const
{
  return pattern_position_;
}

inline const BoardPosition OpenState::GetCheckPosition() const
{
  return check_position_;
}

inline void OpenState::SetCheckPosition(const BoardPosition check_position)
{
  check_position_ = check_position;
}

inline const GuardPositionList& OpenState::GetGuardPositionList() const
{
  return guard_position_list_;
}

inline void OpenState::SetGuardPositionList(const GuardPositionList &guard_position_list)
{
  guard_position_list_ = guard_position_list;
}

template<PlayerTurn P>
inline const bool OpenState::IsInfluenceMove(const MovePosition move) const
{
  assert(IsInBoardMove(move));

  const BoardDirection direction = GetBoardDirection(pattern_position_);
  const BoardPosition move_board_position = GetBoardPosition(move, direction);
  
  //! パターンのマッチ位置を起点とした各パターンごとの影響領域にmove_board_positionが入っているかチェックする
  //! @see doc/05_move_pattern/move_pattern.pptx, 空点状態の影響領域
  const int difference = static_cast<int>(move_board_position) - static_cast<int>(pattern_position_);

  switch(pattern_){
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

inline const bool OpenState::operator==(const OpenState &open_state) const
{
  return IsEqual(*this, open_state);
}

inline const bool OpenState::operator!=(const OpenState &open_state) const
{
  return !(*this == open_state);
}

inline const OpenState& OpenState::operator=(const OpenState &open_state)
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
