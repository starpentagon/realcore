#include <iostream>

#include "OpenState.h"
#include "LineNeighborhood.h"

using namespace std;

namespace realcore
{

template<>
template<>
void LineNeighborhood<kOpenStateNeighborhoodSize>::GetOpenState<kNextOverline, kBlackTurn>(const LocalBitBoard &stone_bit, const LocalBitBoard &open_bit, vector< OpenState<kNextOverline> > *open_state_list) const
{
  assert(open_state_list != nullptr);
  assert(open_state_list->empty());

  array<LocalBitBoard, kFourStonePattern> pattern_search_bit;
  array<LocalBitBoard, kFourStonePattern> open_state_bit;

  for(size_t i=0; i<kLocalBitBoardNum; i++){
    array<uint64_t, kFourStonePattern> pattern_search_bit_list{{0}};
    SearchNextOverline(stone_bit[i], open_bit[i], &pattern_search_bit_list);

    for(size_t index=0; index<kFourStonePattern; index++){
      pattern_search_bit[index][i] = pattern_search_bit_list[index];
      open_state_bit[index][i] = GetOpenBitInPattern(index, pattern_search_bit_list[index]);
    }
  }

  constexpr size_t kPatternLength = 6;    //!< パターン長
  constexpr size_t kOpenCount = 1;        //!< 設定する空点状態の数
  constexpr size_t kMaxSearchBitCount = (2 * kOpenStateNeighborhoodSize + 1) - kPatternLength + 1;
  constexpr size_t kListSize = 4 * kOpenCount * kMaxSearchBitCount;
  open_state_list->reserve(kListSize);

  for(size_t index=0; index<kFourStonePattern; index++){
    vector<BoardPosition> open_state_position_list, pattern_position_list;

    GetBoardPositionList(pattern_search_bit[index], &pattern_position_list);
    GetBoardPositionList(open_state_bit[index], &open_state_position_list);

    assert(pattern_position_list.size() == open_state_position_list.size());

    for(size_t i=0, size=open_state_position_list.size(); i<size; i++){
      const auto open_state_position = open_state_position_list[i];
      const auto pattern_position = pattern_position_list[i];

      open_state_list->emplace_back(open_state_position, pattern_position);
    }
  }
}

template<>
template<>
void LineNeighborhood<kOpenStateNeighborhoodSize>::GetOpenState<kNextOpenFourBlack, kBlackTurn>(const LocalBitBoard &stone_bit, const LocalBitBoard &open_bit, vector< OpenState<kNextOpenFourBlack> > *open_state_list) const
{
  GetOpenStateOpenFour<kNextOpenFourBlack, kBlackTurn>(stone_bit, open_bit, open_state_list);
}

template<>
template<>
void LineNeighborhood<kOpenStateNeighborhoodSize>::GetOpenState<kNextOpenFourWhite, kWhiteTurn>(const LocalBitBoard &stone_bit, const LocalBitBoard &open_bit, vector< OpenState<kNextOpenFourWhite> > *open_state_list) const
{
  GetOpenStateOpenFour<kNextOpenFourWhite, kWhiteTurn>(stone_bit, open_bit, open_state_list);
}

template<>
template<>
void LineNeighborhood<kOpenStateNeighborhoodSize>::GetOpenState<kNextFourBlack, kBlackTurn>(const LocalBitBoard &stone_bit, const LocalBitBoard &open_bit, vector< OpenState<kNextFourBlack> > *open_state_list) const
{
  GetOpenStateFour<kNextFourBlack, kBlackTurn>(stone_bit, open_bit, open_state_list);
}

template<>
template<>
void LineNeighborhood<kOpenStateNeighborhoodSize>::GetOpenState<kNextFourWhite, kWhiteTurn>(const LocalBitBoard &stone_bit, const LocalBitBoard &open_bit, vector< OpenState<kNextFourWhite> > *open_state_list) const
{
  GetOpenStateFour<kNextFourWhite, kWhiteTurn>(stone_bit, open_bit, open_state_list);
}

template<>
template<>
void LineNeighborhood<kOpenStateNeighborhoodSize>::GetOpenState<kNextSemiThreeBlack, kBlackTurn>(const LocalBitBoard &stone_bit, const LocalBitBoard &open_bit, vector< OpenState<kNextSemiThreeBlack> > *open_state_list) const
{
  GetOpenStateSemiThree<kNextSemiThreeBlack, kBlackTurn>(stone_bit, open_bit, open_state_list);
}

template<>
template<>
void LineNeighborhood<kOpenStateNeighborhoodSize>::GetOpenState<kNextSemiThreeWhite, kWhiteTurn>(const LocalBitBoard &stone_bit, const LocalBitBoard &open_bit, vector< OpenState<kNextSemiThreeWhite> > *open_state_list) const
{
  GetOpenStateSemiThree<kNextSemiThreeWhite, kWhiteTurn>(stone_bit, open_bit, open_state_list);
}

}   // namesapce realcore

