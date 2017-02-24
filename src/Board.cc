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

}   // namespace realcore
