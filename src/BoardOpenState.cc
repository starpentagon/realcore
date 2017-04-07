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
  if(lhs.GetNextOverline() != rhs.GetNextOverline()){
    return false;
  }

  if(lhs.GetNextOpenFourBlack() != rhs.GetNextOpenFourBlack()){
    return false;
  }

  if(lhs.GetNextOpenFourWhite() != rhs.GetNextOpenFourWhite()){
    return false;
  }

  if(lhs.GetNextFourBlack() != rhs.GetNextFourBlack()){
    return false;
  }

  if(lhs.GetNextFourWhite() != rhs.GetNextFourWhite()){
    return false;
  }

  if(lhs.GetNextSemiThreeBlack() != rhs.GetNextSemiThreeBlack()){
    return false;
  }

  if(lhs.GetNextSemiThreeWhite() != rhs.GetNextSemiThreeWhite()){
    return false;
  }

  return true;
}

void Copy(const BoardOpenState &from, BoardOpenState * const to)
{
  to->next_overline_ = from.GetNextOverline();
  to->next_open_four_black_ = from.GetNextOpenFourBlack();
  to->next_open_four_white_ = from.GetNextOpenFourWhite();
  to->next_four_black_ = from.GetNextFourBlack();
  to->next_four_white_ = from.GetNextFourWhite();
  to->next_semi_three_black_ = from.GetNextSemiThreeBlack();
  to->next_semi_three_white_ = from.GetNextSemiThreeWhite();
}

}   // namespace realcore
