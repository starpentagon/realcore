#include "Move.h"
#include "Board.h"

namespace realcore
{

Board::Board()
: bit_board_{{
  #include "def/BitBoardDefinition.h"
  }}
{
}

template<>
void Board::SetState<kOverBoard>(const MovePosition move)
{
  assert(false);
}

template<>
void Board::SetState<kBlackStone>(const MovePosition move)
{
  const auto &direction_list = GetBoardDirection();
  constexpr BitBoard black_stone_xor_mask = 0b10ULL;    // kOpenPosition(0b11) XOR 0b10(mask) = 0b01(kBlackStone)

  for(const auto direction : direction_list){
    const BoardPosition board_position = GetWriteBoardPosition(move, direction);

    assert(!IsReadOnlyBoardPosition(board_position));
    assert(IsWriteOnlyBoardPosition(board_position) || GetState(board_position) == kOpenPosition);

    const size_t index = GetBitBoardIndex(board_position);
    const size_t shift = GetBitBoardShift(board_position);

    bit_board_[index] ^= (black_stone_xor_mask << shift);
  }
}

template<>
void Board::SetState<kWhiteStone>(const MovePosition move)
{
  const auto &direction_list = GetBoardDirection();
  constexpr BitBoard white_stone_xor_mask = 0b01ULL;    // kOpenPosition(0b11) XOR 0b01(mask) = 0b10(kWhiteStone)

  for(const auto direction : direction_list){
    const BoardPosition board_position = GetWriteBoardPosition(move, direction);

    assert(!IsReadOnlyBoardPosition(board_position));
    assert(IsWriteOnlyBoardPosition(board_position) || GetState(board_position) == kOpenPosition);

    const size_t index = GetBitBoardIndex(board_position);
    const size_t shift = GetBitBoardShift(board_position);

    bit_board_[index] ^= (white_stone_xor_mask << shift);
  }
}

template<>
void Board::SetState<kOpenPosition>(const MovePosition move)
{
  const auto &direction_list = GetBoardDirection();
  constexpr BitBoard open_position_or_mask = 0b11ULL;    // Any(0b**) OR 0b11(mask) = 0b11(kOpenPosition)

  for(const auto direction : direction_list){
    const BoardPosition board_position = GetWriteBoardPosition(move, direction);

    assert(!IsUndefinedBoardPosition(board_position) && !IsReadOnlyBoardPosition(board_position));

    const size_t index = GetBitBoardIndex(board_position);
    const size_t shift = GetBitBoardShift(board_position);

    bit_board_[index] |= (open_position_or_mask << shift);
  }
}

void Board::SetState(const MovePosition move, const PositionState state)
{
  switch(state){
  case kOverBoard:
    SetState<kOverBoard>(move);
    break;
  
  case kBlackStone:
    SetState<kBlackStone>(move);
    break;

  case kWhiteStone:
    SetState<kWhiteStone>(move);
    break;

  case kOpenPosition:
    SetState<kOpenPosition>(move);
    break;
  }
}

}   // namespace realcore
