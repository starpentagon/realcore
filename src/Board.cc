#include <sstream>

#include "Move.h"
#include "MoveList.h"
#include "Board.h"

using namespace std;

namespace realcore
{

Board::Board()
: bit_board_{{
  #include "def/BitBoardDefinition.h"
  }}
{
}

Board::Board(const MoveList &move_list)
: bit_board_{{
  #include "def/BitBoardDefinition.h"
  }}
{
  bool is_black_turn = true;

  for(const auto move : move_list){
    if(is_black_turn){
      SetState<kBlackStone>(move);
    }else{
      SetState<kWhiteStone>(move);
    }

    is_black_turn = !is_black_turn;
  }
}

const string Board::str() const
{
  static array<string, kBoardLineNum + 1> cordinate_str{{
    "", "A ", "B ", "C ", "D ", "E ", "F ", "G ", "H ", "I ", "J ", "K ", "L ", "M ", "N ", "O "
  }};

  stringstream ss;

  // x座標
  ss << "  ";
  for(Cordinate x=1; x<=kBoardLineNum; ++x){
    ss << cordinate_str[x];
  }

  ss << endl;

  for(Cordinate y=1; y<=kBoardLineNum; ++y){
    ss << cordinate_str[y];

    for(Cordinate x=1; x<=kBoardLineNum; ++x){      
      const MovePosition move = GetMove(x, y);
      const PositionState state = GetState(move);

      string state_str = ". ";

      if(state == kBlackStone){
        state_str = "x ";
      }else if(state == kWhiteStone){
        state_str = "o ";
      }else{
        // 盤端
        bool is_corner = (x == 1 && y == 1) || (x == kBoardLineNum && y == 1);
        is_corner |= (x == 1 && y == kBoardLineNum) || (x == kBoardLineNum && y == kBoardLineNum);

        bool is_star = (x == 4 && y == 4) || (x == 8 && y == 8) || (x == 12 && y == 4);
        is_star |= (x == 4 && y == 12) || (x == 12 && y == 12);

        if(is_corner){
          state_str = "+ ";
        }else if(x == 1 || x == kBoardLineNum){
          state_str = "| ";
        }else if(y == 1 || y == kBoardLineNum){
          state_str = "--";
        }else if(is_star){
          state_str = "* ";
        }
      }

      ss << state_str;
    }

    ss << cordinate_str[y];
    ss << endl;
  }

  // x座標
  ss << "  ";
  for(Cordinate x=1; x<=kBoardLineNum; ++x){
    ss << cordinate_str[x];
  }

  ss << endl;
  return ss.str();
}

}   // namespace realcore
