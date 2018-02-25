#ifndef MOVE_INL_H
#define MOVE_INL_H

#include <cassert>
#include <array>
#include <algorithm>
#include <iostream>

#include "Conversion.h"
#include "Move.h"

namespace realcore
{

inline void GetMoveCordinate(const MovePosition move, Cordinate * const x, Cordinate * const y)
{
  assert(x != nullptr && y != nullptr);

  *x = move % 16;
  *y = move / 16;
}

inline const std::array<MovePosition, kMoveNum>& GetAllMove()
{
  static const std::array<MovePosition, kMoveNum> all_move_list{
      {
        #include "def/MoveDefinition.h"
      }};

  return all_move_list;
}

inline const std::array<MovePosition, kValidMoveNum>& GetAllValidMove()
{
  static const std::array<MovePosition, kValidMoveNum> all_valid_move_list{
      {
        #include "def/InBoardMoveList.h"
        kNullMove
      }};

  return all_valid_move_list;
}

inline const std::array<MovePosition, kInBoardMoveNum>& GetAllInBoardMove()
{
  static const std::array<MovePosition, kInBoardMoveNum> all_in_board_move_list{
      {
        #include "def/InBoardMoveList.h"
      }};

  return all_in_board_move_list;
}

inline const bool IsInvalidMove(const MovePosition move)
{
  return move == kInvalidMove;
}

inline const bool IsNullMove(const MovePosition move)
{
  return move == kNullMove;
}

constexpr inline bool IsInBoard(const Cordinate x, const Cordinate y)
{
  assert(0 <= x && x <= static_cast<Cordinate>(kBoardLineNum));
  assert(0 <= y && y <= static_cast<Cordinate>(kBoardLineNum));

  const bool x_in_board = x != 0;
  const bool y_in_board = y != 0;

  return x_in_board && y_in_board;
}

inline const bool IsInBoardMove(const MovePosition move)
{
  Cordinate x = 0, y = 0;
  GetMoveCordinate(move, &x, &y);

  return IsInBoard(x, y);
}

inline const bool IsValidMove(const MovePosition move)
{
  return IsInBoardMove(move) || IsNullMove(move);
}

inline const MovePosition GetMove(const Cordinate x, const Cordinate y)
{
  assert(IsInBoard(x, y));
  
  const MovePosition move = static_cast<MovePosition>(16 * y + x);
  return move;
}

inline const MovePosition GetSymmetricMove(const MovePosition move, const BoardSymmetry symmetry)
{
  if(!IsInBoardMove(move)){
    return move;
  }

  Cordinate move_x = 0, move_y = 0;
  GetMoveCordinate(move, &move_x, &move_y);

  constexpr int kCordinateCenter = 8;
  const int x = static_cast<int>(move_x) - kCordinateCenter;
  const int y = static_cast<int>(move_y) - kCordinateCenter;

  int symmetric_x = 0, symmetric_y = 0;
  
  switch(symmetry){
  case kIdenticalSymmetry:
    symmetric_x = x;
    symmetric_y = y;
    break;

  case kHorizontalSymmetry:
    symmetric_x = -x;
    symmetric_y = y;
    break;

  case kVerticalSymmetry:
    symmetric_x = x;
    symmetric_y = -y;
    break;

  case kCentricSymmetry:
    symmetric_x = -x;
    symmetric_y = -y;
    break;

  case kDiagonalSymmetry1:
    symmetric_x = y;
    symmetric_y = x;
    break;

  case kDiagonalSymmetry2:
    symmetric_x = -y;
    symmetric_y = x;
    break;

  case kDiagonalSymmetry3:
    symmetric_x = y;
    symmetric_y = -x;
    break;

  case kDiagonalSymmetry4:
    symmetric_x = -y;
    symmetric_y = -x;
    break;
  }

  return GetMove(symmetric_x + kCordinateCenter, symmetric_y + kCordinateCenter);
}

inline std::string MoveString(const MovePosition move)
{
  static const std::array<std::string, kBoardLineNum + 1> kCordinateStr{{
    "", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o"
  }};

  static const std::string kPassStr = "pp";

  if(!IsValidMove(move)){
    return "--";
  }

  if(!IsNullMove(move)){
    Cordinate x = 0, y = 0;
    GetMoveCordinate(move, &x, &y);

    std::string move_str(kCordinateStr[x]);
    move_str += kCordinateStr[y];

    return move_str;
  }else{
    return kPassStr;
  }
}

inline void AscendingSort(std::vector<MoveValue> * const move_value_list)
{
  assert(move_value_list != nullptr);
  stable_sort(move_value_list->begin(), move_value_list->end(), 
    [](const MoveValue &data1, const MoveValue &data2){return data1.second < data2.second;});
}

inline void DescendingSort(std::vector<MoveValue> * const move_value_list)
{
  assert(move_value_list != nullptr);
  stable_sort(move_value_list->begin(), move_value_list->end(), 
    [](const MoveValue &data1, const MoveValue &data2){return data1.second > data2.second;});
}

template<size_t L>
inline const MoveBitSet& GetLineNeighborhoodBit(const MovePosition move)
{
  static std::bitset<kMoveNum> generated_flag;
  static std::array<MoveBitSet, kMoveNum> line_neighborhood_bit;
  
  if(generated_flag[move]){
    return line_neighborhood_bit[move];
  }

  MoveBitSet &move_bit_set = line_neighborhood_bit[move];

  move_bit_set.set(move);

  for(const auto direction : GetBoardDirection()){
    const auto move_board_position = GetBoardPosition(move, direction);

    for(size_t i=1; i<=L; i++){
      const BoardPosition board_position = move_board_position + i;
      const auto neighbor_move = GetBoardMove(board_position);
      
      if(!IsInBoardMove(neighbor_move)){
        break;
      }

      move_bit_set.set(neighbor_move);
    }

    for(size_t i=1; i<=L; i++){
      const BoardPosition board_position = move_board_position - i;
      const auto neighbor_move = GetBoardMove(board_position);
      
      if(!IsInBoardMove(neighbor_move)){
        break;
      }

      move_bit_set.set(neighbor_move);
    }
  }

  generated_flag.set(move);
  return line_neighborhood_bit[move];
}
}   // realcore

#endif    // MOVE_INL_H