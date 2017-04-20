// @brief メンバ関数のテスト
#include <stack>

#include "gtest/gtest.h"

#include "Move.h"
#include "MoveList.h"
#include "BoardOpenState.h"
#include "Board.h"

using namespace std;

namespace realcore
{

class BoardTest
: public ::testing::Test
{
public:
  void DefaultConstructorTest(){
    const Board board;
    const BitBoard bit_board;
    const MoveList move_list;

    EXPECT_TRUE(board.bit_board_ == bit_board);
    EXPECT_TRUE(board.move_list_ == move_list);
  }

  void CopyConstructorTest(){
    Board board_1;
    board_1.MakeMove(kMoveHH);

    const Board board_2(board_1);
    EXPECT_TRUE(board_1.bit_board_ == board_2.bit_board_);
    EXPECT_TRUE(board_1.move_list_ == board_2.move_list_);
  }

  void MoveListConstructorTest(){
    const MoveList board_move_list("hhhg");
    const Board board(board_move_list);

    Board board_expect;
    board_expect.MakeMove(kMoveHH);
    board_expect.MakeMove(kMoveHG);

    EXPECT_TRUE(board.bit_board_ == board_expect.bit_board_);
    EXPECT_TRUE(board.move_list_ == board_expect.move_list_);
  }

  void BoardOpenStateUpdateTest(){
  //   A B C D E F G H I J K L M N O 
  // A + --------------------------+ A 
  // B | . x . . x x x o . . . . . | B 
  // C | . . o . o . . o x x . . . | C 
  // D | . . * o . x . o o o x . . | D 
  // E | . . . x o . o . x o . o . | E 
  // F | . . . . . o x x . o o o x | F 
  // G | . . . . x o x o x x x x o | G 
  // H | . . . . o . x . . o o . . | H 
  // I | . . . . . x o . . . x . . | I 
  // J | . . x . . . . . . . . . . | J 
  // K | . . . . . . . . . . . . . | K 
  // L | . . * . . . . . . . * . . | L 
  // M | . . . . . . . . . . . . . | M 
  // N | . . . . . . . . . . . . . | N 
  // O + --------------------------+ O 
  //   A B C D E F G H I J K L M N O 
    Board board(MoveList("hhhijggggiigfgfhhfhedjidifgfgdkfjekdjcjdldfeeeedhgfcfbkekcichblfmgmfnfmekgdccblhlikhlgnggbib"));
    auto board_open_state = board.board_open_state_list_.back();

    ASSERT_EQ(2ULL, board_open_state.GetList(kNextFourBlack).size());
  }

  void IsNormalMoveTest(){
    const auto in_board_move_list = GetAllInBoardMove();
    
    {
      // 相手に四がないケース
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
        bool is_normal = (board.bit_board_.GetState(move) == kOpenPosition) && move != kMoveKH;

        if(is_normal){
          EXPECT_TRUE(board.IsNormalMove<kBlackTurn>(move));
        }else{
          EXPECT_FALSE(board.IsNormalMove<kBlackTurn>(move));
        }
      }
    }
    {
      // 相手に四があるケース
      //   A B C D E F G H I J K L M N O 
      // A + --------------------------o A 
      // B | . . . . . . . . . . . . . | B 
      // C | . x . . . . . . . . . . . | C 
      // D | . . * . . . . . . . * . . | D 
      // E | . . . . . . . . . . . . . | E 
      // F | . . . . x . . . . . . . . | F 
      // G | . . . . . x . o . . . . . | G 
      // H | . . . . . . x . . . . . . o H 
      // I | . . . . . x . o . . . . . | I 
      // J | . . . . x o . . . . . . . | J 
      // K | . . . . . . . . . . . . . | K 
      // L | . . x . . . . . . . * . . | L 
      // M | . . . . . . . . . . . . . | M 
      // N | . . . . . . . . . . . . . | N 
      // O + --------------------------o O 
      //   A B C D E F G H I J K L M N O 
      Board board(MoveList("hhiggiiifjooggoaccohffgjdl"));

      for(const auto move : in_board_move_list){
        bool is_normal = move == kMoveEK;

        if(is_normal){
          EXPECT_TRUE(board.IsNormalMove<kWhiteTurn>(move));
        }else{
          EXPECT_FALSE(board.IsNormalMove<kWhiteTurn>(move));
        }
      }
    }
    {
      // 盤外
      Board board;

      for(const auto move : GetAllMove()){
        if(IsInBoardMove(move)){
          continue;
        }

        bool is_normal = move == kNullMove;

        if(is_normal){
          EXPECT_TRUE(board.IsNormalMove<kBlackTurn>(move));
        }else{
          EXPECT_FALSE(board.IsNormalMove<kBlackTurn>(move));
        }
      }
    }
  }

