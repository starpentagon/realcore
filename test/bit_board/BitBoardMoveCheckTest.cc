// @brief メンバ関数(指し手判定チェック)のテスト
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
};

TEST_F(BitBoardTest, IsFourMoveTest)
{
  const auto in_board_move_list = GetAllInBoardMove();

  {
    // 四（黒番）
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
    BitBoard bit_board(MoveList("hhiggiiifjooggoaccohffgj"));

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      MovePosition guard_move;
      const bool is_four = bit_board.IsFourMove<kBlackTurn>(move, &guard_move);

      if(move == kMoveEK){
        EXPECT_TRUE(is_four);
        EXPECT_EQ(kMoveDL, guard_move);
      }else if(move == kMoveDL){
        EXPECT_TRUE(is_four);
        EXPECT_EQ(kMoveEK, guard_move);
      }else{
        EXPECT_FALSE(is_four);
      }
    }
  }
  {
    // 四（白版）
    //   A B C D E F G H I J K L M N O 
    // A x --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . x . . . . . . . . | F 
    // G | . . . . . x o o . . o . . | G 
    // H | . . . . . . x . . . . . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhigaahggglgff"));

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      MovePosition guard_move;
      const bool is_four = bit_board.IsFourMove<kWhiteTurn>(move, &guard_move);

      if(move == kMoveJG){
        EXPECT_TRUE(is_four);
        EXPECT_EQ(kMoveKG, guard_move);
      }else if(move == kMoveKG){
        EXPECT_TRUE(is_four);
        EXPECT_EQ(kMoveJG, guard_move);
      }else{
        EXPECT_FALSE(is_four);
      }
    }
  }
}

}   // namespace realcore
