#include <iostream>
#include <sstream>

#include "OpenState.h"
#include "LineNeighborhood.h"
#include "BitBoard.h"
#include "BoardOpenState.h"

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
  return ForbiddenCheck(next_open_four_list, nullptr, nullptr);
}

const ForbiddenCheckState LineNeighborhood::ForbiddenCheck(std::vector<BoardPosition> * const next_open_four_list, MoveBitSet * const downward_influence_area, MoveBitSet * const upward_influence_area) const
{
  const auto combined_black_bit = GetPlayerStoneCombinedBit<kBlackTurn>();
  const auto combined_open_bit = GetOpenPositionCombinedBit();

  // 長連
  if(IsOverline(combined_black_bit)){
    if(downward_influence_area != nullptr){
      downward_influence_area->set(move_);
    }
    
    return kForbiddenMove;
  }

  // 四々
  const auto open_four_bit = SearchOpenFour<kBlackTurn>(combined_black_bit, combined_open_bit);
  std::uint64_t make_five_move_bit = 0;
  const auto four_bit = SearchFour<kBlackTurn>(combined_black_bit, combined_open_bit, &make_five_move_bit);
  
  // 達四があると五連にする位置が2カ所あるので重複カウントしないように片方をオフにする
  // @see doc/06_forbidden_check/forbidden_check.pptx, 「達四がある場合の四のマッチ方法」
  make_five_move_bit ^= RightShift<1>(open_four_bit);

  if(IsMultipleBit(make_five_move_bit)){
    GetDoubleFourInfluenceArea<kBlackTurn>(four_bit, combined_open_bit, make_five_move_bit, downward_influence_area);
    return kForbiddenMove;
  }

  // 見かけの三々
  std::uint64_t next_open_four_bit = 0;
  const auto semi_three_bit = SearchSemiThree<kBlackTurn>(combined_black_bit, combined_open_bit, &next_open_four_bit);

  // 2方向以上で見かけの三々が生じているかチェックする
  bitset<kBoardDirectionNum> semi_three_direction_bit;
  std::vector<size_t> semi_three_combined_shift_list;
  GetBitIndexList(semi_three_bit, &semi_three_combined_shift_list);

  for(const auto combined_shift : semi_three_combined_shift_list){
    const BoardPosition board_position = GetBoardPosition(combined_shift);
    const BoardDirection board_direction = realcore::GetBoardDirection(board_position);

    semi_three_direction_bit.set(board_direction);
  }

  const auto semi_three_direction_count = semi_three_direction_bit.count();

  // 長連/四々/見かけの三々が新たに生じるための影響領域を生成する
  GetNonOverlineInfluenceArea(combined_black_bit, combined_open_bit, upward_influence_area);
  GetNonDoubleFourInfluenceArea(combined_black_bit, combined_open_bit, make_five_move_bit, upward_influence_area);

  if(semi_three_direction_count >= 1){
    GetNonDoubleSemiThreeInfluenceArea(combined_black_bit, combined_open_bit, upward_influence_area);
  }

  if(semi_three_direction_count < 2){
    // 禁手ではない
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

  GetDoubleSemiThreeInfluenceArea<kBlackTurn>(semi_three_bit, combined_open_bit, next_open_four_bit, downward_influence_area);

  return kPossibleForbiddenMove;
}

void LineNeighborhood::GetNonOverlineInfluenceArea(const uint64_t combined_stone_bit, const uint64_t combined_open_bit, MoveBitSet * const influence_area) const
{
  if(influence_area == nullptr){
    return;
  }

  // 長連点の位置に黒石が来ると長連が成立する
  constexpr size_t kPatternNum = GetOpenStatePatternNum(kNextOverline);
  std::array<std::uint64_t, kPatternNum> pattern_search{{0}};

  SearchNextOverline(combined_stone_bit, combined_open_bit, &pattern_search);
  
  for(size_t pattern_index=0; pattern_index<kPatternNum; pattern_index++){
    const auto search_bit = pattern_search[pattern_index];
    SetOpenMoveBitSet(search_bit, pattern_index, influence_area);
  }
}

void LineNeighborhood::GetNonDoubleFourInfluenceArea(const uint64_t combined_stone_bit, const uint64_t combined_open_bit, const std::uint64_t make_five_move_bit, MoveBitSet * const influence_area) const
{
  if(influence_area == nullptr){
    return;
  }

  constexpr size_t kPatternNum = GetOpenStatePatternNum(kNextFourBlack);
  std::array<std::uint64_t, kPatternNum> pattern_search{{0}};

  SearchNextFour<kBlackTurn>(combined_stone_bit, combined_open_bit, &pattern_search);
  
  if(make_five_move_bit != 0){
    // 四がすでに１つできているケース
    std::vector<size_t> bit_index_list;
    GetBitIndexList(make_five_move_bit, &bit_index_list);
    assert(bit_index_list.size() == 1);   // 四々はないので五連を作る位置は１つのみ

    const auto make_five_board_position = GetBoardPosition(bit_index_list[0]);

    // 黒石を１つ加えることでもう１つ四が作れる位置を探す
    for(size_t pattern_index=0; pattern_index<kPatternNum; pattern_index++){
      const auto search_bit = pattern_search[pattern_index];

      if(search_bit == 0){
        continue;
      }
      
      std::vector<size_t> bit_index_list;
      GetBitIndexList(search_bit, &bit_index_list);

      for(const auto combined_shift : bit_index_list){
        const BoardPosition pattern_position = GetBoardPosition(combined_shift);

        {
          const size_t make_five_index = GetGreaterIndexOfTwo(pattern_index);
          const BoardPosition make_five_position = GetOpenBoardPosition(pattern_position, make_five_index);

          if(make_five_position != make_five_board_position){
            // 新たに四が１つできる
            const size_t open_index = GetLessIndexOfTwo(pattern_index);
            const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
            const MovePosition move = GetBoardMove(open_position);

            influence_area->set(move);
          }
        }
        {
          const size_t make_five_index = GetLessIndexOfTwo(pattern_index);
          const BoardPosition make_five_position = GetOpenBoardPosition(pattern_position, make_five_index);

          if(make_five_position != make_five_board_position){
            // 新たに四が１つできる
            const size_t open_index = GetGreaterIndexOfTwo(pattern_index);
            const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
            const MovePosition move = GetBoardMove(open_position);

            influence_area->set(move);
          }
        }
      }
    }
  }

  // 黒石を１つ追加することで中心点で四が２つできるケース
  // 達四点があると四ノビパターンが２回マッチされてしまう
  // ->達四の空点位置とパターン位置が一致する四はスキップしてカウントする。
  // ->達四の空点位置とパターン位置を記録しておく
  std::vector<int> open_four_key_list;

  {
    constexpr size_t kPatternNum = GetOpenStatePatternNum(kNextOpenFourBlack);
    std::array<std::uint64_t, kPatternNum> pattern_search{{0}};

    SearchNextOpenFour<kBlackTurn>(combined_stone_bit, combined_open_bit, &pattern_search);
    
    for(size_t pattern_index=0; pattern_index<kPatternNum; pattern_index++){
      const auto search_bit = pattern_search[pattern_index];

      if(search_bit == 0){
        continue;
      }

      std::vector<size_t> bit_index_list;
      GetBitIndexList(search_bit, &bit_index_list);

      for(const auto combined_shift : bit_index_list){
        const BoardPosition pattern_position = GetBoardPosition(combined_shift);
        const BoardPosition open_position = GetOpenBoardPosition(pattern_position, pattern_index);

        const int open_four_key = (static_cast<int>(open_position) << 16) | static_cast<int>(pattern_position);
        open_four_key_list.emplace_back(open_four_key);
      }
    }
  }

  std::array<int, kMoveNum> next_five_position_table;
  next_five_position_table.fill(-1);

  for(size_t pattern_index=0; pattern_index<kPatternNum; pattern_index++){
    const auto search_bit = pattern_search[pattern_index];

    if(search_bit == 0){
      continue;
    }
    
    std::vector<size_t> bit_index_list;
    GetBitIndexList(search_bit, &bit_index_list);

    for(const auto combined_shift : bit_index_list){
      const BoardPosition pattern_position = GetBoardPosition(combined_shift);

      size_t open_index = GetLessIndexOfTwo(pattern_index);
      BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
      int open_four_key = (static_cast<int>(open_position) << 16) | static_cast<int>(pattern_position);

      // 達四と空点位置, パターン位置が一致する四はスキップする
      std::vector<int>::const_iterator find_it = find(open_four_key_list.begin(), open_four_key_list.end(), open_four_key);

      if(find_it == open_four_key_list.end()){
        const MovePosition add_move = GetBoardMove(open_position);
        const size_t make_five_index = GetGreaterIndexOfTwo(pattern_index);
        const BoardPosition make_five_position = GetOpenBoardPosition(pattern_position, make_five_index);

        const bool is_double_four = next_five_position_table[add_move] != -1 && next_five_position_table[add_move] != static_cast<int>(make_five_position);
        next_five_position_table[add_move] = make_five_position;

        if(is_double_four){
          // 新たに四が１つできる
          influence_area->set(add_move);
        }
      }

      open_index = GetGreaterIndexOfTwo(pattern_index);
      open_position = GetOpenBoardPosition(pattern_position, open_index);
      open_four_key = (static_cast<int>(open_position) << 16) | static_cast<int>(pattern_position);

      // 達四と空点位置, パターン位置が一致する四はスキップする
      find_it = find(open_four_key_list.begin(), open_four_key_list.end(), open_four_key);

      if(find_it == open_four_key_list.end()){
        const MovePosition add_move = GetBoardMove(open_position);
        const size_t make_five_index = GetLessIndexOfTwo(pattern_index);
        const BoardPosition make_five_position = GetOpenBoardPosition(pattern_position, make_five_index);

        const bool is_double_four = next_five_position_table[add_move] != -1 && next_five_position_table[add_move] != static_cast<int>(make_five_position);
        next_five_position_table[add_move] = make_five_position;

        if(is_double_four){
          // 新たに四が１つできる
          influence_area->set(add_move);
        }
      }
    }
  }
}

void LineNeighborhood::GetNonDoubleSemiThreeInfluenceArea(const std::uint64_t combined_stone_bit, const std::uint64_t combined_open_bit, MoveBitSet * const influence_area) const
{
  if(influence_area == nullptr){
    return;
  }
  
  constexpr size_t kPatternNum = GetOpenStatePatternNum(kNextSemiThreeBlack);
  std::array<std::uint64_t, kPatternNum> pattern_search{{0}};

  SearchNextSemiThree<kBlackTurn>(combined_stone_bit, combined_open_bit, &pattern_search);
  
  for(size_t pattern_index=0; pattern_index<kPatternNum; pattern_index++){
    const auto search_bit = pattern_search[pattern_index];

    if(search_bit == 0){
      continue;
    }
    
    std::vector<size_t> bit_index_list;
    GetBitIndexList(search_bit, &bit_index_list);

    for(const auto combined_shift : bit_index_list){
      const BoardPosition pattern_position = GetBoardPosition(combined_shift);

      {
        const size_t open_index = GetLessIndexOfTwo(pattern_index);
        const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
        const MovePosition move = GetBoardMove(open_position);
        influence_area->set(move);
      }
      {
        const size_t open_index = GetGreaterIndexOfTwo(pattern_index);
        const BoardPosition open_position = GetOpenBoardPosition(pattern_position, open_index);
        const MovePosition move = GetBoardMove(open_position);
        influence_area->set(move);
      }
    }
  }
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
      board_position_list->emplace_back(board_position);
    }
  }
}

