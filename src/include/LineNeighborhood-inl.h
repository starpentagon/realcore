#ifndef LINE_NEIGHBORHOOD_INL_H
#define LINE_NEIGHBORHOOD_INL_H

#include "MovePatternSearch.h"
#include "BitBoard.h"
#include "OpenState.h"
#include "LineNeighborhood.h"

namespace realcore
{

template<size_t N>
LineNeighborhood<N>::LineNeighborhood(const MovePosition move, const BitBoard &bit_board)
: local_bit_board_{{0}}, move_(move)
{
  std::array<StateBit, kBoardDirectionNum> line_neighborhood;
  bit_board.GetLineNeighborhoodStateBit<N>(move, &line_neighborhood);

  local_bit_board_[0] |= line_neighborhood[kLateralDirection];
  local_bit_board_[0] |= line_neighborhood[kVerticalDirection] << 32ULL;
  local_bit_board_[1] |= line_neighborhood[kLeftDiagonalDirection];
  local_bit_board_[1] |= line_neighborhood[kRightDiagonalDirection] << 32ULL;
}

template<size_t N>
template<PositionState S>
inline void LineNeighborhood<N>::SetCenterState()
{
  static_assert(S == kBlackStone || S == kWhiteStone, "PositionState must be kBlackStone or kWhiteStone");
  
  constexpr size_t kLowerCenter = 14;   // 下位32bitの中心位置
  constexpr size_t kUpperCenter = 46;   // 上位32bitの中心位置
  
#ifndef NDEBUG
  constexpr std::uint64_t center_open_mask = (0b11ULL << kLowerCenter) | (0b11ULL << kUpperCenter);
#endif
  assert((local_bit_board_[0] & center_open_mask) == center_open_mask);
  assert((local_bit_board_[1] & center_open_mask) == center_open_mask);

  constexpr std::uint64_t stone_xor_mask = S == kBlackStone ? 
  ((0b10ULL << kLowerCenter) | (0b10ULL << kUpperCenter)) : 
  ((0b01ULL << kLowerCenter) | (0b01ULL << kUpperCenter));
  
  local_bit_board_[0] ^= stone_xor_mask;
  local_bit_board_[1] ^= stone_xor_mask;
}

template<size_t N>
inline const BoardDirection LineNeighborhood<N>::GetBoardDirection(const size_t index, const size_t bit_index) const
{
  const auto &board_direction_list = realcore::GetBoardDirection();

  constexpr size_t kMinUpperBitIndex = 32;
  const size_t direction_index = 2 * index + (bit_index < kMinUpperBitIndex ? 0 : 1);

  return board_direction_list[direction_index];
}

template<size_t N>
void LineNeighborhood<N>::GetBoardPositionList(const LocalBitBoard &bit_list, std::vector<BoardPosition> * const board_position_list) const
{
  assert(board_position_list != nullptr);
  assert(board_position_list->empty());

  // 1方向(2N + 1)個 * 4方向 = 8N + 4個
  board_position_list->reserve(8 * N + 4);

  constexpr size_t kMinUpperBitIndex = 32;
  constexpr size_t kLowerCenter = 14;   // 下位32bitの中心位置
  constexpr size_t kUpperCenter = 46;   // 上位32bitの中心位置

  Cordinate x = 0, y = 0;
  GetMoveCordinate(move_, &x, &y);

  std::array<size_t, kBoardDirectionNum> index_list;
  GetBitBoardIndexList(x, y, &index_list);

  std::array<size_t, kBoardDirectionNum> shift_list;
  GetBitBoardShiftList(x, y, &shift_list);
  
  for(size_t list_index=0; list_index<kLocalBitBoardNum; list_index++){
    std::vector<size_t> bit_index_list;
    bit_index_list.reserve(4 * N + 2);   // 1方向(2N + 1)個 * 2方向 = 4N + 2個

    GetBitIndexList(bit_list[list_index], &bit_index_list);

    for(const auto bit_index : bit_index_list){
      const auto direction = GetBoardDirection(list_index, bit_index);
      const auto index = index_list[direction];
      const auto shift = shift_list[direction];
      
      // BitBoard配列での達四位置に対応するシフト量を求める
      const auto center_shift = bit_index < kMinUpperBitIndex ? kLowerCenter : kUpperCenter;
      const auto open_four_shift = shift + GetIndexDifference(center_shift, bit_index);

      const auto board_position = GetBoardPosition(index, open_four_shift);
      board_position_list->push_back(board_position);
    }
  }
}

template<size_t N>
template<PlayerTurn P>
const bool LineNeighborhood<N>::IsOpenFour() const
{
  for(size_t i=0; i<kLocalBitBoardNum; i++){
    const auto state_bit = local_bit_board_[i];

    const auto stone_bit = GetPlayerStoneBit<P>(state_bit);
    const auto open_bit = GetOpenPositionBit(state_bit);

    const auto open_four_bit = SearchOpenFour<P>(stone_bit, open_bit);

    if(open_four_bit != 0){
      return true;
    }
  }
  
  return false;
}

template<size_t N>
template<PlayerTurn P>
const bool LineNeighborhood<N>::IsFour(MovePosition * const guard_move) const
{
  assert(guard_move != nullptr);

  LocalBitBoard guard_move_bit{{0}};

  for(size_t i=0; i<kLocalBitBoardNum; i++){
    const auto state_bit = local_bit_board_[i];

    const auto stone_bit = GetPlayerStoneBit<P>(state_bit);
    const auto open_bit = GetOpenPositionBit(state_bit);

    const auto four_bit = SearchFour<P>(stone_bit, open_bit, &(guard_move_bit[i]));

    if(four_bit != 0){
      std::vector<BoardPosition> guard_move_position;
      GetBoardPositionList(guard_move_bit, &guard_move_position);

      assert(!guard_move_position.empty());
      *guard_move = GetBoardMove(guard_move_position[0]);

      return true;
    }
  }

  return false;
}

template<size_t N>
template<PlayerTurn P>
const bool LineNeighborhood<N>::IsDoubleFour() const
{
  // 「五連にする位置」(= 四の防手位置)の数をカウントする
  LocalBitBoard four_guard_bit{{0}};

  for(size_t i=0; i<kLocalBitBoardNum; i++){
    const auto state_bit = local_bit_board_[i];

    const auto stone_bit = GetPlayerStoneBit<P>(state_bit);
    const auto open_bit = GetOpenPositionBit(state_bit);

    const auto open_four_bit = SearchOpenFour<P>(stone_bit, open_bit);
    SearchFour<P>(stone_bit, open_bit, &(four_guard_bit[i]));

    // 達四があると五連にする位置が2カ所あるので重複カウントしないように片方をオフにする
    four_guard_bit[i] ^= open_four_bit;
  }

  if(IsMultipleBit(four_guard_bit[0], four_guard_bit[1])){
    return true;
  }

  return false;
}

template<size_t N>
const ForbiddenCheckState LineNeighborhood<N>::ForbiddenCheck(std::vector<BoardPosition> * const next_open_four_list) const
{
  assert(next_open_four_list != nullptr);
  assert(next_open_four_list->empty());

  LocalBitBoard local_black_bit{{0}};
  LocalBitBoard local_open_bit{{0}};

  for(size_t i=0; i<kLocalBitBoardNum; i++){
    const auto state_bit = local_bit_board_[i];

    local_black_bit[i] = GetBlackStoneBit(state_bit);
    local_open_bit[i] = GetOpenPositionBit(state_bit);
  }

  // 長連
  for(const auto black_bit : local_black_bit){
    if(IsOverline(black_bit)){
      return kForbiddenMove;
    }
  }

  // 四々
  LocalBitBoard four_guard_bit{{0}};

  for(size_t i=0; i<kLocalBitBoardNum; i++){
    const auto black_bit = local_black_bit[i];
    const auto open_bit = local_open_bit[i];

    const auto open_four_bit = SearchOpenFour<kBlackTurn>(black_bit, open_bit);
    SearchFour<kBlackTurn>(black_bit, open_bit, &(four_guard_bit[i]));

    // 達四があると四のパターンが2カ所マッチするので片方をオフにする
    four_guard_bit[i] ^= open_four_bit;
  }

  if(IsMultipleBit(four_guard_bit[0], four_guard_bit[1])){
    return kForbiddenMove;
  }

  // 見かけの三々
  LocalBitBoard semi_three_bit{{0}};
  LocalBitBoard next_open_four_bit{{0}};

  for(size_t i=0; i<kLocalBitBoardNum; i++){
    const auto black_bit = local_black_bit[i];
    const auto open_bit = local_open_bit[i];

    semi_three_bit[i] = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &(next_open_four_bit[i]));
  }

