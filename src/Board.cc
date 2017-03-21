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
    "", "Ａ", "Ｂ", "Ｃ", "Ｄ", "Ｅ", "Ｆ", "Ｇ", "Ｈ", "Ｉ", "Ｊ", "Ｋ", "Ｌ", "Ｍ", "Ｎ", "Ｏ"
  }};

  stringstream ss;

  // x座標
  ss << "　";
  for(Cordinate x=1; x<=kBoardLineNum; ++x){
    ss << cordinate_str[x];
  }

  ss << endl;

  for(Cordinate y=1; y<=kBoardLineNum; ++y){
    ss << cordinate_str[y];

    for(Cordinate x=1; x<=kBoardLineNum; ++x){      
      const MovePosition move = GetMove(x, y);
      const PositionState state = GetState(move);

      string state_str = "＋";

      if(state == kBlackStone){
        state_str = "●";
      }else if(state == kWhiteStone){
        state_str = "○";
      }else{
        // 盤端
        if(x == 1 && y == 1){
          state_str = "┏";
        }else if(x == kBoardLineNum && y == 1){
          state_str = "┓";
        }else if(x == 1 && y == kBoardLineNum){
          state_str = "┗";
        }else if(x == kBoardLineNum && y == kBoardLineNum){
          state_str = "┛";
        }else if(x == 1 || x == kBoardLineNum){
          state_str = "┃";
        }else if(y == 1 || y == kBoardLineNum){
          state_str = "ー";
        }
      }

      ss << state_str;
    }

    ss << endl;
  }

  return ss.str();
}

}   // namespace realcore