template<>
void LineNeighborhood::AddOpenState<kBlackTurn>(const UpdateOpenStateFlag &update_flag, BoardOpenState * const board_open_state) const
{
  const auto combined_black_stone = GetPlayerStoneCombinedBit<kBlackTurn>();
  const auto combined_open_stone = GetOpenPositionCombinedBit();

  if(update_flag[kNextOverline]){
    GetOpenState<kNextOverline>(combined_black_stone, combined_open_stone, board_open_state);
  }

  if(update_flag[kNextOpenFourBlack]){
    GetOpenState<kNextOpenFourBlack>(combined_black_stone, combined_open_stone, board_open_state);
  }

  if(update_flag[kNextFourBlack]){
    GetOpenState<kNextFourBlack>(combined_black_stone, combined_open_stone, board_open_state);
  }

  if(update_flag[kNextSemiThreeBlack]){
    GetOpenState<kNextSemiThreeBlack>(combined_black_stone, combined_open_stone, board_open_state);
  }

  if(update_flag[kNextPointOfSwordBlack]){
    GetOpenState<kNextPointOfSwordBlack>(combined_black_stone, combined_open_stone, board_open_state);
  }

  if(update_flag[kNextTwoBlack]){
    GetOpenState<kNextTwoBlack>(combined_black_stone, combined_open_stone, board_open_state);
  }
}

