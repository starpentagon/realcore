// @brief メンバ関数のテスト
#include "gtest/gtest.h"

#include "Move.h"
#include "MoveList.h"
#include "BitBoard.h"

using namespace std;

namespace realcore
{

class BitBoardTest
: public ::testing::Test
{
public:
  void GetSetStateTest(){
    BitBoard bit_board;
    const auto &all_move_list = GetAllMove();

    // 黒石
    for(auto set_move : all_move_list){
      bit_board.SetState<kBlackStone>(set_move);

      for(auto get_move : all_move_list){
        // 設定した位置では黒石が、それ以外は空点が設定されていることを確認する
        PositionState check_state = kOverBoard;

        if(IsInBoardMove(get_move)){
          check_state = set_move == get_move ? kBlackStone : kOpenPosition;
        }

        EXPECT_EQ(check_state, bit_board.GetState(get_move));
      }

      bit_board.SetState<kOpenPosition>(set_move);
    }

    // 白石
    for(auto set_move : all_move_list){
      bit_board.SetState<kWhiteStone>(set_move);

      for(auto get_move : all_move_list){
        // 設定した位置では白石が、それ以外は空点が設定されていることを確認する
        PositionState check_state = kOverBoard;

        if(IsInBoardMove(get_move)){
          check_state = set_move == get_move ? kWhiteStone : kOpenPosition;
        }

        EXPECT_EQ(check_state, bit_board.GetState(get_move));
      }

      bit_board.SetState<kOpenPosition>(set_move);
    }
  }
};

TEST_F(BitBoardTest, DefaultConstructorTest){
  const auto &move_list = GetAllInBoardMove();
  const BitBoard bit_board;

  // 盤内は空点で初期化されていることを確認する
  for(const auto move : move_list)
  {
    const PositionState state = bit_board.GetState(move);
    EXPECT_EQ(kOpenPosition, state);
  }

  // 盤内以外の手では盤外が返ってくることを確認する
  EXPECT_EQ(kOverBoard, bit_board.GetState(kInvalidMove));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kNullMove));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kDeclareEndGame));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kUndefinedMove01));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kUndefinedMove28));
}

TEST_F(BitBoardTest, CopyConstructorTest){
  BitBoard bit_board_1;

  bit_board_1.SetState<kBlackStone>(kMoveHH);

  BitBoard bit_board_2(bit_board_1);
  EXPECT_TRUE(bit_board_1 == bit_board_2);
}

TEST_F(BitBoardTest, MoveListConstructorTest){
  const MoveList board_move_list("hhhg");
  BitBoard bit_board(board_move_list);

  const auto &move_list = GetAllInBoardMove();

  // 盤内が初期化されているか確認する
  for(const auto move : move_list){
    const PositionState state = bit_board.GetState(move);

    if(move == kMoveHH){
      EXPECT_EQ(kBlackStone, state);
    }else if(move == kMoveHG){
      EXPECT_EQ(kWhiteStone, state);
    }else{
      EXPECT_EQ(kOpenPosition, state);
    }
  }
  
  // 盤内以外の手では盤外が返ってくることを確認する
  EXPECT_EQ(kOverBoard, bit_board.GetState(kInvalidMove));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kNullMove));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kDeclareEndGame));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kUndefinedMove01));
  EXPECT_EQ(kOverBoard, bit_board.GetState(kUndefinedMove28));

  BitBoard bitboard_expect;
  bitboard_expect.SetState<kBlackStone>(kMoveHH);
  bitboard_expect.SetState<kWhiteStone>(kMoveHG);

  EXPECT_TRUE(bitboard_expect == bit_board);
}

