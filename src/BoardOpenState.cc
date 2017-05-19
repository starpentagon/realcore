#include "BoardOpenState.h"

using namespace std;

namespace realcore
{

BoardOpenState::BoardOpenState()
: update_flag_(kUpdateAllOpenState)
{
}

BoardOpenState::BoardOpenState(const UpdateOpenStateFlag &update_flag)
: update_flag_(update_flag)
{
}

BoardOpenState::BoardOpenState(const BoardOpenState &board_open_state)
{
  *this = board_open_state;
}

BoardOpenState::BoardOpenState(const BoardOpenState &board_open_state, const bool is_black_turn, const MovePosition move, const BitBoard &bit_board)
{
  update_flag_ = board_open_state.GetUpdateOpenStateFlag();

  for(const auto pattern : GetAllOpenStatePattern()){
    const auto &base_list = board_open_state.GetList(pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[pattern]);
  }

  LineNeighborhood line_neighborhood(move, kOpenStateNeighborhoodSize, bit_board);

  if(is_black_turn){
    line_neighborhood.AddOpenState<kBlackTurn>(update_flag_, this);
  }else{
    line_neighborhood.AddOpenState<kWhiteTurn>(update_flag_, this);
  }
}

bool IsEqual(const BoardOpenState &lhs, const BoardOpenState &rhs)
{
  for(const auto pattern : GetAllOpenStatePattern()){
    if(lhs.GetList(pattern) != rhs.GetList(pattern)){
      return false;
    }
  }

  if(lhs.GetUpdateOpenStateFlag() != rhs.GetUpdateOpenStateFlag()){
    return false;
  }

  return true;
}

void Copy(const BoardOpenState &from, BoardOpenState * const to)
{
  for(const auto pattern : GetAllOpenStatePattern()){
    to->open_state_list_[pattern] = from.GetList(pattern);
  }

  to->update_flag_ = from.update_flag_;
}

}   // namespace realcore
