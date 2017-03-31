#include <sstream>
#include <bitset>

#include "MoveList.h"
#include "LineNeighborhood.h"
#include "BitBoard.h"

using namespace std;

namespace realcore
{

BitBoard::BitBoard()
: bit_board_{{
  #include "def/BitBoardDefinition.h"
  }}
{
}

BitBoard::BitBoard(const BitBoard &bit_board)
{
  *this = bit_board;
}

BitBoard::BitBoard(const MoveList &move_list)
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

bool IsEqual(const BitBoard &bit_board_1, const BitBoard &bit_board_2)
{
  if(bit_board_1.bit_board_ != bit_board_2.bit_board_){
    return false;
  }

  return true;
}

const bool BitBoard::operator==(const BitBoard &bit_board) const{
  return IsEqual(*this, bit_board);
}

const bool BitBoard::operator!=(const BitBoard &bit_board) const{
  return !(*this == bit_board);
}

void Copy(const BitBoard &bit_board_from, BitBoard * const bit_board_to)
{
  assert(bit_board_to != nullptr);

  bit_board_to->bit_board_ = bit_board_from.bit_board_;
}

const BitBoard& BitBoard::operator=(const BitBoard &bit_board)
{
  if(this != &bit_board){
    Copy(bit_board, this);
  }

  return *this;
}

template<>
const bool BitBoard::IsForbiddenMove<kBlackTurn>(const MovePosition move) const
{
  if(!IsInBoardMove(move)){
    return false;
  }
  
  assert(GetState(move) == kOpenPosition);

  // 禁手チェックはmoveの長さ5の直線近傍をチェックすれば十分
  // @see doc/06_forbidden_check/forbidden_check.pptx
  constexpr size_t kForbiddenCheck = 5;
  LineNeighborhood<kForbiddenCheck> line_neighbor(move, *this);

  line_neighbor.SetCenterState<kBlackStone>();

  // 直線近傍の禁手チェック
  vector<BoardPosition> next_open_four_list;
  const ForbiddenCheckState forbidden_state = line_neighbor.ForbiddenCheck(&next_open_four_list);

  if(forbidden_state == kForbiddenMove){
    return true;
  }else if(forbidden_state == kNonForbiddenMove){
    return false;
  }

  // 見かけの三々が存在する(kPossibleForbiddenMove)
  // 達四を作る位置が禁手かどうかを再帰的にチェックする
  BitBoard board(*this);
  board.SetState<kBlackStone>(move);

  size_t three_count = 0;
  bitset<kBoardDirectionNum> checked_direction;
  
  for(const auto board_position : next_open_four_list){
    const auto direction = GetBoardDirection(board_position);

    if(checked_direction[direction]){
      continue;
    }

    const MovePosition next_open_four_move = GetBoardMove(board_position);
    const bool is_forbidden = board.IsForbiddenMove<kBlackTurn>(next_open_four_move);

    if(!is_forbidden){
      // 達四を作る位置が禁点でなければ三
      three_count++;

      if(three_count == 2){
        return true;
      }

      checked_direction.set(direction);
    }
  }

  return false;
}

template<>
const bool BitBoard::IsForbiddenMove<kWhiteTurn>(const MovePosition move) const
{
  // 白番に禁手はない
  return false;
}

const string BitBoard::str() const
{
  static array<string, kBoardLineNum + 1> cordinate_str{{
    "", "A ", "B ", "C ", "D ", "E ", "F ", "G ", "H ", "I ", "J ", "K ", "L ", "M ", "N ", "O "
  }};

  stringstream ss;

  // x座標
  ss << "  ";
  for(Cordinate x=1; x<=static_cast<Cordinate>(kBoardLineNum); ++x){
    ss << cordinate_str[x];
  }

  ss << endl;

  for(Cordinate y=1; y<=static_cast<Cordinate>(kBoardLineNum); ++y){
    ss << cordinate_str[y];

    for(Cordinate x=1; x<=static_cast<Cordinate>(kBoardLineNum); ++x){      
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
  for(Cordinate x=1; x<=static_cast<Cordinate>(kBoardLineNum); ++x){
    ss << cordinate_str[x];
  }

  ss << endl;
  return ss.str();
}

}   // namespace realcore