  if(!IsMultipleBit(semi_three_bit[0], semi_three_bit[1])){
    return kNonForbiddenMove;
  }

  // 見かけの三々が2つ以上存在する
  GetBoardPositionList(next_open_four_bit, next_open_four_list);

  return kPossibleForbiddenMove;
}

template<size_t N>
template<OpenStatePattern Pattern, PlayerTurn P>
void LineNeighborhood<N>::GetOpenState(std::vector< OpenState<Pattern> > *open_state_list) const
{
  // パターンの手番と着手の手番が異なる場合、パターンがマッチすることはないので抜ける
  if(GetPatternPlayerTurn(Pattern) != P){
    return;
  }

  LocalBitBoard stone_bit{{0}}, open_bit{{0}};

  for(size_t i=0; i<kLocalBitBoardNum; i++){
    const auto state_bit = local_bit_board_[i];

    stone_bit[i] = GetPlayerStoneBit<P>(state_bit);
    open_bit[i] = GetOpenPositionBit(state_bit);
  }
  
  GetOpenState<Pattern, P>(stone_bit, open_bit, open_state_list);
}

template<size_t N>
template<OpenStatePattern Pattern, PlayerTurn P>
void LineNeighborhood<N>::GetOpenStateOpenFour(const LocalBitBoard &stone_bit, const LocalBitBoard &open_bit, std::vector< OpenState<Pattern> > *open_state_list) const
{
  static_assert(N == kOpenStateNeighborhoodSize, "N must be kOpenStateNeighborhoodSize for calling GetOpenState.");
  static_assert(Pattern == kNextOpenFourBlack || Pattern == kNextOpenFourWhite, "Pattern must be kNextOpenFour[Black|White]");
  static_assert(GetPatternPlayerTurn(Pattern) == P, "Pattern's turn must be consistent with player turn.'");

  assert(open_state_list != nullptr);
  assert(open_state_list->empty());

  std::array<LocalBitBoard, kFourStonePattern> pattern_search_bit;
  std::array<LocalBitBoard, kFourStonePattern> open_state_bit;

  for(size_t i=0; i<kLocalBitBoardNum; i++){
    std::array<uint64_t, kFourStonePattern> pattern_search_bit_list{{0}};
    SearchNextOpenFour<P>(stone_bit[i], open_bit[i], &pattern_search_bit_list);

    for(size_t index=0; index<kFourStonePattern; index++){
      pattern_search_bit[index][i] = pattern_search_bit_list[index];
      open_state_bit[index][i] = GetOpenBitInPattern(index, pattern_search_bit_list[index]);
    }
  }

  constexpr size_t kPatternLength = 6;    //!< パターン長
  constexpr size_t kOpenCount = 1;        //!< 設定する空点状態の数
  constexpr size_t kMaxSearchBitCount = (2 * kOpenStateNeighborhoodSize + 1) - kPatternLength + 1;
  constexpr size_t kListSize = 4 * kOpenCount * kMaxSearchBitCount;
  open_state_list->reserve(kListSize);

  for(size_t index=0; index<kFourStonePattern; index++){
    std::vector<BoardPosition> open_state_position_list, pattern_position_list;

    GetBoardPositionList(pattern_search_bit[index], &pattern_position_list);
    GetBoardPositionList(open_state_bit[index], &open_state_position_list);

    assert(pattern_position_list.size() == open_state_position_list.size());

    for(size_t i=0, size=open_state_position_list.size(); i<size; i++){
      const auto open_state_position = open_state_position_list[i];
      const auto pattern_position = pattern_position_list[i];

      open_state_list->emplace_back(open_state_position, pattern_position);
    }
  }
}