  void BitBoardTest(){
    Board board;
    BitBoard bit_board;

    ASSERT_TRUE(board.bit_board_ == bit_board);

    // 黒の着手
    board.MakeMove(kMoveHH);
    bit_board.SetState<kBlackStone>(kMoveHH);

    ASSERT_TRUE(board.bit_board_ == bit_board);

    // 白の着手
    board.MakeMove(kMoveHG);
    bit_board.SetState<kWhiteStone>(kMoveHG);

    ASSERT_TRUE(board.bit_board_ == bit_board);

    // 1手戻す
    board.UndoMove();
    bit_board.SetState<kOpenPosition>(kMoveHG);

    ASSERT_TRUE(board.bit_board_ == bit_board);
    
    // 1手戻す
    board.UndoMove();
    bit_board.SetState<kOpenPosition>(kMoveHH);

    ASSERT_TRUE(board.bit_board_ == bit_board);
  }

  void MoveListTest(){
    Board board;
    MoveList move_list;

    ASSERT_TRUE(board.move_list_ == move_list);

    // 黒の着手
    board.MakeMove(kMoveHH);
    move_list += kMoveHH;

    ASSERT_TRUE(board.move_list_ == move_list);

    // 白の着手
    board.MakeMove(kMoveHG);
    move_list += kMoveHG;

    ASSERT_TRUE(board.move_list_ == move_list);

    // 1手戻す
    board.UndoMove();
    --move_list;

    ASSERT_TRUE(board.move_list_ == move_list);
    
    // 1手戻す
    board.UndoMove();
    --move_list;

    ASSERT_TRUE(board.move_list_ == move_list);
  }

  void BoardOpenStateStackTest(){
    Board board;

    {
      // 初期化時は要素数1で初期状態のBoardOpenStateがtop
      ASSERT_EQ(1, board.board_open_state_list_.size());
      
      const auto &board_open_state = board.board_open_state_list_.back();
      BoardOpenState init_state;
      EXPECT_TRUE(board_open_state == init_state);
    }

    {
      // MakeMove時に更新されているかチェック
      MoveList board_move_list("hhigff");

      stack<BoardOpenState> state_stack;
      state_stack.emplace();

      BitBoard bit_board;
      PlayerTurn player_turn = kBlackTurn;

      for(const auto move : board_move_list){
        const auto prev_board_open_state = board.board_open_state_list_.back();
        board.MakeMove(move);

        bit_board.SetState(move, GetPlayerStone(player_turn));
        BoardOpenState update_state(prev_board_open_state, player_turn == kBlackTurn, move, bit_board);

        const auto &board_open_state = board.board_open_state_list_.back();
        EXPECT_TRUE(board_open_state == update_state);

        state_stack.emplace(update_state);
        player_turn = GetOpponentTurn(player_turn);
      }

      // UndoMove時に更新されているかチェック
      for(size_t i=0; i<board_move_list.size(); i++){
        board.UndoMove();
        state_stack.pop();

        EXPECT_TRUE(board.board_open_state_list_.back() == state_stack.top());
      }
    }
  }
};

TEST_F(BoardTest, DefaultConstructorTest){
  DefaultConstructorTest();
}

TEST_F(BoardTest, CopyConstructorTest){
  CopyConstructorTest();
}

TEST_F(BoardTest, MoveListConstructorTest){
  MoveListConstructorTest();
}

TEST_F(BoardTest, CompareOperTest){
  Board board_1, board_2;

  EXPECT_TRUE(board_1 == board_1);
  EXPECT_TRUE(board_1 == board_2);

  board_1.MakeMove(kMoveHH);
  EXPECT_TRUE(board_1 == board_1);
  EXPECT_TRUE(board_1 != board_2);
  
  board_2.MakeMove(kMoveHH);
  EXPECT_TRUE(board_1 == board_2);
}

