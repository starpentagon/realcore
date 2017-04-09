#include "BoardOpenState.h"

using namespace std;

namespace realcore
{

BoardOpenState::BoardOpenState()
{
}

BoardOpenState::BoardOpenState(const BoardOpenState &board_open_state)
{
  *this = board_open_state;
}

bool IsEqual(const BoardOpenState &lhs, const BoardOpenState &rhs)
{
  for(const auto pattern : GetAllOpenStatePattern()){
    if(lhs.GetList(pattern) != rhs.GetList(pattern)){
      return false;
    }
  }

  return true;
}

void Copy(const BoardOpenState &from, BoardOpenState * const to)
{
  to->next_overline_ = from.GetList(kNextOverline);
  to->next_open_four_black_ = from.GetList(kNextOpenFourBlack);
  to->next_open_four_white_ = from.GetList(kNextOpenFourWhite);
  to->next_four_black_ = from.GetList(kNextFourBlack);
  to->next_four_white_ = from.GetList(kNextFourWhite);
  to->next_semi_three_black_ = from.GetList(kNextSemiThreeBlack);
  to->next_semi_three_white_ = from.GetList(kNextSemiThreeWhite);
}

}   // namespace realcore
