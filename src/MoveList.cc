#include "Board.h"
#include "MoveList.h"

using namespace std;

namespace realcore
{

MoveList::MoveList()
{
  const size_t reserve_size = CalcInitialReserveSize(0);
  move_list_.reserve(reserve_size);
}

MoveList::MoveList(const MoveList &move_list)
{
  *this = move_list;
}

MoveList::MoveList(const MovePosition move)
{
  *this = move;
}

const size_t MoveList::CalcInitialReserveSize(const size_t initial_list_size) const
{
  // 領域の再確保を抑制するため初期化時のリストより長めの領域を確保しておく
  size_t reserve_list_size = initial_list_size + 16;
  reserve_list_size += 8 - (reserve_list_size % 8);   // 「8 - (8で割った余り)」を加算して8の倍数にする

  return reserve_list_size;
}

string MoveList::str() const
{
  static const array<string, kBoardLineNum + 1> kCordinateStr{{
    "", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o"
  }};

  static const string kPassStr = "pp";
  string move_str;

  for(const auto move : move_list_)
  {
    assert(IsValidMove(move));

    if(!IsNullMove(move)){
      Cordinate x = 0, y = 0;
      GetMoveCordinate(move, &x, &y);

      move_str += kCordinateStr[x];
      move_str += kCordinateStr[y];
    }else{
      move_str += kPassStr;
    }
  }

  return move_str;
}

bool GetMoveList(const string &move_string, MoveList *move_list)
{
  assert(move_list != nullptr && move_list->empty());
  
  const size_t str_length = move_string.size();

  // 2文字 * nになっているかチェックする
  if(str_length % 2 == 1){
    return false;
  }

  bool valid_str_format = true;   // 与えられた文字列が[a-o][a-o]形式かのフラグ

  for(size_t i=0; i<str_length; i+=2){
    const char first_char = move_string[i];
    const char second_char = move_string[i + 1];

    if(first_char == 'p' && second_char == 'p'){
      *move_list += kNullMove;
      continue;
    }

    const Cordinate x = first_char - 'a' + 1;
    const Cordinate y = second_char - 'a' + 1;

    if(!IsInBoard(x, y))
    {
      valid_str_format = false;
      break;
    }

    const MovePosition move = GetMove(x, y);
    *move_list += move;
  }

  if(!valid_str_format){
    move_list->clear();
  }

  return valid_str_format;
}

}   // namespace realcore