TEST_F(BoardTest, AssignOperTest){
  Board board_1, board_2;

  board_1.MakeMove(kMoveHH);
  EXPECT_TRUE(board_1 != board_2);

  board_2 = board_1;
  EXPECT_TRUE(board_1 == board_2);
}

TEST_F(BoardTest, IsNormalMoveTest)
{
  IsNormalMoveTest();
}

TEST_F(BoardTest, IsTerminateMoveTest)
{
  const auto in_board_move_list = GetAllInBoardMove();

  {
    // 達四(黒番)ができるケース
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . . o o o . . . . | G 
    // H | . . . . . . x x x . . . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    Board board(MoveList("hhhgihigjhjg"));
    
    for(const auto move : in_board_move_list){
      if(!board.IsNormalMove<kBlackTurn>(move)){
        continue;
      }
      
      const bool is_terminate = board.IsTerminateMove<kBlackTurn>(move);
      bool expect = (move == kMoveGH) || (move == kMoveKH);

      EXPECT_EQ(expect, is_terminate);
    }
  }
  {
    // 達四(白番)
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------x A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . x . o o o . . . . | G 
    // H | . . . . . . x x . . . . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    Board board(MoveList("hhhgihigfgjgoa"));
    
    for(const auto move : in_board_move_list){
      if(!board.IsNormalMove<kWhiteTurn>(move)){
        continue;
      }
      
      const bool is_terminate = board.IsTerminateMove<kWhiteTurn>(move);
      bool expect = (move == kMoveKG);

      EXPECT_EQ(expect, is_terminate);
    }
  }
  {
    // 四々(白番)
    //   A B C D E F G H I J K L M N O 
    // A x --------------x ----------+ A 
    // B | . . . . . . . x . . . . . | B 
    // C | . . . . . . . o . . . . . | C 
    // D | . . * . . . . o . . * . . | D 
    // E | . . . . . . . o . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . x o o o . . . . . . | G 
    // H | . . . . x x x . . . . . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    Board board(MoveList("hhhgghggegfgaaidiaicibiefh"));
    
    for(const auto move : in_board_move_list){
      if(!board.IsNormalMove<kWhiteTurn>(move)){
        continue;
      }
      
      const bool is_terminate = board.IsTerminateMove<kWhiteTurn>(move);
      bool expect = (move == kMoveIG);

      EXPECT_EQ(expect, is_terminate);
    }
  }
  {
    // 四ノビで極める
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . x . . . . . * . . | D 
    // E | . . . o . o . . . . . . . | E 
    // F | . . . . . . o . x . . . . | F 
    // G | . . . . . . . o x . . . . | G 
    // H | . . . . . . x x . . . . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    Board board(MoveList("hhigihhfjfgefdeejg"));
    
    for(const auto move : in_board_move_list){
      if(!board.IsNormalMove<kWhiteTurn>(move)){
        continue;
      }
      
      const bool is_terminate = board.IsTerminateMove<kWhiteTurn>(move);
      bool expect = (move == kMoveKI);

      EXPECT_EQ(expect, is_terminate);
    }
  }
  {
    // 否 終端手
    // 点JHは禁点だが点IIに四ノビすると点JHは否禁になるので終端手ではない
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . o . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . x . . | F 
    // G | . . . . . . . . . x . . . | G 
    // H | . . . . . x x . . . . . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . o . x . . . . | J 
    // K | . . . . . o . . . . . . . | K 
    // L | . . * . o . . . . . o . . | L 
    // M | . . . x . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    Board board(MoveList("hhhjlfgkjjllkgddghflem"));
    
    for(const auto move : in_board_move_list){
      if(!board.IsNormalMove<kWhiteTurn>(move)){
        continue;
      }
      
      const bool is_terminate = board.IsTerminateMove<kWhiteTurn>(move);
      EXPECT_FALSE(is_terminate);
    }
  }
}

TEST_F(BoardTest, BitBoardTest)
{
  BitBoardTest();
}

