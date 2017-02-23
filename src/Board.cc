#include "Move.h"
#include "Board.h"

namespace realcore
{

Board::Board()
{
}

template<>
void Board::SetState<kOverBoard>(const MovePosition move)
{
  assert(false);
}

template<>
void Board::SetState<kBlackStone>(const MovePosition move)
{

}

template<>
void Board::SetState<kWhiteStone>(const MovePosition move)
{

}

template<>
void Board::SetState<kOpenPosition>(const MovePosition move)
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
