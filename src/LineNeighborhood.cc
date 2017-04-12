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
  const auto combined_black_bit = GetPlayerStoneCombinedBit<kBlackTurn>();
  const auto combined_open_bit = GetOpenPositionCombinedBit();

  // 長連
  if(IsOverline(combined_black_bit)){
    return kForbiddenMove;
  }

  // 四々
  const auto open_four_bit = SearchOpenFour<kBlackTurn>(combined_black_bit, combined_open_bit);
  std::uint64_t make_five_move_bit = 0;
  SearchFour<kBlackTurn>(combined_black_bit, combined_open_bit, &make_five_move_bit);
  
  // 達四があると五連にする位置が2カ所あるので重複カウントしないように片方をオフにする
  // @see doc/06_forbidden_check/forbidden_check.pptx, 「達四がある場合の四のマッチ方法」
  make_five_move_bit ^= RightShift<1>(open_four_bit);

  if(IsMultipleBit(make_five_move_bit)){
    return kForbiddenMove;
  }

  // 見かけの三々
  std::uint64_t next_open_four_bit = 0;
  const auto semi_three_bit = SearchSemiThree<kBlackTurn>(combined_black_bit, combined_open_bit, &next_open_four_bit);

  if(!IsMultipleBit(semi_three_bit)){
    return kNonForbiddenMove;
  }

  // 見かけの三々が2つ以上存在する
  assert(next_open_four_list != nullptr);
  assert(next_open_four_list->empty());

  std::vector<size_t> combined_shift_list;
  GetBitIndexList(next_open_four_bit, &combined_shift_list);
  next_open_four_list->reserve(combined_shift_list.size());

  for(const auto combined_shift : combined_shift_list){
    const auto guard_board_position = GetBoardPosition(combined_shift);
    next_open_four_list->emplace_back(guard_board_position);
  }

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
      
      // BitBoard配列での位置に対応するシフト量を求める
      const auto center_shift = bit_index < kMinUpperBitIndex ? kLowerCenter : kUpperCenter;
      const auto open_four_shift = shift + GetIndexDifference(center_shift, bit_index);

      const auto board_position = realcore::GetBoardPosition(index, open_four_shift);
      board_position_list->push_back(board_position);
    }
  }
}

template<>
void LineNeighborhood::AddOpenState<kBlackTurn>(BoardOpenState * const board_open_state) const
{
  const auto combined_black_stone = GetBlackStoneBit(local_bit_board_[0]) | (GetBlackStoneBit(local_bit_board_[1]) << 1);
  const auto combined_open_stone = GetOpenPositionBit(local_bit_board_[0]) | (GetOpenPositionBit(local_bit_board_[1]) << 1);

  GetOpenState<kNextOverline>(combined_black_stone, combined_open_stone, board_open_state);
  GetOpenState<kNextOpenFourBlack>(combined_black_stone, combined_open_stone, board_open_state);
  GetOpenState<kNextFourBlack>(combined_black_stone, combined_open_stone, board_open_state);
  GetOpenState<kNextSemiThreeBlack>(combined_black_stone, combined_open_stone, board_open_state);
}

template<>
void LineNeighborhood::AddOpenState<kWhiteTurn>(BoardOpenState * const board_open_state) const
{
  const auto combined_white_stone = GetWhiteStoneBit(local_bit_board_[0]) | (GetWhiteStoneBit(local_bit_board_[1]) << 1);
  const auto combined_open_stone = GetOpenPositionBit(local_bit_board_[0]) | (GetOpenPositionBit(local_bit_board_[1]) << 1);

  GetOpenState<kNextOpenFourWhite>(combined_white_stone, combined_open_stone, board_open_state);
  GetOpenState<kNextFourWhite>(combined_white_stone, combined_open_stone, board_open_state);
  GetOpenState<kNextSemiThreeWhite>(combined_white_stone, combined_open_stone, board_open_state);
}
}   // namesapce realcore