template<>
void LineNeighborhood::AddOpenState<kWhiteTurn>(const UpdateOpenStateFlag &update_flag, BoardOpenState * const board_open_state) const
{
  const auto combined_white_stone = GetPlayerStoneCombinedBit<kWhiteTurn>();
  const auto combined_open_stone = GetOpenPositionCombinedBit();

  if(update_flag[kNextOpenFourWhite]){
    GetOpenState<kNextOpenFourWhite>(combined_white_stone, combined_open_stone, board_open_state);
  }
  
  if(update_flag[kNextFourWhite]){
    GetOpenState<kNextFourWhite>(combined_white_stone, combined_open_stone, board_open_state);
  }

  if(update_flag[kNextSemiThreeWhite]){
    GetOpenState<kNextSemiThreeWhite>(combined_white_stone, combined_open_stone, board_open_state);
  }

  if(update_flag[kNextPointOfSwordWhite]){
    GetOpenState<kNextPointOfSwordWhite>(combined_white_stone, combined_open_stone, board_open_state);
  }

  if(update_flag[kNextTwoWhite]){
    GetOpenState<kNextTwoWhite>(combined_white_stone, combined_open_stone, board_open_state);
  }
}

void LineNeighborhood::GetOpenMovePosition(MoveList * const move_list) const
{
  const auto combined_open_bit = GetOpenPositionCombinedBit();
  MoveBitSet open_bit;
  SetMoveBitSet(combined_open_bit, &open_bit);
  
  GetMoveList(open_bit, move_list);
}

