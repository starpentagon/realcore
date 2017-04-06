#ifndef BOARD_OPEN_STATE_INL_H
#define BOARD_OPEN_STATE_INL_H

#include <iostream>
#include <vector>

#include "OpenState.h"
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

inline const std::vector< OpenState<kNextOverline> >& BoardOpenState::GetNextOverline() const{
  return next_overline_;
}

inline const std::vector< OpenState<kNextOpenFourBlack> >& BoardOpenState::GetNextOpenFourBlack() const{
  return next_open_four_black_;
}

inline const std::vector< OpenState<kNextOpenFourWhite> >& BoardOpenState::GetNextOpenFourWhite() const{
  return next_open_four_white_;
}

inline const std::vector< OpenState<kNextFourBlack> >& BoardOpenState::GetNextFourBlack() const{
  return next_four_black_;
}

inline const std::vector< OpenState<kNextFourWhite> >& BoardOpenState::GetNextFourWhite() const{
  return next_four_white_;
}

inline const std::vector< OpenState<kNextSemiThreeBlack> >& BoardOpenState::GetNextSemiThreeBlack() const{
  return next_semi_three_black_;
}

inline const std::vector< OpenState<kNextSemiThreeWhite> >& BoardOpenState::GetNextSemiThreeWhite() const{
  return next_semi_three_white_;
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
void BoardOpenState::AddElement(const LineNeighborhood<kOpenStateNeighborhoodSize> &line_neighbor, std::vector< OpenState<Pattern> > * const added_open_state_list) const
{
  assert(added_open_state_list != nullptr);

  std::vector< OpenState<Pattern> > open_state_list;
  line_neighbor.GetOpenState<Pattern, P>(&open_state_list);

  added_open_state_list->insert(added_open_state_list->end(), open_state_list.begin(), open_state_list.end());
}

template<PlayerTurn P>
void BoardOpenState::Update(const MovePosition move, const BitBoard &bit_board)
{
  LineNeighborhood<kOpenStateNeighborhoodSize> line_neighborhood(move, bit_board);

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


}   // namespace realcore

#endif    // BOARD_OPEN_STATE_INL_H