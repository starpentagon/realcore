#include <array>
#include <iostream>
#include <fstream>

#include "Move.h"
#include "MoveList.h"

using namespace std;

namespace realcore
{

MoveList::MoveList()
{
  ReserveInitial(0);
}

MoveList::MoveList(const MoveList &move_list)
{
  *this = move_list;
}

MoveList::MoveList(const MovePosition move)
{
  *this = move;
}

MoveList::MoveList(const string &move_str)
{
  GetMoveList(move_str, this);
}

void MoveList::ReserveInitial(const size_t initial_list_size)
{
  const size_t reserve_size = CalcInitialReserveSize(initial_list_size);
  move_list_.reserve(reserve_size);
}

string MoveList::str() const
{
  string move_str;

  for(const auto move : move_list_)
  {
    move_str += MoveString(move);
  }

  return move_str;
}

string MoveList::GetSGFPositionText() const
{
  MoveList black_list, white_list;
  bool is_black_turn = true;

  for(const auto move : move_list_){
    if(is_black_turn){
      black_list += move;
    }else{
      white_list += move;
    }

    is_black_turn = !is_black_turn;
  }

  string sgf_str;

  if(!black_list.empty()){
    string sgf_black_str("AB");

    for(const auto move : black_list){
      if(move == kNullMove){
        continue;
      }

      sgf_black_str += "[" + MoveString(move) + "]";
    }

    sgf_str += sgf_black_str;
  }

  if(!white_list.empty()){
    string sgf_white_str("AW");

    for(const auto move : white_list){
      if(move == kNullMove){
        continue;
      }

      sgf_white_str += "[" + MoveString(move) + "]";
    }

    sgf_str += sgf_white_str;
  }

  return sgf_str;
}

void MoveList::GetOpenMove(MoveBitSet * const open_move_bit) const
{
  assert(open_move_bit != nullptr);
  assert(open_move_bit->none());

  for(const auto move : move_list_){
    open_move_bit->set(move);
  }

  open_move_bit->flip();
  *open_move_bit &= GetInBoardMoveBitSet();
}

void MoveList::GetOpenMove(MoveList * const open_move_list) const
{
  assert(open_move_list != nullptr);
  assert(open_move_list->empty());

  MoveBitSet move_bit;
  GetOpenMove(&move_bit);

  GetMoveList(move_bit, open_move_list);
}

void MoveList::GetOpenMove(const MoveBitSet &forbidden_bit, MoveBitSet * const open_move_bit) const
{
  GetOpenMove(open_move_bit);
  *open_move_bit &= ~forbidden_bit;
}
void MoveList::GetOpenMove(const MoveBitSet &forbidden_bit, MoveList * const open_move_list) const
{
  assert(open_move_list != nullptr);
  assert(open_move_list->empty());

  MoveBitSet move_bit;
  GetOpenMove(forbidden_bit, &move_bit);

  GetMoveList(move_bit, open_move_list);
}

void MoveList::GetPossibleMove(MoveBitSet * const possible_move_bit) const
{
  assert(possible_move_bit != nullptr);

  GetOpenMove(possible_move_bit);
  possible_move_bit->set(kNullMove);
}

void MoveList::GetPossibleMove(MoveList * const possible_move_list) const
{
  GetOpenMove(possible_move_list);
  (*possible_move_list) += kNullMove;
}

void MoveList::GetPossibleMove(const MoveBitSet &forbidden_bit, MoveBitSet * const possible_move_bit) const
{
  GetPossibleMove(possible_move_bit);
  *possible_move_bit &= ~forbidden_bit;
}

void MoveList::GetPossibleMove(const MoveBitSet &forbidden_bit, MoveList * const possible_move_list) const
{
  GetOpenMove(forbidden_bit, possible_move_list);
  (*possible_move_list) += kNullMove;
}

bool GetMoveList(const string &move_string, MoveList * const move_list)
{
  assert(move_list != nullptr && move_list->empty());
  
  const size_t str_length = move_string.size();

  // 2文字 * nになっているかチェックする
  if(str_length % 2 == 1){
    return false;
  }

  const size_t initial_move_length = str_length / 2;
  move_list->ReserveInitial(initial_move_length);

  bool valid_str_format = true;   // 与えられた文字列が[a-o][a-o]形式かのフラグ

  for(size_t i=0; i<str_length; i+=2){
    const char first_char = move_string[i];
    const char second_char = move_string[i + 1];

    if(first_char == 'p' && second_char == 'p'){
      *move_list += kNullMove;
      continue;
    }

    Cordinate x = first_char - 'a' + 1;
    Cordinate y = second_char - 'a' + 1;
    
    const bool x_in_board = 1 <= x && x <= static_cast<Cordinate>(kBoardLineNum);
    const bool y_in_board = 1 <= y && y <= static_cast<Cordinate>(kBoardLineNum);

    if(!(x_in_board && y_in_board))
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

void GetSymmetricMoveList(const MoveList &move_list, const BoardSymmetry symmetry, MoveList * const symmetric_move_list)
{
  assert(symmetric_move_list != nullptr);
  assert(symmetric_move_list->empty());

  for(const auto move : move_list){
    const auto symmetric_move = GetSymmetricMove(move, symmetry);
    (*symmetric_move_list) += symmetric_move;
  }
}

void SortByNearMove(const MovePosition move, MoveList * const move_list)
{
  assert(move_list != nullptr);
  vector<MoveValue> move_distance_list;
  move_distance_list.reserve(move_list->size());

  for(const auto move_to : *move_list){
    const auto distance = CalcBoardDistance(move, move_to);
    move_distance_list.emplace_back(move_to, distance);
  }

  AscendingSort(&move_distance_list);

  move_list->clear();
  
  for(const auto &near_move : move_distance_list){
    *move_list += near_move.first;
  }
}

void ReadPOSFile(const std::string &pos_file_path, MoveList * const move_list)
{
  assert(move_list != nullptr);
  assert(move_list->empty());

  ifstream pos_file(pos_file_path, ios::in | ios::binary);

  if(!pos_file){
    cerr << "Failed to open pos file: " << pos_file_path << endl;
    return;
  }

  pos_file.seekg(0, ios::end);
  const size_t size = pos_file.tellg();
  pos_file.seekg(0, ios::beg);

  char *buffer = new char[size];
  pos_file.read(buffer, size);

  for(size_t i=1; i<size; i++){
    const unsigned char val = static_cast<unsigned char>(buffer[i]);
    const Cordinate x = static_cast<Cordinate>(val % kBoardLineNum + 1);
    const Cordinate y = static_cast<Cordinate>(val / kBoardLineNum + 1);

    const auto move = GetMove(x, y);
    *move_list += move;
  }

  delete [] buffer;
}

}   // namespace realcore