TEST_F(BitBoardTest, strTest)
{
  BitBoard bit_board;

  {
    // 初期状態のテスト
    string board_str = bit_board.str();
    stringstream expect_ss;

    expect_ss << "  A B C D E F G H I J K L M N O " << endl;
    expect_ss << "A + --------------------------+ A " << endl;
    expect_ss << "B | . . . . . . . . . . . . . | B " << endl;
    expect_ss << "C | . . . . . . . . . . . . . | C " << endl;
    expect_ss << "D | . . * . . . . . . . * . . | D " << endl;
    expect_ss << "E | . . . . . . . . . . . . . | E " << endl;
    expect_ss << "F | . . . . . . . . . . . . . | F " << endl;
    expect_ss << "G | . . . . . . . . . . . . . | G " << endl;
    expect_ss << "H | . . . . . . * . . . . . . | H " << endl;
    expect_ss << "I | . . . . . . . . . . . . . | I " << endl;
    expect_ss << "J | . . . . . . . . . . . . . | J " << endl;
    expect_ss << "K | . . . . . . . . . . . . . | K " << endl;
    expect_ss << "L | . . * . . . . . . . * . . | L " << endl;
    expect_ss << "M | . . . . . . . . . . . . . | M " << endl;
    expect_ss << "N | . . . . . . . . . . . . . | N " << endl;
    expect_ss << "O + --------------------------+ O " << endl;
    expect_ss << "  A B C D E F G H I J K L M N O " << endl;
    
    EXPECT_TRUE(board_str == expect_ss.str());
  }
  {
    // 黒石、白石を設定
    bit_board.SetState<kBlackStone>(kMoveHH);
    bit_board.SetState<kWhiteStone>(kMoveHG);
    string board_str = bit_board.str();

    stringstream expect_ss;
    
    expect_ss << "  A B C D E F G H I J K L M N O " << endl;
    expect_ss << "A + --------------------------+ A " << endl;
    expect_ss << "B | . . . . . . . . . . . . . | B " << endl;
    expect_ss << "C | . . . . . . . . . . . . . | C " << endl;
    expect_ss << "D | . . * . . . . . . . * . . | D " << endl;
    expect_ss << "E | . . . . . . . . . . . . . | E " << endl;
    expect_ss << "F | . . . . . . . . . . . . . | F " << endl;
    expect_ss << "G | . . . . . . o . . . . . . | G " << endl;
    expect_ss << "H | . . . . . . x . . . . . . | H " << endl;
    expect_ss << "I | . . . . . . . . . . . . . | I " << endl;
    expect_ss << "J | . . . . . . . . . . . . . | J " << endl;
    expect_ss << "K | . . . . . . . . . . . . . | K " << endl;
    expect_ss << "L | . . * . . . . . . . * . . | L " << endl;
    expect_ss << "M | . . . . . . . . . . . . . | M " << endl;
    expect_ss << "N | . . . . . . . . . . . . . | N " << endl;
    expect_ss << "O + --------------------------+ O " << endl;
    expect_ss << "  A B C D E F G H I J K L M N O " << endl;

    EXPECT_TRUE(board_str == expect_ss.str());
  }
}