string LineNeighborhood::str() const
{
  stringstream ss;
  const auto cross = GetStateBitString(local_bit_board_[0]);
  const auto saltire = GetStateBitString(local_bit_board_[1]);

  // 空白を除く
  const auto cross_concat = cross.substr(0, 8) + cross.substr(8 + 1, 8) + cross.substr(16 + 2, 8) + cross.substr(24 + 3, 8);
  const auto saltire_concat = saltire.substr(0, 8) + saltire.substr(8 + 1, 8) + saltire.substr(16 + 2, 8) + saltire.substr(24 + 3, 8);

  // 左詰めで文字列が生成されるので反転して右詰めにする
  string cross_flip, saltire_flip;

  for(size_t i=0, size=cross_concat.length(); i<size; i++){
    cross_flip = cross_concat.at(i) + cross_flip;
    saltire_flip = saltire_concat.at(i) + saltire_flip;
  }

  // move_を中心とした文字列を抽出する
  const size_t start_index = 7 - distance_;
  const size_t string_number = 2 * distance_ + 1;

  const auto horizontal = cross_flip.substr(0, 16).substr(start_index, string_number);
  const auto vertical = cross_flip.substr(16).substr(start_index, string_number);
  const auto left_down = saltire_flip.substr(0, 16).substr(start_index, string_number);
  const auto right_down = saltire_flip.substr(16).substr(start_index, string_number);
  
  ss << "Horizn: " << horizontal << endl;
  ss << "Vertcl: " << vertical << endl;
  ss << "L-Down: " << left_down << endl;
  ss << "R-Down: " << right_down << endl;

  return ss.str();
}

}   // namesapce realcore

