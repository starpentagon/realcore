#include <array>

#include "gtest/gtest.h"

#include "Move.h"
#include "MoveList.h"
#include "Board.h"

using namespace std;

namespace realcore
{

class BoardTest
: public ::testing::Test
{
public:
  void GetSetStateTest(){
    Board board;
    const auto &all_move_list = GetAllMove();

    // 黒石
    for(auto set_move : all_move_list){
      board.SetState<kBlackStone>(set_move);

      for(auto get_move : all_move_list){
        // 設定した位置では黒石が、それ以外は空点が設定されていることを確認する
        PositionState check_state = kOverBoard;

        if(IsInBoardMove(get_move)){
          check_state = set_move == get_move ? kBlackStone : kOpenPosition;
        }

        EXPECT_EQ(check_state, board.GetState(get_move));
      }

      board.SetState<kOpenPosition>(set_move);
    }

    // 白石
    for(auto set_move : all_move_list){
      board.SetState<kWhiteStone>(set_move);

      for(auto get_move : all_move_list){
        // 設定した位置では白石が、それ以外は空点が設定されていることを確認する
        PositionState check_state = kOverBoard;

        if(IsInBoardMove(get_move)){
          check_state = set_move == get_move ? kWhiteStone : kOpenPosition;
        }

        EXPECT_EQ(check_state, board.GetState(get_move));
      }

      board.SetState<kOpenPosition>(set_move);
    }
  }

  void GetBitBoardIndexListTest()
  {
    Board board;
    {
      // 左上の境界
      constexpr Cordinate x = 1, y = 1;
      array<size_t, kBoardDirectionNum> index_list;
      board.GetBitBoardIndexList(x, y, &index_list);

      EXPECT_EQ(0, index_list[kLateralDirection]);
      EXPECT_EQ(8, index_list[kVerticalDirection]);
      EXPECT_EQ(16, index_list[kLeftDiagonalDirection]);
      EXPECT_EQ(31, index_list[kRightDiagonalDirection]);
    }
    {
      // 盤内
      constexpr Cordinate x = 8, y = 9;
      array<size_t, kBoardDirectionNum> index_list;
      board.GetBitBoardIndexList(x, y, &index_list);

      EXPECT_EQ(4, index_list[kLateralDirection]);
      EXPECT_EQ(12, index_list[kVerticalDirection]);
      EXPECT_EQ(23, index_list[kLeftDiagonalDirection]);
      EXPECT_EQ(31, index_list[kRightDiagonalDirection]);
    }      
    {
      // 右下の境界
      constexpr Cordinate x = 15, y = 15;
      array<size_t, kBoardDirectionNum> index_list;
      board.GetBitBoardIndexList(x, y, &index_list);

      EXPECT_EQ(7, index_list[kLateralDirection]);
      EXPECT_EQ(15, index_list[kVerticalDirection]);
      EXPECT_EQ(22, index_list[kLeftDiagonalDirection]);
      EXPECT_EQ(31, index_list[kRightDiagonalDirection]);
    }      
  }

  void GetBitBoardShiftListTest(){
    Board board;
    {
      // 左上の境界
      constexpr Cordinate x = 1, y = 1;
      array<size_t, kBoardDirectionNum> shift_list;
      board.GetBitBoardShiftList(x, y, &shift_list);

      EXPECT_EQ(34, shift_list[kLateralDirection]);
      EXPECT_EQ(34, shift_list[kVerticalDirection]);
      EXPECT_EQ(0, shift_list[kLeftDiagonalDirection]);
      EXPECT_EQ(0, shift_list[kRightDiagonalDirection]);
    }
    {
      // 盤内
      constexpr Cordinate x = 8, y = 9;
      array<size_t, kBoardDirectionNum> shift_list;
      board.GetBitBoardShiftList(x, y, &shift_list);

      EXPECT_EQ(48, shift_list[kLateralDirection]);
      EXPECT_EQ(18, shift_list[kVerticalDirection]);
      EXPECT_EQ(48, shift_list[kLeftDiagonalDirection]);
      EXPECT_EQ(48, shift_list[kRightDiagonalDirection]);
    }
    {
      // 右下の境界
      constexpr Cordinate x = 15, y = 15;
      array<size_t, kBoardDirectionNum> shift_list;
      board.GetBitBoardShiftList(x, y, &shift_list);

      EXPECT_EQ(62, shift_list[kLateralDirection]);
      EXPECT_EQ(62, shift_list[kVerticalDirection]);
      EXPECT_EQ(28, shift_list[kLeftDiagonalDirection]);
      EXPECT_EQ(28, shift_list[kRightDiagonalDirection]);
    }    
  }

  void GetBitBoardIndexTest(){
     const Board board;
     // 各配列Indexの境界値テスト
     // 横方向
     EXPECT_EQ(0, board.GetBitBoardIndex(0));
     EXPECT_EQ(0, board.GetBitBoardIndex(31));
     EXPECT_EQ(1, board.GetBitBoardIndex(32));
     EXPECT_EQ(1, board.GetBitBoardIndex(63));
     EXPECT_EQ(2, board.GetBitBoardIndex(64));
     EXPECT_EQ(2, board.GetBitBoardIndex(95));
     EXPECT_EQ(3, board.GetBitBoardIndex(96));
     EXPECT_EQ(3, board.GetBitBoardIndex(127));
     EXPECT_EQ(4, board.GetBitBoardIndex(128));
     EXPECT_EQ(4, board.GetBitBoardIndex(159));
     EXPECT_EQ(5, board.GetBitBoardIndex(160));
     EXPECT_EQ(5, board.GetBitBoardIndex(191));
     EXPECT_EQ(6, board.GetBitBoardIndex(192));
     EXPECT_EQ(6, board.GetBitBoardIndex(223));
     EXPECT_EQ(7, board.GetBitBoardIndex(224));
     EXPECT_EQ(7, board.GetBitBoardIndex(255));

     // 縦方向
     EXPECT_EQ(8, board.GetBitBoardIndex(256));
     EXPECT_EQ(8, board.GetBitBoardIndex(287));
     EXPECT_EQ(9, board.GetBitBoardIndex(288));
     EXPECT_EQ(9, board.GetBitBoardIndex(319));
     EXPECT_EQ(10, board.GetBitBoardIndex(320));
     EXPECT_EQ(10, board.GetBitBoardIndex(351));
     EXPECT_EQ(11, board.GetBitBoardIndex(352));
     EXPECT_EQ(11, board.GetBitBoardIndex(383));
     EXPECT_EQ(12, board.GetBitBoardIndex(384));
     EXPECT_EQ(12, board.GetBitBoardIndex(415));
     EXPECT_EQ(13, board.GetBitBoardIndex(416));
     EXPECT_EQ(13, board.GetBitBoardIndex(447));
     EXPECT_EQ(14, board.GetBitBoardIndex(448));
     EXPECT_EQ(14, board.GetBitBoardIndex(479));
     EXPECT_EQ(15, board.GetBitBoardIndex(480));
     EXPECT_EQ(15, board.GetBitBoardIndex(511));

     // 左斜め方向
     EXPECT_EQ(16, board.GetBitBoardIndex(512));
     EXPECT_EQ(16, board.GetBitBoardIndex(543));
     EXPECT_EQ(17, board.GetBitBoardIndex(544));
     EXPECT_EQ(17, board.GetBitBoardIndex(575));
     EXPECT_EQ(18, board.GetBitBoardIndex(576));
     EXPECT_EQ(18, board.GetBitBoardIndex(607));
     EXPECT_EQ(19, board.GetBitBoardIndex(608));
     EXPECT_EQ(19, board.GetBitBoardIndex(639));
     EXPECT_EQ(20, board.GetBitBoardIndex(640));
     EXPECT_EQ(20, board.GetBitBoardIndex(671));
     EXPECT_EQ(21, board.GetBitBoardIndex(672));
     EXPECT_EQ(21, board.GetBitBoardIndex(703));
     EXPECT_EQ(22, board.GetBitBoardIndex(704));
     EXPECT_EQ(22, board.GetBitBoardIndex(735));
     EXPECT_EQ(23, board.GetBitBoardIndex(736));
     EXPECT_EQ(23, board.GetBitBoardIndex(767));

     // 右斜め方向
     EXPECT_EQ(24, board.GetBitBoardIndex(768));
     EXPECT_EQ(24, board.GetBitBoardIndex(799));
     EXPECT_EQ(25, board.GetBitBoardIndex(800));
     EXPECT_EQ(25, board.GetBitBoardIndex(831));
     EXPECT_EQ(26, board.GetBitBoardIndex(832));
     EXPECT_EQ(26, board.GetBitBoardIndex(863));
     EXPECT_EQ(27, board.GetBitBoardIndex(864));
     EXPECT_EQ(27, board.GetBitBoardIndex(895));
     EXPECT_EQ(28, board.GetBitBoardIndex(896));
     EXPECT_EQ(28, board.GetBitBoardIndex(927));
     EXPECT_EQ(29, board.GetBitBoardIndex(928));
     EXPECT_EQ(29, board.GetBitBoardIndex(959));
     EXPECT_EQ(30, board.GetBitBoardIndex(960));
     EXPECT_EQ(30, board.GetBitBoardIndex(991));
     EXPECT_EQ(31, board.GetBitBoardIndex(992));
     EXPECT_EQ(31, board.GetBitBoardIndex(1023));
  }

  void GetBitBoardShiftTest(){
    // シフト量の全数テスト
    const Board board;

    for(size_t i=0; i<1024; i++){
      const size_t shift_val = 2 * (i % 32);
      EXPECT_EQ(shift_val, board.GetBitBoardShift(i));
    }
  }

  void IsForbiddenMoveTest()
  {
    // 黒番
    const auto in_board_move_list = GetAllInBoardMove();

    {
      // 長連
      //   A B C D E F G H I J K L M N O 
      // A + --------------------------+ A 
      // B | . . . . . . . . . . . . . | B 
      // C | . . . . . . . . . . . . . | C 
      // D | . . * . . . . . . . * . . | D 
      // E | . . . . . . . . . . . . . | E 
      // F | . . . . . . . . . . . . . | F 
      // G | . . . . . . o o . . . . . | G 
      // H | . . . . . o x x x . x x . | H 
      // I | . . . . . . o o . . . . . | I 
      // J | . . . . . . . . . . . . . | J 
      // K | . . . . . . . . . . . . . | K 
      // L | . . * . . . . . . . * . . | L 
      // M | . . . . . . . . . . . . . | M 
      // N | . . . . . . . . . . . . . | N 
      // O + --------------------------+ O 
      //   A B C D E F G H I J K L M N O 
      Board board(MoveList("hhigihghjhhgmhhilhii"));

      for(const auto move : in_board_move_list){
        if(board.GetState(move) != kOpenPosition){
          continue;
        }
        
        if(move == kMoveKH){
          EXPECT_TRUE(board.IsForbiddenMove<kBlackTurn>(move));
        }else{
          EXPECT_FALSE(board.IsForbiddenMove<kBlackTurn>(move));
        }

        EXPECT_FALSE(board.IsForbiddenMove<kWhiteTurn>(move));
      }
    }
    {
      // 四々(1直線)
      ASSERT_TRUE(false);
    }
    {
      // 四々(2直線, 四のみ)
      ASSERT_TRUE(false);
    }
    {
      // 四々(2直線, 達四のみ)
      ASSERT_TRUE(false);
    }
    {
      // 四々(2直線, 達四、四混在)
      ASSERT_TRUE(false);
    }

    // 盤外
    ASSERT_TRUE(false);
  }
};

TEST_F(BoardTest, DefaultConstructorTest){
  const auto &move_list = GetAllInBoardMove();
  const Board board;

  // 盤内は空点で初期化されていることを確認する
  for(const auto move : move_list)
  {
    const PositionState state = board.GetState(move);
    EXPECT_EQ(kOpenPosition, state);
  }

  // 盤内以外の手では盤外が返ってくることを確認する
  EXPECT_EQ(kOverBoard, board.GetState(kInvalidMove));
  EXPECT_EQ(kOverBoard, board.GetState(kNullMove));
  EXPECT_EQ(kOverBoard, board.GetState(kDeclareEndGame));
  EXPECT_EQ(kOverBoard, board.GetState(kUndefinedMove01));
  EXPECT_EQ(kOverBoard, board.GetState(kUndefinedMove28));
}

TEST_F(BoardTest, MoveListConstructorTest){
  const MoveList board_move_list("hhhg");
  Board board(board_move_list);

  const auto &move_list = GetAllInBoardMove();

  // 盤内が初期化されているか確認する
  for(const auto move : move_list){
    const PositionState state = board.GetState(move);

    if(move == kMoveHH){
      EXPECT_EQ(kBlackStone, state);
    }else if(move == kMoveHG){
      EXPECT_EQ(kWhiteStone, state);
    }else{
      EXPECT_EQ(kOpenPosition, state);
    }
  }
  
  // 盤内以外の手では盤外が返ってくることを確認する
  EXPECT_EQ(kOverBoard, board.GetState(kInvalidMove));
  EXPECT_EQ(kOverBoard, board.GetState(kNullMove));
  EXPECT_EQ(kOverBoard, board.GetState(kDeclareEndGame));
  EXPECT_EQ(kOverBoard, board.GetState(kUndefinedMove01));
  EXPECT_EQ(kOverBoard, board.GetState(kUndefinedMove28));
}

TEST_F(BoardTest, GetSetStateTest)
{
  GetSetStateTest();
}

TEST_F(BoardTest, GetBitBoardIndexTest){
  GetBitBoardIndexTest();
}

TEST_F(BoardTest, GetBitBoardShiftTest){
  GetBitBoardShiftTest();
}

TEST_F(BoardTest, GetBitBoardIndexListTest){
  GetBitBoardIndexListTest();
}

TEST_F(BoardTest, GetBitBoardShiftListTest){
  GetBitBoardShiftListTest();
}

TEST_F(BoardTest, IsInBoardTest)
{
  // 境界値テスト
  EXPECT_FALSE(IsInBoard(1, 0));
  EXPECT_FALSE(IsInBoard(0, 1));
  EXPECT_TRUE(IsInBoard(1, 1));
  EXPECT_TRUE(IsInBoard(15, 15));
}

TEST_F(BoardTest, strTest)
{
  // @todo テスト実装
  Board board;

  {
    // 初期状態のテスト
    string board_str = board.str();
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
    board.SetState<kBlackStone>(kMoveHH);
    board.SetState<kWhiteStone>(kMoveHG);
    string board_str = board.str();

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

TEST_F(BoardTest, GetLineNeighborhoodStateBitTest)
{
  {
    // 盤の中央に黒石、白石があるケース(N=1)
    MoveList move_list("hhhg");
    Board board(move_list);
    
    array<StateBit, kBoardDirectionNum> line_neighborhood;
    board.GetLineNeighborhoodStateBit<1>(kMoveHH, &line_neighborhood);

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
    Board board(move_list);
    
    array<StateBit, kBoardDirectionNum> line_neighborhood;
    board.GetLineNeighborhoodStateBit<7>(kMoveHH, &line_neighborhood);

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
    Board board(move_list);
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
      board.GetLineNeighborhoodStateBit<1>(kMoveAA, &line_neighborhood);

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
      board.GetLineNeighborhoodStateBit<7>(kMoveAA, &line_neighborhood);

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
      board.GetLineNeighborhoodStateBit<1>(kMoveOA, &line_neighborhood);

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
      board.GetLineNeighborhoodStateBit<7>(kMoveOA, &line_neighborhood);

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
      board.GetLineNeighborhoodStateBit<1>(kMoveAO, &line_neighborhood);

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
      board.GetLineNeighborhoodStateBit<7>(kMoveAO, &line_neighborhood);

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
      board.GetLineNeighborhoodStateBit<1>(kMoveOO, &line_neighborhood);

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
      board.GetLineNeighborhoodStateBit<7>(kMoveOO, &line_neighborhood);

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
      board.GetLineNeighborhoodStateBit<7>(kNullMove, &line_neighborhood);

      for(auto value : line_neighborhood){
        EXPECT_EQ(0, value);
      }
    }
  }
}

TEST_F(BoardTest, IsForbiddenMoveTest)
{
  IsForbiddenMoveTest();
}

}   // namespace realcore
