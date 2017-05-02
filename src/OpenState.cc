#include "OpenState.h"

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