template<size_t N>
template<OpenStatePattern Pattern, PlayerTurn P>
void LineNeighborhood<N>::GetOpenStateFour(const LocalBitBoard &stone_bit, const LocalBitBoard &open_bit, std::vector< OpenState<Pattern> > *open_state_list) const
{
  static_assert(N == kOpenStateNeighborhoodSize, "N must be kOpenStateNeighborhoodSize for calling GetOpenState.");
  static_assert(Pattern == kNextFourBlack || Pattern == kNextFourWhite, "Pattern must be kNextFour[Black|White]");
  static_assert(GetPatternPlayerTurn(Pattern) == P, "Pattern's turn must be consistent with player turn.'");

  assert(open_state_list != nullptr);
  assert(open_state_list->empty());

  constexpr size_t kPatternNum = kTwoOfFivePattern;

  std::array<LocalBitBoard, kPatternNum> pattern_search_bit;
  std::array<LocalBitBoard, kPatternNum> open_state_bit_1, open_state_bit_2;

  for(size_t i=0; i<kLocalBitBoardNum; i++){
    std::array<uint64_t, kPatternNum> pattern_search_bit_list{{0}};
    SearchNextFour<P>(stone_bit[i], open_bit[i], &pattern_search_bit_list);

    for(size_t index=0; index<kPatternNum; index++){
      pattern_search_bit[index][i] = pattern_search_bit_list[index];
      
      const size_t less_index = GetLessIndexOfTwo(index);
      const size_t greater_index = GetGreaterIndexOfTwo(index);
      
      open_state_bit_1[index][i] = GetOpenBitInPattern(less_index, pattern_search_bit_list[index]);
      open_state_bit_2[index][i] = GetOpenBitInPattern(greater_index, pattern_search_bit_list[index]);
    }
  }

  constexpr size_t kPatternLength = 5;    //!< パターン長
  constexpr size_t kOpenCount = 2;        //!< 設定する空点状態の数
  constexpr size_t kMaxSearchBitCount = (2 * kOpenStateNeighborhoodSize + 1) - kPatternLength + 1;
  constexpr size_t kListSize = 4 * kOpenCount * kMaxSearchBitCount;
  open_state_list->reserve(kListSize);

  for(size_t index=0; index<kPatternNum; index++){
    std::vector<BoardPosition> open_state_position_list_1, open_state_position_list_2, pattern_position_list;

    GetBoardPositionList(pattern_search_bit[index], &pattern_position_list);
    GetBoardPositionList(open_state_bit_1[index], &open_state_position_list_1);
    GetBoardPositionList(open_state_bit_2[index], &open_state_position_list_2);

    assert(pattern_position_list.size() == open_state_position_list_1.size());
    assert(pattern_position_list.size() == open_state_position_list_2.size());

    for(size_t i=0, size=open_state_position_list_1.size(); i<size; i++){
      const auto open_state_position_1 = open_state_position_list_1[i];
      const auto open_state_position_2 = open_state_position_list_2[i];
      const auto pattern_position = pattern_position_list[i];

      {
        open_state_list->emplace_back(open_state_position_1, pattern_position);
        
        std::vector<BoardPosition> guard_list_1{open_state_position_2};
        open_state_list->back().SetGuardPositionList(guard_list_1);
      }
      {
        open_state_list->emplace_back(open_state_position_2, pattern_position);
        
        std::vector<BoardPosition> guard_list_2{open_state_position_1};
        open_state_list->back().SetGuardPositionList(guard_list_2);
      }
    }
  }
}

