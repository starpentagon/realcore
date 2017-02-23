#include "Move.h"
#include "Board.h"

namespace realcore
{

Board::Board()
: bit_board_{{
  #include "def/BitBoardDefinition.h"
  }}
{
}

void Board::SetState(const MovePosition move, const PositionState state)
{
  switch(state){
  case kOverBoard:
    SetState<kOverBoard>(move);
    break;
  
  case kBlackStone:
    SetState<kBlackStone>(move);
    break;

  case kWhiteStone:
    SetState<kWhiteStone>(move);
    break;

  case kOpenPosition:
    SetState<kOpenPosition>(move);
    break;
  }
}

}   // namespace realcore
