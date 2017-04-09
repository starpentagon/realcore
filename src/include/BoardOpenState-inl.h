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

inline void BoardOpenState::Update(const bool black_turn, const MovePosition move, const BitBoard &bit_board)
{
  if(black_turn){
    Update<kBlackTurn>(move, bit_board);
  }else{
    Update<kWhiteTurn>(move, bit_board);
  }
}

inline const std::vector<OpenState>& BoardOpenState::GetList(const OpenStatePattern pattern) const
{
  switch(pattern){
  case kNextOverline:
    return next_overline_;
  
  case kNextOpenFourBlack:
    return next_open_four_black_;
  
  case kNextOpenFourWhite:
    return next_open_four_white_;

  case kNextFourBlack:
    return next_four_black_;

  case kNextFourWhite:
    return next_four_white_;

  case kNextSemiThreeBlack:
    return next_semi_three_black_;

  case kNextSemiThreeWhite:
    return next_semi_three_white_;

  default:
    assert(false);
    static std::vector<OpenState> dummy;
    return dummy;
  }
}

inline void BoardOpenState::AddNextOverline(const BoardPosition open_position, const BoardPosition pattern_position)
{
  next_overline_.emplace_back(kNextOverline, open_position, pattern_position);
}

template<PlayerTurn P>
inline void BoardOpenState::AddNextOpenFour(const BoardPosition open_position, const BoardPosition pattern_position)
{
  if(P == kBlackTurn){
    next_open_four_black_.emplace_back(kNextOpenFourBlack, open_position, pattern_position);
  }else{
    next_open_four_white_.emplace_back(kNextOpenFourWhite, open_position, pattern_position);
  }
}

template<PlayerTurn P>
inline void BoardOpenState::AddNextFour(const BoardPosition open_position, const BoardPosition pattern_position, const BoardPosition guard_position)
{
  if(P == kBlackTurn){
    next_four_black_.emplace_back(kNextFourBlack, open_position, pattern_position);
    next_four_black_.back().SetGuardPositionList({{guard_position}});
  }else{
    next_four_white_.emplace_back(kNextFourWhite, open_position, pattern_position);
    next_four_white_.back().SetGuardPositionList({{guard_position}});
  }  
}

template<PlayerTurn P>
inline void BoardOpenState::AddNextSemiThree(const BoardPosition open_position, const BoardPosition pattern_position, const BoardPosition check_position, const GuardPositionList &guard_position_list)
{
  if(P == kBlackTurn){
    next_semi_three_black_.emplace_back(kNextSemiThreeBlack, open_position, pattern_position);
    next_semi_three_black_.back().SetCheckPosition(check_position);
    next_semi_three_black_.back().SetGuardPositionList(guard_position_list);
  }else{
    next_semi_three_white_.emplace_back(kNextSemiThreeWhite, open_position, pattern_position);
    next_semi_three_white_.back().SetGuardPositionList(guard_position_list);
  }  
}

template<PlayerTurn P>
void BoardOpenState::ClearInfluencedElement(const std::vector<OpenState> &open_state_list, const MovePosition move, std::vector<OpenState> * const cleared_open_state_list) const
{
  assert(cleared_open_state_list != nullptr);
  assert(cleared_open_state_list->empty());

  for(const auto &open_state : open_state_list){
    const bool is_influenced = open_state.IsInfluenceMove<P>(move);
    
    if(is_influenced){
      continue;
    }

    cleared_open_state_list->emplace_back(open_state);
  }
}

template<PlayerTurn P>
void BoardOpenState::AddElement(const OpenStatePattern pattern, const LineNeighborhood &line_neighbor, std::vector<OpenState> * const added_open_state_list) const
{
  assert(added_open_state_list != nullptr);

  std::vector<OpenState> open_state_list;
  line_neighbor.GetOpenState<P>(pattern, &open_state_list);

  added_open_state_list->insert(added_open_state_list->end(), open_state_list.begin(), open_state_list.end());
}

template<PlayerTurn P>
void BoardOpenState::Update(const MovePosition move, const BitBoard &bit_board)
{
  LineNeighborhood line_neighborhood(move, kOpenStateNeighborhoodSize, bit_board);

  {
    // 長連点
    constexpr OpenStatePattern kPattern = kNextOverline;

    std::vector<OpenState> updated_list;
    ClearInfluencedElement<P>(next_overline_, move, &updated_list);
    AddElement<P>(kPattern, line_neighborhood, &updated_list);

    next_overline_ = updated_list;
  }
  {
    // 達四点(黒)
    constexpr OpenStatePattern kPattern = kNextOpenFourBlack;

    std::vector<OpenState> updated_list;
    ClearInfluencedElement<P>(next_open_four_black_, move, &updated_list);
    AddElement<P>(kPattern, line_neighborhood, &updated_list);

    next_open_four_black_ = updated_list;
  }
  {
    // 達四点(白)
    constexpr OpenStatePattern kPattern = kNextOpenFourWhite;

    std::vector<OpenState> updated_list;
    ClearInfluencedElement<P>(next_open_four_white_, move, &updated_list);
    AddElement<P>(kPattern, line_neighborhood, &updated_list);

    next_open_four_white_ = updated_list;
  }
  {
    // 四ノビ点(黒)
    constexpr OpenStatePattern kPattern = kNextFourBlack;

    std::vector<OpenState> updated_list;
    ClearInfluencedElement<P>(next_four_black_, move, &updated_list);
    AddElement<P>(kPattern, line_neighborhood, &updated_list);

    next_four_black_ = updated_list;
  }
  {
    // 四ノビ点(白)
    constexpr OpenStatePattern kPattern = kNextFourWhite;

    std::vector<OpenState> updated_list;
    ClearInfluencedElement<P>(next_four_white_, move, &updated_list);
    AddElement<P>(kPattern, line_neighborhood, &updated_list);

    next_four_white_ = updated_list;
  }
  {
    // 見かけの三ノビ点(黒)
    constexpr OpenStatePattern kPattern = kNextSemiThreeBlack;

    std::vector<OpenState> updated_list;
    ClearInfluencedElement<P>(next_semi_three_black_, move, &updated_list);
    AddElement<P>(kPattern, line_neighborhood, &updated_list);

    next_semi_three_black_ = updated_list;
  }
  {
    // 見かけの三ノビ点(白)
    constexpr OpenStatePattern kPattern = kNextSemiThreeWhite;

    std::vector<OpenState> updated_list;
    ClearInfluencedElement<P>(next_semi_three_white_, move, &updated_list);
    AddElement<P>(kPattern, line_neighborhood, &updated_list);

    next_semi_three_white_ = updated_list;
  }
}

inline const bool BoardOpenState::empty() const
{
  const bool is_empty =
    next_overline_.empty() &&
    next_open_four_black_.empty() &&
    next_open_four_white_.empty() &&
    next_four_black_.empty() &&
    next_four_white_.empty() &&
    next_semi_three_black_.empty() &&
    next_semi_three_white_.empty();

  return is_empty;
}

}   // namespace realcore

#endif    // BOARD_OPEN_STATE_INL_H