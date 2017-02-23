#include "RealCore.h"

using namespace std;

namespace realcore
{

const array<BoardDirection, kBoardDirectionNum>& GetBoardDirection(){
  static const array<BoardDirection, kBoardDirectionNum> board_direction_list{{
    kLateralDirection, kVerticalDirection, kLeftDiagonalDirection, kRightDiagonalDirection
  }};

  return board_direction_list;
}


}