template<size_t N>
template<OpenStatePattern Pattern, PlayerTurn P>
void LineNeighborhood<N>::GetOpenStateSemiThree(const LocalBitBoard &stone_bit, const LocalBitBoard &open_bit, std::vector< OpenState<Pattern> > *open_state_list) const
{
  static_assert(N == kOpenStateNeighborhoodSize, "N must be kOpenStateNeighborhoodSize for calling GetOpenState.");
  static_assert(Pattern == kNextSemiThreeBlack || Pattern == kNextSemiThreeWhite, "Pattern must be kNextSemiThree[Black|White]");
  static_assert(GetPatternPlayerTurn(Pattern) == P, "Pattern's turn must be consistent with player turn.'");

  assert(open_state_list != nullptr);
  assert(open_state_list->empty());

  constexpr size_t kPatternNum = kTwoOfFourPattern;

  std::array<LocalBitBoard, kPatternNum> pattern_search_bit;
  std::array<LocalBitBoard, kPatternNum> open_state_bit_1, open_state_bit_2;

  for(size_t i=0; i<kLocalBitBoardNum; i++){
    std::array<uint64_t, kPatternNum> pattern_search_bit_list{{0}};
    SearchNextSemiThree<P>(stone_bit[i], open_bit[i], &pattern_search_bit_list);

    for(size_t index=0; index<kPatternNum; index++){
      pattern_search_bit[index][i] = pattern_search_bit_list[index];
      
      const size_t less_index = GetLessIndexOfTwo(index);
      const size_t greater_index = GetGreaterIndexOfTwo(index);
      
      open_state_bit_1[index][i] = GetOpenBitInPattern(less_index, pattern_search_bit_list[index]);
      open_state_bit_2[index][i] = GetOpenBitInPattern(greater_index, pattern_search_bit_list[index]);
    }
  }

  constexpr size_t kPatternLength = 6;    //!< パターン長
  constexpr size_t kOpenCount = 2;        //!< 設定する空点状態の数
  constexpr size_t kMaxSearchBitCount = (2 * kOpenStateNeighborhoodSize + 1) - kPatternLength + 1;
  constexpr size_t kListSize = 4 * kOpenCount * kMaxSearchBitCount;
  open_state_list->reserve(kListSize);

  for(size_t index=0; index<kPatternNum; index++){
    std::vector<BoardPosition> open_state_position_list_1, open_state_position_list_2, pattern_position_list;

    GetBoardPositionList(pattern_search_bit[index], &pattern_position_list);
    GetBoardPositionList(open_state_bit_1[index], &open_state_position_list_1);
    GetBoardPositionList(open_state_bit_2[index], &open_state_position_list_2);

    assert(pattern_position_list.size() == open_state_position_list_1.size());
    assert(pattern_position_list.size() == open_state_position_list_2.size());

    for(size_t i=0, size=open_state_position_list_1.size(); i<size; i++){
      const auto open_state_position_1 = open_state_position_list_1[i];
      const auto open_state_position_2 = open_state_position_list_2[i];
      const auto pattern_position = pattern_position_list[i];

      const auto guard_position_1 = pattern_position - 1;
      const auto guard_position_2 = pattern_position + 4;

      {
        open_state_list->emplace_back(open_state_position_1, pattern_position);

        if(P == kBlackTurn){
          std::vector<BoardPosition> check_list{open_state_position_2};
          open_state_list->back().SetCheckPositionList(check_list);
        }
        
        std::vector<BoardPosition> guard_list_1{open_state_position_2, guard_position_1, guard_position_2};
        open_state_list->back().SetGuardPositionList(guard_list_1);
      }
      {
        open_state_list->emplace_back(open_state_position_2, pattern_position);
        
        if(P == kBlackTurn){
          std::vector<BoardPosition> check_list{open_state_position_1};
          open_state_list->back().SetCheckPositionList(check_list);
        }

        std::vector<BoardPosition> guard_list_2{open_state_position_1, guard_position_1, guard_position_2};
        open_state_list->back().SetGuardPositionList(guard_list_2);
      }

    }
  }
}

}   // namespace realcore

#endif    // LINE_NEIGHBORHOOD_INL_H
