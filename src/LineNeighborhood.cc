#include <iostream>

#include "OpenState.h"
#include "LineNeighborhood.h"
#include "BitBoard.h"

using namespace std;

namespace realcore
{

LineNeighborhood::LineNeighborhood(const MovePosition move, const size_t distance, const BitBoard &bit_board)
: local_bit_board_{{0}}, move_(move), distance_(distance)
{
  std::array<StateBit, kBoardDirectionNum> line_neighborhood;

  switch(distance){
  case 1:
    bit_board.GetLineNeighborhoodStateBit<1>(move, &line_neighborhood);
    break;
  case 2:
    bit_board.GetLineNeighborhoodStateBit<2>(move, &line_neighborhood);
    break;
  case 3:
    bit_board.GetLineNeighborhoodStateBit<3>(move, &line_neighborhood);
    break;
  case 4:
    bit_board.GetLineNeighborhoodStateBit<4>(move, &line_neighborhood);
    break;
  case 5:
    bit_board.GetLineNeighborhoodStateBit<5>(move, &line_neighborhood);
    break;
  case 6:
    bit_board.GetLineNeighborhoodStateBit<6>(move, &line_neighborhood);
    break;
  case 7:
    bit_board.GetLineNeighborhoodStateBit<7>(move, &line_neighborhood);
    break;
  default:
    assert(false);
  }

  local_bit_board_[0] |= line_neighborhood[kLateralDirection];
  local_bit_board_[0] |= line_neighborhood[kVerticalDirection] << 32ULL;
  local_bit_board_[1] |= line_neighborhood[kLeftDiagonalDirection];
  local_bit_board_[1] |= line_neighborhood[kRightDiagonalDirection] << 32ULL;
}

const ForbiddenCheckState LineNeighborhood::ForbiddenCheck(std::vector<BoardPosition> * const next_open_four_list) const
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

void LineNeighborhood::GetBoardPositionList(const LocalBitBoard &bit_list, std::vector<BoardPosition> * const board_position_list) const
{
  assert(board_position_list != nullptr);
  assert(board_position_list->empty());

  if(bit_list[0] == 0 && bit_list[1] == 0){
    return;
  }

  // 1方向(2N + 1)個 * 4方向 = 8N + 4個
  board_position_list->reserve(8 * distance_ + 4);

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

void LineNeighborhood::GetOpenStateOverline(const LocalBitBoard &stone_bit, const LocalBitBoard &open_bit, std::vector<OpenState> *open_state_list) const
{
  assert(distance_ == kOpenStateNeighborhoodSize);
  assert(open_state_list != nullptr);
  assert(open_state_list->empty());

  array<LocalBitBoard, kFourStonePattern> pattern_search_bit;
  array<LocalBitBoard, kFourStonePattern> open_state_bit;

  for(size_t i=0; i<kLocalBitBoardNum; i++){
    array<uint64_t, kFourStonePattern> pattern_search_bit_list{{0}};
    SearchNextOverline(stone_bit[i], open_bit[i], &pattern_search_bit_list);

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
    vector<BoardPosition> open_state_position_list, pattern_position_list;

    GetBoardPositionList(pattern_search_bit[index], &pattern_position_list);
    GetBoardPositionList(open_state_bit[index], &open_state_position_list);

    assert(pattern_position_list.size() == open_state_position_list.size());

    for(size_t i=0, size=open_state_position_list.size(); i<size; i++){
      const auto open_state_position = open_state_position_list[i];
      const auto pattern_position = pattern_position_list[i];

      open_state_list->emplace_back(kNextOverline, open_state_position, pattern_position);
    }
  }
}

}   // namesapce realcore