TEST_F(BitBoardTest, GetLineNeighborhoodStateBitTest)
{
  {
    // 盤の中央に黒石、白石があるケース(N=1)
    MoveList move_list("hhhg");
    BitBoard bit_board(move_list);
    
    array<StateBit, kBoardDirectionNum> line_neighborhood;
    bit_board.GetLineNeighborhoodStateBit<1>(kMoveHH, &line_neighborhood);

    const StateBit lateral_state_bit = GetStateBit("OBOXXXXXX");
    EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

    const StateBit vertical_state_bit = GetStateBit("OBWXXXXXX");
    EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

    const StateBit left_diagonal_state_bit = GetStateBit("OBOXXXXXX");
    EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

    const StateBit right_diagonal_state_bit = GetStateBit("OBOXXXXXX");
    EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
  }
  {
    // 盤の中央に黒石、白石があるケース(N=7)
    MoveList move_list("hhhg");
    BitBoard bit_board(move_list);
    
    array<StateBit, kBoardDirectionNum> line_neighborhood;
    bit_board.GetLineNeighborhoodStateBit<7>(kMoveHH, &line_neighborhood);

    const StateBit lateral_state_bit = GetStateBit("OOOOOOOBOOOOOOO");
    EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

    const StateBit vertical_state_bit = GetStateBit("OOOOOOOBWOOOOOO");
    EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

    const StateBit left_diagonal_state_bit = GetStateBit("OOOOOOOBOOOOOOO");
    EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

    const StateBit right_diagonal_state_bit = GetStateBit("OOOOOOOBOOOOOOO");
    EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
  }
  {
    // 四隅に黒石、白石があるケース
    MoveList move_list("aaoaaooo");
    BitBoard bit_board(move_list);
    //   A B C D E F G H I J K L M N O 
    // A x --------------------------o A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . . . . . . . . . | G 
    // H | . . . . . . * . . . . . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O x --------------------------o O 
    //   A B C D E F G H I J K L M N O 
    {
      // 左上隅(N=1)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<1>(kMoveAA, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("OBXXXXXXX");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("OBXXXXXXX");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("BXXXXXXX");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("OBXXXXXXX");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 左上隅(N=7)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<7>(kMoveAA, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("OOOOOOOBXXXXXXX");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("OOOOOOOBXXXXXXX");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("OOOOOOXBXXXXXXX");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("OOOOOOOBXXXXXXX");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 右上隅(N=1)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<1>(kMoveOA, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("WOXXXXXX");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("OWXXXXXXX");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("OWXXXXXXX");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("WXXXXXXX");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 右上隅(N=7)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<7>(kMoveOA, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("WOOOOOOO");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("OOOOOOOWXOOOOOO");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("OOOOOOOWXXXXXXX");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("OOOOOOXWXXXXXXX");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 左下隅(N=1)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<1>(kMoveAO, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("OBXXXXXXX");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("BOXXXXXX");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("BOXXXXXX");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("BXXXXXXX");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 左下隅(N=7)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<7>(kMoveAO, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("OOOOOOOBXOOOOOO");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("BOOOOOOO");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("OOOOOXXBOOOOOOO");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("OOOOOOXBXOOOOOO");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 右下隅(N=1)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<1>(kMoveOO, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("WOXXXXXX");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("WOXXXXXX");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("WXXXXXXX");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("WOXXXXXX");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 右下隅(N=7)
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<7>(kMoveOO, &line_neighborhood);

      const StateBit lateral_state_bit = GetStateBit("WOOOOOOO");
      EXPECT_EQ(lateral_state_bit, line_neighborhood[kLateralDirection]);

      const StateBit vertical_state_bit = GetStateBit("WOOOOOOO");
      EXPECT_EQ(vertical_state_bit, line_neighborhood[kVerticalDirection]);

      const StateBit left_diagonal_state_bit = GetStateBit("OOOOOOXWXOOOOOO");
      EXPECT_EQ(left_diagonal_state_bit, line_neighborhood[kLeftDiagonalDirection]);

      const StateBit right_diagonal_state_bit = GetStateBit("OOOOOXXWOOOOOOO");
      EXPECT_EQ(right_diagonal_state_bit, line_neighborhood[kRightDiagonalDirection]);
    }
    {
      // 盤外のケース
      array<StateBit, kBoardDirectionNum> line_neighborhood;
      bit_board.GetLineNeighborhoodStateBit<7>(kNullMove, &line_neighborhood);

      for(auto value : line_neighborhood){
        EXPECT_EQ(0, value);
      }
    }
  }
}

TEST_F(BitBoardTest, GetSetStateTest)
{
  GetSetStateTest();
}

TEST_F(BitBoardTest, CompareOperTest){
  BitBoard bit_board_1, bit_board_2;

  EXPECT_TRUE(bit_board_1 == bit_board_1);
  EXPECT_TRUE(bit_board_1 == bit_board_2);

  bit_board_1.SetState<kBlackStone>(kMoveHH);
  EXPECT_TRUE(bit_board_1 == bit_board_1);
  EXPECT_TRUE(bit_board_1 != bit_board_2);
  
  bit_board_2.SetState<kBlackStone>(kMoveHH);
  EXPECT_TRUE(bit_board_1 == bit_board_2);
}

TEST_F(BitBoardTest, AssignOperTest){
  BitBoard bit_board_1, bit_board_2;

  bit_board_1.SetState<kBlackStone>(kMoveHH);
  EXPECT_TRUE(bit_board_1 != bit_board_2);

  bit_board_2 = bit_board_1;
  EXPECT_TRUE(bit_board_1 == bit_board_2);
}
}
