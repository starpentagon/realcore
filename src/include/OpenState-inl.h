#ifndef OPEN_STATE_INL_H
#define OPEN_STATE_INL_H
#include <cassert>

#include "Move.h"
#include "BitBoard.h"
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

}   // namespace realcore

#endif    // OPEN_STATE_INL_H