TEST_F(BoardTest, MoveListTest)
{
  MoveListTest();
}

TEST_F(BoardTest, BoardOpenStateStackTest)
{
  BoardOpenStateStackTest();
}

TEST_F(BoardTest, UpdateTest)
{
  BoardOpenStateUpdateTest();
}

TEST_F(BoardTest, IsOpponentFour)
{
  {
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------o A 
    // B | . . . . . . . . . . . . . | B 
    // C | . x . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . x . . . . . . . . | F 
    // G | . . . . . x . o . . . . . | G 
    // H | . . . . . . x . . . . . . o H 
    // I | . . . . . x . o . . . . . | I 
    // J | . . . . x o . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . x . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------o O 
    //   A B C D E F G H I J K L M N O 
    Board board(MoveList("hhiggiiifjooggoaccohffgjdl"));
    MovePosition guard_move;
    const bool is_opponent_four = board.IsOpponentFour(&guard_move);

    ASSERT_TRUE(is_opponent_four);
    ASSERT_EQ(kMoveEK, guard_move);
  }
  {
    // 最終手がPassの場合
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------o A 
    // B | . . . . . . . . . . . . . | B 
    // C | . x . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . x . . . . . . . . | F 
    // G | . . . . . x . o . . . . . | G 
    // H | . . . . . . x . . . . . . o H 
    // I | . . . . . x . o . . . . . | I 
    // J | . . . . x o . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------o O 
    //   A B C D E F G H I J K L M N O 
    Board board(MoveList("hhiggiiifjooggoaccohffgjpp"));
    MovePosition guard_move;
    const bool is_opponent_four = board.IsOpponentFour(&guard_move);
    
    ASSERT_FALSE(is_opponent_four);
  }
}

TEST_F(BoardTest, TerminateCheckTest)
{
  {
    // 達四(黒番)ができるケース
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . . o o o . . . . | G 
    // H | . . . . . . x x x . . . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    Board board(MoveList("hhhgihigjhjg"));
    MovePosition terminating_move;
    const bool is_terminate = board.TerminateCheck<kBlackTurn>(&terminating_move);
    
    ASSERT_TRUE(is_terminate);
    ASSERT_TRUE(terminating_move == kMoveGH || terminating_move == kMoveKH);
  }
  {
    // 四ノビで極める
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . x . . . . . * . . | D 
    // E | . . . o . o . . . . . . . | E 
    // F | . . . . . . o . x . . . . | F 
    // G | . . . . . . . o x . . . . | G 
    // H | . . . . . . x x . . . . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    Board board(MoveList("hhigihhfjfgefdeejg"));
    MovePosition terminating_move;
    const bool is_terminate = board.TerminateCheck<kWhiteTurn>(&terminating_move);
    
    ASSERT_TRUE(is_terminate);
    ASSERT_TRUE(terminating_move == kMoveKI);
  }
  {
    // 相手に四ノビがあり終端しない
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . x o o o o . . . | G 
    // H | . . . . . . x . . . . . . | H 
    // I | . . . . . x . x . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    Board board(MoveList("hhhgggiggijgiikg"));
    MovePosition terminating_move;
    const bool is_terminate = board.TerminateCheck<kBlackTurn>(&terminating_move);
    
    ASSERT_FALSE(is_terminate);
  }
  {
    // 相手の四ノビをノリ返して終端する場合
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . x . . . . | F 
    // G | . . . x o o o . o . . . . | G 
    // H | . . . . . . x . . . . . . | H 
    // I | . . . . . x . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    Board board(MoveList("hhhgjfggegfggijg"));
    MovePosition terminating_move;
    const bool is_terminate = board.TerminateCheck<kBlackTurn>(&terminating_move);
    
    ASSERT_TRUE(is_terminate);
    ASSERT_TRUE(terminating_move == kMoveIG);
  }
}

TEST_F(BoardTest, TerminateCheckDebug)
{
  Board board(MoveList("hhgihgfiigeggfgkijhejjkfeikgjklekilldfjmidmjmdgddcjged"));
  MovePosition terminating_move;
  const bool is_terminate = board.TerminateCheck<kWhiteTurn>(&terminating_move);
  
  ASSERT_FALSE(is_terminate);
}

}