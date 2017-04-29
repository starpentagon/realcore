// @brief メンバ関数(禁手チェック)のテスト
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

TEST_F(BitBoardTest, IsOverlineTest)
{
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
    BitBoard bit_board(MoveList("hhigihghjhhgmhhilhii"));
    
    MoveBitSet move_bit_set;
    bit_board.EnumerateForbiddenMoves(&move_bit_set);

    ASSERT_EQ(1, move_bit_set.count());
    EXPECT_TRUE(move_bit_set[kMoveKH]);

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveKH){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
}

TEST_F(BitBoardTest, IsOneLineDoubleFourTest)
{
  const auto in_board_move_list = GetAllInBoardMove();

  {
    // 両頭の四々_1
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . o . o . . . . . o G 
    // H | . . . . . . x . . x x . x | H 
    // I | . . . . . . . . . o . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhignhggkhkilhog"));

    MoveBitSet move_bit_set;
    bit_board.EnumerateForbiddenMoves(&move_bit_set);

    ASSERT_EQ(1, move_bit_set.count());
    EXPECT_TRUE(move_bit_set[kMoveJH]);

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveJH){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));

        MoveBitSet influence_area;
        EXPECT_TRUE(bit_board.IsDoubleFourMove<kBlackTurn>(move, &influence_area));

        ASSERT_EQ(5, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveGH]);
        ASSERT_TRUE(influence_area[kMoveIH]);
        ASSERT_TRUE(influence_area[kMoveJH]);
        ASSERT_TRUE(influence_area[kMoveMH]);
        ASSERT_TRUE(influence_area[kMoveOH]);
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
        EXPECT_FALSE(bit_board.IsDoubleFourMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
  {
    // 両頭の四々_2
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . x . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . x . . . * . . | D 
    // E | . . . . . o . o . . . . . | E 
    // F | . . . . . . x . . . . . . | F 
    // G | . . . . . o . o . . . . . | G 
    // H | . . . . . . x . . . . . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhighfgghbgehdie"));

    MoveBitSet move_bit_set;
    bit_board.EnumerateForbiddenMoves(&move_bit_set);

    ASSERT_EQ(1, move_bit_set.count());
    EXPECT_TRUE(move_bit_set[kMoveHE]);

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveHE){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));

        MoveBitSet influence_area;
        EXPECT_TRUE(bit_board.IsDoubleFourMove<kBlackTurn>(move, &influence_area));

        ASSERT_EQ(5, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveHA]);
        ASSERT_TRUE(influence_area[kMoveHC]);
        ASSERT_TRUE(influence_area[kMoveHE]);
        ASSERT_TRUE(influence_area[kMoveHG]);
        ASSERT_TRUE(influence_area[kMoveHI]);
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
        EXPECT_FALSE(bit_board.IsDoubleFourMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
  {
    // 両頭の四々_3
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . . . o . . . o . | G 
    // H | . . . . . . x . x x . . x | H 
    // I | . . . . . . . o . . o . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhigjhiinhmgkhli"));

    MoveBitSet move_bit_set;
    bit_board.EnumerateForbiddenMoves(&move_bit_set);

    ASSERT_EQ(1, move_bit_set.count());
    EXPECT_TRUE(move_bit_set[kMoveLH]);

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveLH){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));

        MoveBitSet influence_area;
        EXPECT_TRUE(bit_board.IsDoubleFourMove<kBlackTurn>(move, &influence_area));

        ASSERT_EQ(5, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveGH]);
        ASSERT_TRUE(influence_area[kMoveIH]);
        ASSERT_TRUE(influence_area[kMoveLH]);
        ASSERT_TRUE(influence_area[kMoveMH]);
        ASSERT_TRUE(influence_area[kMoveOH]);
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
        EXPECT_FALSE(bit_board.IsDoubleFourMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
  {
    // 長蛇の四々_1
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . x | B 
    // C | . . . . . . . . . . . x . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . x . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . o . . . . . . . | G 
    // H | . . . . o . x . . . . . . | H 
    // I | . . . . . x . o . . . . . | I 
    // J | . . . . . . o . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . o . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhgggiiikehjmcfhnbdl"));

    MoveBitSet move_bit_set;
    bit_board.EnumerateForbiddenMoves(&move_bit_set);

    ASSERT_EQ(1, move_bit_set.count());
    EXPECT_TRUE(move_bit_set[kMoveJF]);

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveJF){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));

        MoveBitSet influence_area;
        EXPECT_TRUE(bit_board.IsDoubleFourMove<kBlackTurn>(move, &influence_area));

        ASSERT_EQ(5, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveOA]);
        ASSERT_TRUE(influence_area[kMoveLD]);
        ASSERT_TRUE(influence_area[kMoveJF]);
        ASSERT_TRUE(influence_area[kMoveIG]);
        ASSERT_TRUE(influence_area[kMoveFJ]);
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
        EXPECT_FALSE(bit_board.IsDoubleFourMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
  {
    // 長蛇の四々_2
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------x A 
    // B | . . . . . . . . . . . . x | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . x . . . | E 
    // F | . . . . . . o . . . . . . | F 
    // G | . . . . . o . x . . . . . | G 
    // H | . . . . . . x . o . . . . | H 
    // I | . . . . . o . o . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhgiigiioaggnbjhkehf"));

    MoveBitSet move_bit_set;
    bit_board.EnumerateForbiddenMoves(&move_bit_set);

    ASSERT_EQ(1, move_bit_set.count());
    EXPECT_TRUE(move_bit_set[kMoveLD]);

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveLD){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));

        MoveBitSet influence_area;
        EXPECT_TRUE(bit_board.IsDoubleFourMove<kBlackTurn>(move, &influence_area));

        ASSERT_EQ(3, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveMC]);
        ASSERT_TRUE(influence_area[kMoveLD]);
        ASSERT_TRUE(influence_area[kMoveJF]);
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
        EXPECT_FALSE(bit_board.IsDoubleFourMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
  {
    // 双竜の四々
    //   A B C D E F G H I J K L M N O 
    // A o o ------------------------+ A 
    // B o x . . . . . . . . . . . . | B 
    // C | . x . . . . . . . . . . . | C 
    // D | . . x . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . . . . . . . . . | G 
    // H | . . . . . . x . . . . . . | H 
    // I | . . . . . . . x . o . . . | I 
    // J | . . . . . . . . x . . . . | J 
    // K | . . . . . . . o . o . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhkkjjkiiiikddaabbbaccab"));

    MoveBitSet move_bit_set;
    bit_board.EnumerateForbiddenMoves(&move_bit_set);

    ASSERT_EQ(1, move_bit_set.count());
    EXPECT_TRUE(move_bit_set[kMoveFF]);

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveFF){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));

        MoveBitSet influence_area;
        EXPECT_TRUE(bit_board.IsDoubleFourMove<kBlackTurn>(move, &influence_area));

        ASSERT_EQ(3, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveEE]);
        ASSERT_TRUE(influence_area[kMoveFF]);
        ASSERT_TRUE(influence_area[kMoveGG]);
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
        EXPECT_FALSE(bit_board.IsDoubleFourMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
}

