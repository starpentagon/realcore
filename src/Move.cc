#include "Move.h"

namespace realcore
{

const size_t CalcBoardDistance(const MovePosition from, const MovePosition to)
{
  Cordinate from_x, from_y, to_x, to_y;
  
  GetMoveCordinate(from, &from_x, &from_y);
  GetMoveCordinate(to, &to_x, &to_y);

  constexpr size_t kOutOfBoard = 225;

  if(!IsInBoard(from_x, from_y) || !IsInBoard(to_x, to_y)){
    return kOutOfBoard;
  }

  const auto x_diff = from_x - to_x;
  const auto y_diff = from_y - to_y;
  const auto abs_x_diff = abs(x_diff);
  const auto abs_y_diff = abs(y_diff);

  const size_t distance = std::max(abs_x_diff, abs_y_diff) + 15 * std::min({abs_x_diff, abs_y_diff, abs(x_diff + y_diff), abs(x_diff - y_diff)});

  return distance;
}
}   // namespace realcore
