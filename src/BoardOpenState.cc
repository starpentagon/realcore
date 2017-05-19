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

  // @note OpenStatePatternリストのfor文にすると30%程度遅くなったのでfor文を展開した実装を採用
  if(update_flag_[kNextOverline]){
    // 長連点
    constexpr OpenStatePattern Pattern = kNextOverline;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }

  if(update_flag_[kNextOpenFourBlack]){
    // 達四点(黒)
    constexpr OpenStatePattern Pattern = kNextOpenFourBlack;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }

  if(update_flag_[kNextOpenFourWhite]){
    // 達四点(白)
    constexpr OpenStatePattern Pattern = kNextOpenFourWhite;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }

  if(update_flag_[kNextFourBlack]){
    // 四ノビ点(黒)
    constexpr OpenStatePattern Pattern = kNextFourBlack;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }

  if(update_flag_[kNextFourWhite]){
    // 四ノビ点(白)
    constexpr OpenStatePattern Pattern = kNextFourWhite;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }

  if(update_flag_[kNextSemiThreeBlack]){
    // 見かけの三ノビ点(黒)
    constexpr OpenStatePattern Pattern = kNextSemiThreeBlack;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }

  if(update_flag_[kNextSemiThreeWhite]){
    // 見かけの三ノビ点(白)
    constexpr OpenStatePattern Pattern = kNextSemiThreeWhite;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }

  if(update_flag_[kNextPointOfSwordBlack]){
    // 剣先点(黒)
    constexpr OpenStatePattern Pattern = kNextPointOfSwordBlack;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }

  if(update_flag_[kNextPointOfSwordWhite]){
    // 剣先点(白)
    constexpr OpenStatePattern Pattern = kNextPointOfSwordWhite;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }

  if(update_flag_[kNextTwoBlack]){
    // 二ノビ点(黒)
    constexpr OpenStatePattern Pattern = kNextTwoBlack;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
  }

  if(update_flag_[kNextTwoWhite]){
    // 二ノビ点(白)
    constexpr OpenStatePattern Pattern = kNextTwoWhite;
    const auto &base_list = board_open_state.GetList(Pattern);
    ClearInfluencedOpenState(is_black_turn, base_list, move, &open_state_list_[Pattern]);
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