TEST_F(BitBoardTest, IsTwoLineDoubleFourTest)
{
  const auto in_board_move_list = GetAllInBoardMove();

  {
    // 四々(2直線, 四のみ)
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . o o . . . | C 
    // D | . . * . . . . o x . * . . | D 
    // E | . . . . . . . x . . . . . | E 
    // F | . . . . . . x . . . . . . | F 
    // G | . . . . . . . . . . . . . | G 
    // H | . . . . . . x . . . . . . | H 
    // I | . . . . . . . x . o . . . | I 
    // J | . . . . . . . . x . o . . | J 
    // K | . . . . . . . . . o . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhkkjjkiiikchfjcjdidielj"));

    MoveBitSet move_bit_set;
    bit_board.EnumerateForbiddenMoves(&move_bit_set);

    ASSERT_EQ(1, move_bit_set.count());
    EXPECT_TRUE(move_bit_set[kMoveGG]);

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveGG){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));

        MoveBitSet influence_area;
        EXPECT_TRUE(bit_board.IsDoubleFourMove<kBlackTurn>(move, &influence_area));

        ASSERT_EQ(5, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveEE]);
        ASSERT_TRUE(influence_area[kMoveFF]);
        ASSERT_TRUE(influence_area[kMoveGG]);
        ASSERT_TRUE(influence_area[kMoveFH]);
        ASSERT_TRUE(influence_area[kMoveEI]);
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
        EXPECT_FALSE(bit_board.IsDoubleFourMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
  {
    // 四々(2直線, 達四のみ)
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . o . . . . . . . o . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | o . . . x . . . . . . . . | F 
    // G | . . x x x . . . . . . . . | G 
    // H | . . . . . . x . . . . . . | H 
    // I | . . . . . . . x . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . o . . . | K 
    // L | . . o . . . . . . . o . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhkkiiddeglddgdlffllfgbf"));
  
    MoveBitSet move_bit_set;
    bit_board.EnumerateForbiddenMoves(&move_bit_set);

    ASSERT_EQ(1, move_bit_set.count());
    EXPECT_TRUE(move_bit_set[kMoveGG]);

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveGG){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));

        MoveBitSet influence_area;
        EXPECT_TRUE(bit_board.IsDoubleFourMove<kBlackTurn>(move, &influence_area));

        ASSERT_EQ(7, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveEE]);
        ASSERT_TRUE(influence_area[kMoveGG]);
        ASSERT_TRUE(influence_area[kMoveJJ]);
        ASSERT_TRUE(influence_area[kMoveBG]);
        ASSERT_TRUE(influence_area[kMoveCG]);
        ASSERT_TRUE(influence_area[kMoveHG]);
        ASSERT_TRUE(influence_area[kMoveIG]);
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
        EXPECT_FALSE(bit_board.IsDoubleFourMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
  {
    // 四々(2直線, 達四、四混在)
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . o . . . . . . . o . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . x . . . . . . . . | F 
    // G | o . x x x . . . . . . . . | G 
    // H | . . . . . . x . . . . . . | H 
    // I | . . . . . . . x . . . . . | I 
    // J | . . . . . . . . o . . . . | J 
    // K | . . . . . . . . . o . . . | K 
    // L | . . o . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhkkiiddeglddgdlffjjfgbg"));

    MoveBitSet move_bit_set;
    bit_board.EnumerateForbiddenMoves(&move_bit_set);

    ASSERT_EQ(1, move_bit_set.count());
    EXPECT_TRUE(move_bit_set[kMoveGG]);

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveGG){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));

        MoveBitSet influence_area;
        EXPECT_TRUE(bit_board.IsDoubleFourMove<kBlackTurn>(move, &influence_area));

        ASSERT_EQ(5, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveEE]);
        ASSERT_TRUE(influence_area[kMoveGG]);
        ASSERT_TRUE(influence_area[kMoveCG]);
        ASSERT_TRUE(influence_area[kMoveHG]);
        ASSERT_TRUE(influence_area[kMoveIG]);
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
        EXPECT_FALSE(bit_board.IsDoubleFourMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
  {
    // 否禁(達四)
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . o . . . . . . . o . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . . . . . . . . . | G 
    // H | . . . . . x x x . . . . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . o . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhldihddghdl"));

    MoveBitSet move_bit_set;
    bit_board.EnumerateForbiddenMoves(&move_bit_set);

    ASSERT_EQ(0, move_bit_set.count());

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      EXPECT_FALSE(bit_board.IsDoubleFourMove<kBlackTurn>(move));
      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
}

TEST_F(BitBoardTest, IsDoubleThreeTest)
{
  const auto in_board_move_list = GetAllInBoardMove();

  {
    // 三々
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . o o o . . . . . . | G 
    // H | . . . . . o x . x . . . . | H 
    // I | . . . . . x . . . x . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhhggiggjhghkifg"));

    MoveBitSet move_bit_set;
    bit_board.EnumerateForbiddenMoves(&move_bit_set);

    ASSERT_EQ(1, move_bit_set.count());
    EXPECT_TRUE(move_bit_set[kMoveIG]);

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveIG){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
  {
    // 三々(禁点による否禁点の存在)
    // JHが禁点なのでIIは否禁
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . o . . . o . . . o . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . x . . | F 
    // G | . . . . . . . . . x . . . | G 
    // H | . . . . . x x . . . . . . | H 
    // I | . . . . . . . . x . . . . | I 
    // J | . . . . . . . . x . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . o . . . o . . . o . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O   
    BitBoard bit_board(MoveList("hhhdlfldjjddkgllghdljihl"));

    MoveBitSet move_bit_set;
    bit_board.EnumerateForbiddenMoves(&move_bit_set);

    ASSERT_EQ(1, move_bit_set.count());
    EXPECT_TRUE(move_bit_set[kMoveJH]);

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveJH){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
  {
    // 三々(判定路が必要になる複雑なケース)
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------o A 
    // B | . . . . . . . . . . . . . | B 
    // C | . o . . . . o . . . o . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G o . . . . . . . . . . . . . | G 
    // H o . x . . . . x . . . . . . | H 
    // I | . x x . . . . . . . . . . | I 
    // J | x . x . . . . . . . . . . | J 
    // K | . . . . . x . . . . . . . | K 
    // L | . . * x . . . . . . * . . | L 
    // M | . . . x x . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + ------o --o --------o ----o O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhoobjahciagemeodigochlofmccellcgkhcdjoa"));

    MoveBitSet move_bit_set;
    bit_board.EnumerateForbiddenMoves(&move_bit_set);

    ASSERT_EQ(8, move_bit_set.count());
    EXPECT_TRUE(move_bit_set[kMoveDH]);
    EXPECT_TRUE(move_bit_set[kMoveCJ]);
    EXPECT_TRUE(move_bit_set[kMoveEJ]);
    EXPECT_TRUE(move_bit_set[kMoveCK]);
    EXPECT_TRUE(move_bit_set[kMoveDK]);
    EXPECT_TRUE(move_bit_set[kMoveEK]);
    EXPECT_TRUE(move_bit_set[kMoveDL]);
    EXPECT_TRUE(move_bit_set[kMoveFL]);

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      bool is_forbidden = move == kMoveDH;
      is_forbidden |= move == kMoveCJ;
      is_forbidden |= move == kMoveEJ;
      is_forbidden |= move == kMoveCK;
      is_forbidden |= move == kMoveDK;
      is_forbidden |= move == kMoveEK;
      is_forbidden |= move == kMoveDL;
      is_forbidden |= move == kMoveFL;

      if(is_forbidden){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
  {
    // 否禁(三々)
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------o A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . x . . . . . . . . . | C 
    // D | o . x . x . o . x x * . o | D 
    // E | . . . x . . . . . . x . . | E 
    // F | . . . . . . . . . . x . . | F 
    // G | . . . . . . . . . . . . . | G 
    // H | . x . . . x x . . . . . . | H 
    // I | . . . x . . . . . . . . . | I 
    // J | . . . x . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M o . . . . . . . . . . . . . | M 
    // N | o . . . . . . . . . . . . | N 
    // O o --o --o --o --o --o --o --+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhbdddamecbnfdhdeecojdoakdndleaolfeoghgoeiioejkochmo"));

    MoveBitSet move_bit_set;
    bit_board.EnumerateForbiddenMoves(&move_bit_set);

    ASSERT_EQ(0, move_bit_set.count());

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
  {
    // 盤内以外
    BitBoard bit_board;

    for(auto move : GetAllMove()){
      if(IsInBoardMove(move)){
        continue;
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
}
}