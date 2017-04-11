#include "BoardOpenState.h"

using namespace std;

namespace realcore
{

BoardOpenState::BoardOpenState()
{
}

BoardOpenState::BoardOpenState(const BoardOpenState &board_open_state)
{
  *this = board_open_state;
}

BoardOpenState::BoardOpenState(const BoardOpenState &board_open_state, const bool is_black_turn, const MovePosition move, const BitBoard &bit_board)
{
  {
    // 長連点
    constexpr OpenStatePattern Pattern = kNextOverline;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }
  {
    // 達四点(黒)
    constexpr OpenStatePattern Pattern = kNextOpenFourBlack;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }
  {
    // 達四点(白)
    constexpr OpenStatePattern Pattern = kNextOpenFourWhite;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }
  {
    // 四ノビ点(黒)
    constexpr OpenStatePattern Pattern = kNextFourBlack;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }
  {
    // 四ノビ点(白)
    constexpr OpenStatePattern Pattern = kNextFourWhite;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }
  {
    // 見かけの三ノビ点(黒)
    constexpr OpenStatePattern Pattern = kNextSemiThreeBlack;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }
  {
    // 見かけの三ノビ点(白)
    constexpr OpenStatePattern Pattern = kNextSemiThreeWhite;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }

  LineNeighborhood line_neighborhood(move, kOpenStateNeighborhoodSize, bit_board);

  if(is_black_turn){
    line_neighborhood.AddOpenState<kBlackTurn>(this);
  }else{
    line_neighborhood.AddOpenState<kWhiteTurn>(this);
  }
}

bool IsEqual(const BoardOpenState &lhs, const BoardOpenState &rhs)
{
  for(const auto pattern : GetAllOpenStatePattern()){
    if(lhs.GetList(pattern) != rhs.GetList(pattern)){
      return false;
    }
  }

  return true;
}

void Copy(const BoardOpenState &from, BoardOpenState * const to)
{
  for(const auto pattern : GetAllOpenStatePattern()){
    to->open_state_list_[pattern] = from.GetList(pattern);
  }
}

}   // namespace realcore
