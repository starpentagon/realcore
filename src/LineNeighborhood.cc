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

  LocalBitBoard search_bit{{0}}, open_state_bit{{0}};

  for(size_t i=0; i<kLocalBitBoardNum; i++){
    search_bit[i] = SearchNextOverline(stone_bit[i], open_bit[i], &open_state_bit[i]);
  }

  if(open_state_bit[0] == 0 && open_state_bit[1] == 0){
    return;
  }

  vector<BoardPosition> open_state_position_list, pattern_position_list;

  GetBoardPositionList(open_state_bit, &open_state_position_list);  
  GetBoardPositionList(search_bit, &pattern_position_list);

  constexpr size_t kPatternLength = 6;    //!< パターン長
  constexpr size_t kOpenCount = 1;        //!< 設定する空点状態の数
  constexpr size_t kMaxSearchBitCount = (2 * kOpenStateNeighborhoodSize + 1) - kPatternLength + 1;
  constexpr size_t kListSize = 4 * kOpenCount * kMaxSearchBitCount;
  open_state_list->reserve(kListSize);

  // 空点位置×パターン位置ごとにOpenStateオブジェクトを設定する
  for(const auto pattern_position : pattern_position_list){
    const auto pattern_direction = realcore::GetBoardDirection(pattern_position);
    for(const auto open_state_position : open_state_position_list){
      const auto open_state_direction = realcore::GetBoardDirection(open_state_position);

      if(open_state_direction != pattern_direction){
        continue;
      }

      open_state_list->emplace_back(open_state_position, pattern_position);
    }
  }
}

template<>
template<>
void LineNeighborhood<kOpenStateNeighborhoodSize>::GetOpenState<kNextOpenFourBlack, kBlackTurn>(const LocalBitBoard &stone_bit, const LocalBitBoard &open_bit, vector< OpenState<kNextOpenFourBlack> > *open_state_list) const
{
  assert(open_state_list != nullptr);
  assert(open_state_list->empty());

  GetOpenStateOpenFour<kNextOpenFourBlack, kBlackTurn>(stone_bit, open_bit, open_state_list);
}

}   // namesapce realcore

