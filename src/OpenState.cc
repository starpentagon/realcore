#include "OpenState.h"

using namespace std;

namespace realcore
{

OpenState::OpenState(const OpenStatePattern pattern, const BoardPosition open_position, const BoardPosition pattern_position, const size_t pattern_search_index)
: pattern_(pattern), open_position_(open_position), pattern_position_(pattern_position), pattern_search_index_(pattern_search_index)
{
}

OpenState::OpenState(const OpenState &open_state)
{
  *this = open_state;
}

void OpenState::GetFourPosition(array<BoardPosition, 2> * const four_position_list) const
{
  assert(pattern_ == kNextPointOfSwordBlack || pattern_ == kNextPointOfSwordWhite);
  assert(four_position_list != nullptr);

  const array<size_t, 3> open_index_list{{
    GetMinIndexOfThree(pattern_search_index_),
    GetMedianIndexOfThree(pattern_search_index_),
    GetMaxIndexOfThree(pattern_search_index_),
  }};
  size_t index = 0;

  for(const auto open_index : open_index_list){
    const auto open_position = GetOpenBoardPosition(pattern_position_, open_index);

    if(open_position != open_position_){
      (*four_position_list)[index++] = open_position;
    }
  }
}

void OpenState::GetSemiThreePosition(array<BoardPosition, 2> * const semi_three_position_list) const
{
  assert(pattern_ == kNextTwoBlack || pattern_ == kNextTwoWhite);
  assert(semi_three_position_list != nullptr);

  const array<size_t, 3> open_index_list{{
    GetMinIndexOfThree(pattern_search_index_),
    GetMedianIndexOfThree(pattern_search_index_),
    GetMaxIndexOfThree(pattern_search_index_),
  }};
  size_t index = 0;

  for(const auto open_index : open_index_list){
    const auto open_position = GetOpenBoardPosition(pattern_position_, open_index);

    if(open_position != open_position_){
      (*semi_three_position_list)[index++] = open_position;
    }
  }
}

void OpenState::GetStonePosition(std::vector<BoardPosition> * const stone_position_list) const
{
  assert(pattern_ == kNextFourBlack || pattern_ == kNextFourWhite || 
    pattern_ == kNextPointOfSwordBlack || pattern_ == kNextPointOfSwordWhite);
  assert(stone_position_list != nullptr);
  
  if(pattern_ == kNextFourBlack || pattern_ == kNextFourWhite)
  {
    // 四ノビ点([B302][W3O2])
    const auto open_index_1 = GetLessIndexOfTwo(pattern_search_index_);
    const auto open_index_2 = GetGreaterIndexOfTwo(pattern_search_index_);

    const auto open_position_1 = GetOpenBoardPosition(pattern_position_, open_index_1);
    const auto open_position_2 = GetOpenBoardPosition(pattern_position_, open_index_2);

    for(BoardPosition board_position=pattern_position_; board_position<pattern_position_+5; board_position++){
      if(board_position == open_position_1 || board_position == open_position_2){
        continue;
      }

      stone_position_list->emplace_back(board_position);
    }
  }else{
    // 剣先点([B203][W2O3])
    array<BoardPosition, 2> open_position_list;
    GetFourPosition(&open_position_list);

    for(BoardPosition board_position=pattern_position_; board_position<pattern_position_+5; board_position++){
      if(board_position == open_position_ || board_position == open_position_list[0] || board_position == open_position_list[1]){
        continue;
      }

      stone_position_list->emplace_back(board_position);
    }
  }
}

bool IsEqual(const OpenState &open_state_1, const OpenState &open_state_2)
{
  if(open_state_1.pattern_ != open_state_2.pattern_){
    return false;
  }
  
  if(open_state_1.open_position_ != open_state_2.open_position_){
    return false;
  }

  if(open_state_1.pattern_position_ != open_state_2.pattern_position_){
    return false;
  }

  if(open_state_1.pattern_search_index_ != open_state_2.pattern_search_index_){
    return false;
  }

  return true;
}

void Copy(const OpenState &from, OpenState * const to)
{
  to->pattern_ = from.pattern_;
  to->open_position_ = from.open_position_;
  to->pattern_position_ = from.pattern_position_;
  to->pattern_search_index_ = from.pattern_search_index_;
}

}

