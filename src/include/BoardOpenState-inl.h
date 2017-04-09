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

inline const std::vector< OpenState<kNextOverline> >& BoardOpenState::GetNextOverline() const{
  return next_overline_;
}

inline void BoardOpenState::AddNextOverline(const BoardPosition open_position, const BoardPosition pattern_position)
{
  next_overline_.emplace_back(open_position, pattern_position);
}

inline const std::vector< OpenState<kNextOpenFourBlack> >& BoardOpenState::GetNextOpenFourBlack() const{
  return next_open_four_black_;
}

inline const std::vector< OpenState<kNextOpenFourWhite> >& BoardOpenState::GetNextOpenFourWhite() const{
  return next_open_four_white_;
}

template<PlayerTurn P>
inline void BoardOpenState::AddNextOpenFour(const BoardPosition open_position, const BoardPosition pattern_position)
{
  if(P == kBlackTurn){
    next_open_four_black_.emplace_back(open_position, pattern_position);
  }else{
    next_open_four_white_.emplace_back(open_position, pattern_position);
  }
}

inline const std::vector< OpenState<kNextFourBlack> >& BoardOpenState::GetNextFourBlack() const{
  return next_four_black_;
}

inline const std::vector< OpenState<kNextFourWhite> >& BoardOpenState::GetNextFourWhite() const{
  return next_four_white_;
}

template<PlayerTurn P>
inline void BoardOpenState::AddNextFour(const BoardPosition open_position, const BoardPosition pattern_position, const BoardPosition guard_position)
{
  if(P == kBlackTurn){
    next_four_black_.emplace_back(open_position, pattern_position);
    next_four_black_.back().SetGuardPositionList({{guard_position}});
  }else{
    next_four_white_.emplace_back(open_position, pattern_position);
    next_four_white_.back().SetGuardPositionList({{guard_position}});
  }  
}

inline const std::vector< OpenState<kNextSemiThreeBlack> >& BoardOpenState::GetNextSemiThreeBlack() const{
  return next_semi_three_black_;
}

inline const std::vector< OpenState<kNextSemiThreeWhite> >& BoardOpenState::GetNextSemiThreeWhite() const{
  return next_semi_three_white_;
}

template<PlayerTurn P>
inline void BoardOpenState::AddNextSemiThree(const BoardPosition open_position, const BoardPosition pattern_position, const BoardPosition check_position, const GuardPositionList &guard_position_list)
{
  if(P == kBlackTurn){
    next_semi_three_black_.emplace_back(open_position, pattern_position);
    next_semi_three_black_.back().SetCheckPositionList({{check_position}});
    next_semi_three_black_.back().SetGuardPositionList(guard_position_list);
  }else{
    next_semi_three_white_.emplace_back(open_position, pattern_position);
    next_semi_three_white_.back().SetGuardPositionList(guard_position_list);
  }  
}

template<OpenStatePattern Pattern, PlayerTurn P>
void BoardOpenState::ClearInfluencedElement(const std::vector< OpenState<Pattern> > &open_state_list, const MovePosition move, std::vector< OpenState<Pattern> > * const cleared_open_state_list) const
{
  assert(cleared_open_state_list != nullptr);
  assert(cleared_open_state_list->empty());

  for(const auto &open_state : open_state_list){
    const bool is_influenced = open_state.template IsInfluenceMove<P>(move);
    
    if(is_influenced){
      continue;
    }

    cleared_open_state_list->emplace_back(open_state);
  }
}

template<OpenStatePattern Pattern, PlayerTurn P>
void BoardOpenState::AddElement(const LineNeighborhood &line_neighbor, std::vector< OpenState<Pattern> > * const added_open_state_list) const
{
  assert(added_open_state_list != nullptr);

  std::vector< OpenState<Pattern> > open_state_list;
  line_neighbor.GetOpenState<Pattern, P>(&open_state_list);

  added_open_state_list->insert(added_open_state_list->end(), open_state_list.begin(), open_state_list.end());
}

template<PlayerTurn P>
void BoardOpenState::Update(const MovePosition move, const BitBoard &bit_board)
{
  LineNeighborhood line_neighborhood(move, kOpenStateNeighborhoodSize, bit_board);

  {
    // 長連点
    constexpr OpenStatePattern kPattern = kNextOverline;

    std::vector< OpenState<kPattern> > updated_list;
    ClearInfluencedElement<kPattern, P>(next_overline_, move, &updated_list);
    AddElement<kPattern, P>(line_neighborhood, &updated_list);

    next_overline_ = updated_list;
  }
  {
    // 達四点(黒)
    constexpr OpenStatePattern kPattern = kNextOpenFourBlack;

    std::vector< OpenState<kPattern> > updated_list;
    ClearInfluencedElement<kPattern, P>(next_open_four_black_, move, &updated_list);
    AddElement<kPattern, P>(line_neighborhood, &updated_list);

    next_open_four_black_ = updated_list;
  }
  {
    // 達四点(白)
    constexpr OpenStatePattern kPattern = kNextOpenFourWhite;

    std::vector< OpenState<kPattern> > updated_list;
    ClearInfluencedElement<kPattern, P>(next_open_four_white_, move, &updated_list);
    AddElement<kPattern, P>(line_neighborhood, &updated_list);

    next_open_four_white_ = updated_list;
  }
  {
    // 四ノビ点(黒)
    constexpr OpenStatePattern kPattern = kNextFourBlack;

    std::vector< OpenState<kPattern> > updated_list;
    ClearInfluencedElement<kPattern, P>(next_four_black_, move, &updated_list);
    AddElement<kPattern, P>(line_neighborhood, &updated_list);

    next_four_black_ = updated_list;
  }
  {
    // 四ノビ点(白)
    constexpr OpenStatePattern kPattern = kNextFourWhite;

    std::vector< OpenState<kPattern> > updated_list;
    ClearInfluencedElement<kPattern, P>(next_four_white_, move, &updated_list);
    AddElement<kPattern, P>(line_neighborhood, &updated_list);

    next_four_white_ = updated_list;
  }
  {
    // 見かけの三ノビ点(黒)
    constexpr OpenStatePattern kPattern = kNextSemiThreeBlack;

    std::vector< OpenState<kPattern> > updated_list;
    ClearInfluencedElement<kPattern, P>(next_semi_three_black_, move, &updated_list);
    AddElement<kPattern, P>(line_neighborhood, &updated_list);

    next_semi_three_black_ = updated_list;
  }
  {
    // 見かけの三ノビ点(白)
    constexpr OpenStatePattern kPattern = kNextSemiThreeWhite;

    std::vector< OpenState<kPattern> > updated_list;
    ClearInfluencedElement<kPattern, P>(next_semi_three_white_, move, &updated_list);
    AddElement<kPattern, P>(line_neighborhood, &updated_list);

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