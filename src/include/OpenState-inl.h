#ifndef OPEN_STATE_INL_H
#define OPEN_STATE_INL_H

#include <cassert>

#include "Move.h"
#include "Conversion.h"
#include "BitSearch.h"
#include "OpenState.h"

namespace realcore
{

inline const std::array<OpenStatePattern, kOpenStatePatternNum>& GetAllOpenStatePattern()
{
  static const std::array<OpenStatePattern, kOpenStatePatternNum> all_open_state_pattern_list{{
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
  assert(pattern_ == kNextFourBlack || pattern_ == kNextFourWhite || pattern_ == kNextSemiThreeBlack || pattern_ == kNextSemiThreeWhite);
  const auto right_open_index = GetLessIndexOfTwo(pattern_search_index_);
  const auto right_open_position = GetOpenBoardPosition(pattern_position_, right_open_index);

  if(open_position_ == right_open_position){
    const auto left_open_index = GetGreaterIndexOfTwo(pattern_search_index_);
    const auto left_open_position = GetOpenBoardPosition(pattern_position_, left_open_index);
    
    return left_open_position;
  }else{
    return right_open_position;
  }
}

template<PlayerTurn P>
void OpenState::GetInfluenceArea(std::vector<BoardPosition> * const downward_influence_area) const
{
  assert(downward_influence_area != nullptr);
  assert(downward_influence_area->empty());
  
  constexpr size_t kMaxListSize = 6;    // 最も影響領域が大きいのは"XO[B2O2]OX"の6ヶ所
  downward_influence_area->reserve(kMaxListSize);
  
  switch(pattern_){
  case kNextOverline:
  {
    // B[B3O1]B
    if(P == kWhiteTurn){
      downward_influence_area->emplace_back(open_position_);
    }

    return;
  }

  case kNextOpenFourBlack:
  case kNextOpenFourWhite:
  {
    // XO[B3O1]OX, O[W3O1]O
    downward_influence_area->emplace_back(open_position_);          // 達四位置
    downward_influence_area->emplace_back(pattern_position_ - 1);   // 右側のO
    downward_influence_area->emplace_back(pattern_position_ + 4);   // 左側のO
    
    if(P == kBlackTurn && pattern_ == kNextOpenFourBlack){
      // 長連筋
      downward_influence_area->emplace_back(pattern_position_ - 2);   // 右側のX
      downward_influence_area->emplace_back(pattern_position_ + 5);   // 左側のX
    }

    return;
  }

  case kNextFourBlack:
  case kNextFourWhite:
  {
    // X[B3O2]X, [W3O2]
    const auto right_open_index = GetLessIndexOfTwo(pattern_search_index_);
    const auto left_open_index = GetGreaterIndexOfTwo(pattern_search_index_);
    
    const auto right_open_position = GetOpenBoardPosition(pattern_position_, right_open_index);
    const auto left_open_position = GetOpenBoardPosition(pattern_position_, left_open_index);

    downward_influence_area->emplace_back(right_open_position);  // [B3O2][W3O2]の右側のO
    downward_influence_area->emplace_back(left_open_position);   // [B3O2][W3O2]の左側のO

    if(P == kBlackTurn && pattern_ == kNextFourBlack){
      // 長連筋(X[B3O2]X)
      downward_influence_area->emplace_back(pattern_position_ - 1);   // 右側のX
      downward_influence_area->emplace_back(pattern_position_ + 5);   // 左側のX
    }

    return;
  }

  case kNextSemiThreeBlack:
  case kNextSemiThreeWhite:
  {
    // XO[B2O2]OX, O[W2O2]O
    const auto right_open_index = GetLessIndexOfTwo(pattern_search_index_);
    const auto left_open_index = GetGreaterIndexOfTwo(pattern_search_index_);
    
    const auto right_open_position = GetOpenBoardPosition(pattern_position_, right_open_index);
    const auto left_open_position = GetOpenBoardPosition(pattern_position_, left_open_index);

    downward_influence_area->emplace_back(right_open_position);  // [B2O2][W2O2]の右側のO
    downward_influence_area->emplace_back(left_open_position);   // [B2O2][W2O2]の左側のO
    downward_influence_area->emplace_back(pattern_position_ - 1);   // 右側のO
    downward_influence_area->emplace_back(pattern_position_ + 4);   // 左側のO

    if(P == kBlackTurn && pattern_ == kNextSemiThreeBlack){
      // 長連筋(XO[B2O2]OX)
      downward_influence_area->emplace_back(pattern_position_ - 2);   // 右側のX
      downward_influence_area->emplace_back(pattern_position_ + 5);   // 左側のX
    }

    return;
  }

  default:
    assert(false);
    return;
  }
}

template<PlayerTurn P>
inline const bool OpenState::IsInfluenceMove(const MovePosition move) const
{
  if(!IsInBoardMove(move)){
    return false;
  }

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
