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

TEST_F(BitBoardTest, IsOpenFourMoveTest)
{
  const auto in_board_move_list = GetAllInBoardMove();

  {
    // 達四(黒番)
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
    BitBoard bit_board(MoveList("hhhgihigjhjg"));
    
    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      const bool is_four = bit_board.IsOpenFourMove<kBlackTurn>(move);
      bool expect = (move == kMoveGH) || (move == kMoveKH);

      EXPECT_EQ(expect, is_four);
    }
  }
  {
    // 達四(黒番, 長連筋)
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . o o o . . . . . | G 
    // H | . . o . . x x x . . x . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhhgihiglhdhghgg"));
    
    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      const bool is_four = bit_board.IsOpenFourMove<kBlackTurn>(move);
      bool expect = (move == kMoveFH);

      EXPECT_EQ(expect, is_four);
    }
  }
  {
    // 否達四(黒番)
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . o o o . . . . . | G 
    // H | . . . o . x x x . . x . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhhgihiglhehghgg"));
    
    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      const bool is_four = bit_board.IsOpenFourMove<kBlackTurn>(move);
      EXPECT_FALSE(is_four);
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
    BitBoard bit_board(MoveList("hhhgihigfgjgoa"));
    
    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      const bool is_four = bit_board.IsOpenFourMove<kWhiteTurn>(move);
      bool expect = (move == kMoveKG);

      EXPECT_EQ(expect, is_four);
    }
  }
}

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

TEST_F(BitBoardTest, IsDoubleFourMoveTest)
{
  const auto in_board_move_list = GetAllInBoardMove();

  // 黒番はIsOneLineDoubleFourTest, IsTwoLineDoubleFourTestでテストする
  {
    // 両頭の四々_1
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . . o . . o o . o x G 
    // H | . . . . . . x . . . . . . | H 
    // I | . . . . . . x . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O x x ------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhhghikgaolgbongog"));

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      MoveBitSet influence_area;
      const bool is_double_four = bit_board.IsDoubleFourMove<kWhiteTurn>(move, &influence_area);
      const bool expect = move == kMoveJG;

      EXPECT_EQ(expect, is_double_four);

      if(expect){
        ASSERT_EQ(3, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveIG]);
        ASSERT_TRUE(influence_area[kMoveJG]);
        ASSERT_TRUE(influence_area[kMoveMG]);
      }
    }
  }
  {
    // 両頭の四々_2
    //   A B C D E F G H I J K L M N O 
    // A + ------------o ------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . o . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . o . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . . o . . . . . . | G 
    // H | . . . . . . x . . . . . . | H 
    // I | . . . . . . x . . . . . . | I 
    // J | . . . . . . x . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O x --------------------------x O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhhghihehjhcoohaao"));

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      MoveBitSet influence_area;
      const bool is_double_four = bit_board.IsDoubleFourMove<kWhiteTurn>(move, &influence_area);
      const bool expect = move == kMoveHD;

      EXPECT_EQ(expect, is_double_four);

      if(expect){
        ASSERT_EQ(3, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveHB]);
        ASSERT_TRUE(influence_area[kMoveHD]);
        ASSERT_TRUE(influence_area[kMoveHF]);
      }
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
    // G | . . . . . . . . . . . . . | G 
    // H | . . . . . . x o . o o . . o H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N x x . . . . . . . . . . . . | N 
    // O x x ------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhihaokhboohanlhbn"));

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      MoveBitSet influence_area;
      const bool is_double_four = bit_board.IsDoubleFourMove<kWhiteTurn>(move, &influence_area);
      const bool expect = move == kMoveMH;

      EXPECT_EQ(expect, is_double_four);

      if(expect){
        ASSERT_EQ(3, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveJH]);
        ASSERT_TRUE(influence_area[kMoveMH]);
        ASSERT_TRUE(influence_area[kMoveNH]);
      }
    }
  }
  {
    // 長蛇の四々_1
    //   A B C D E F G H I J K L M N O 
    // A + ------------------------o + A 
    // B | . . . . . . . . . . . o . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . o * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . . o . . . . . . | G 
    // H | . . . . . o x . . . . . . | H 
    // I | . . . . x . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . x x N 
    // O + ------------------------x x O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhhgfighookdnombonnann"));

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      MoveBitSet influence_area;
      const bool is_double_four = bit_board.IsDoubleFourMove<kWhiteTurn>(move, &influence_area);
      const bool expect = move == kMoveJE;

      EXPECT_EQ(expect, is_double_four);

      if(expect){
        ASSERT_EQ(3, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveLC]);
        ASSERT_TRUE(influence_area[kMoveJE]);
        ASSERT_TRUE(influence_area[kMoveIF]);
      }
    }
  }
  {
    // 長蛇の四々_2
    //   A B C D E F G H I J K L M N O 
    // A + ------------------------o + A 
    // B | . . . . . . . . . . . o . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . o . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . . . o . . . . . . | G 
    // H | . . . . . o x . . . . . . | H 
    // I | . . . . x . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . x x N 
    // O + ------------------------x x O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhhgfighoojenombonnann"));

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      MoveBitSet influence_area;
      const bool is_double_four = bit_board.IsDoubleFourMove<kWhiteTurn>(move, &influence_area);
      const bool expect = move == kMoveKD;

      EXPECT_EQ(expect, is_double_four);

      if(expect){
        ASSERT_EQ(3, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveLC]);
        ASSERT_TRUE(influence_area[kMoveKD]);
        ASSERT_TRUE(influence_area[kMoveIF]);
      }
    }
  }
  {
    // 双竜の四々
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . x o o o . . . o o o x G 
    // H | . . . . . . x . . . . . . | H 
    // I | . . . . . . . . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N x x . . . . . . . . . . . . | N 
    // O x x ------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhhgaogganlgbomgegfgbnngog"));

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      MoveBitSet influence_area;
      const bool is_double_four = bit_board.IsDoubleFourMove<kWhiteTurn>(move, &influence_area);
      const bool expect = move == kMoveJG;

      EXPECT_EQ(expect, is_double_four);

      if(expect){
        ASSERT_EQ(3, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveIG]);
        ASSERT_TRUE(influence_area[kMoveJG]);
        ASSERT_TRUE(influence_area[kMoveKG]);
      }
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
    BitBoard bit_board(MoveList("hhhgghggegfgaaidiaicibiefh"));
    
    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      MoveBitSet influence_area;
      const bool is_double_four = bit_board.IsDoubleFourMove<kWhiteTurn>(move, &influence_area);
      const bool expect = move == kMoveIG;

      EXPECT_EQ(expect, is_double_four);

      if(expect){
        ASSERT_EQ(3, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveIF]);
        ASSERT_TRUE(influence_area[kMoveIG]);
        ASSERT_TRUE(influence_area[kMoveJG]);
      }
    }
  }
  {
    // 四々(白番)
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . x . x . . | F 
    // G | . . . . o . o o . o . . . | G 
    // H | . . . . . o x x o . o . . | H 
    // I | . . . . . x o o . x . . . | I 
    // J | . . . . . . o x x x . . . | J 
    // K | . . . . . x . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O     
    BitBoard bit_board(MoveList("hhhijjiigiigihjhkikglflhjffgkjghijhjgkhg"));
    
    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      const bool expect = (move == kMoveGG || move == kMoveJG);

      MoveBitSet influence_area;
      const bool is_double_four = bit_board.IsDoubleFourMove<kWhiteTurn>(move, &influence_area);

      EXPECT_EQ(expect, is_double_four);

      if(move == kMoveGG){
        ASSERT_EQ(3, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveEG]);
        ASSERT_TRUE(influence_area[kMoveGG]);
        ASSERT_TRUE(influence_area[kMoveJG]);
      }else if(move == kMoveJG){
        ASSERT_EQ(3, influence_area.count());
        ASSERT_TRUE(influence_area[kMoveGG]);
        ASSERT_TRUE(influence_area[kMoveJG]);
        ASSERT_TRUE(influence_area[kMoveLG]);
      }
    }
  }
  {
    // 否 四四
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . x . . . . . . . | B 
    // C | . . . . x . o . . . . . . | C 
    // D | . o x . . x o . . . * . . | D 
    // E | . o x o . . o . o . x . . | E 
    // F | . . x x o o x x x o . o . | F 
    // G | . . o x x . x o o . x . . | G 
    // H | . o x x . o x . x x o . . | H 
    // I | . o . o o x o o . o . . x | I 
    // J | . . . x x x o o x o x . . | J 
    // K | . x . o o o x x x . o . . | K 
    // L | . . * . x . o x o o * x . | L 
    // M | . . . . . . x o o . . . . | M 
    // N | . . . . . o x x . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhhihfijifiigighjkigikhjhkgkjhjggjkjhggfjjjlfgkikhlhlglkmlklljjeegdgiljminimhmfihngnflhlfjekejfkdhffdfceehchefeigdhdjfkflemfnieedecdddcickhcfchegb"));
    
    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      const bool is_double_four = bit_board.IsDoubleFourMove<kWhiteTurn>(move);
      const bool expect = false;

      EXPECT_EQ(expect, is_double_four);
    }
  }
}

TEST_F(BitBoardTest, IsDoubleSemiThreeMoveBlackTest)
{
  const auto in_board_move_list = GetAllInBoardMove();
  
  {
    // 三々(黒番)
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

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveIG){
        EXPECT_TRUE(bit_board.IsDoubleSemiThreeMove<kBlackTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kWhiteTurn>(move));
    }
  }
  {
    // 三々
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

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveJH || move == kMoveII){
        EXPECT_TRUE(bit_board.IsDoubleSemiThreeMove<kBlackTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kWhiteTurn>(move));
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

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      bool is_semi_double_three = move == kMoveDH;
      is_semi_double_three |= move == kMoveCJ;
      is_semi_double_three |= move == kMoveEJ;
      is_semi_double_three |= move == kMoveCK;
      is_semi_double_three |= move == kMoveDK;
      is_semi_double_three |= move == kMoveEK;
      is_semi_double_three |= move == kMoveDL;
      is_semi_double_three |= move == kMoveFL;

      if(is_semi_double_three){
        EXPECT_TRUE(bit_board.IsDoubleSemiThreeMove<kBlackTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kWhiteTurn>(move));
    }
  }
  {
    // 出展: 連珠世界2000年11月号
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------o A 
    // B | . . . . . . . . . . . . . o B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . x x . . . . * . . | D 
    // E | . . . . . . x . . . . . . | E 
    // F | . . . . . x x . . . . . . | F 
    // G | . . . . . . x . . . . . . | G 
    // H | . . . . . . x . . . . . . | H 
    // I | . . . . . . o . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N o . . . . . . . . . . . . . o N 
    // O o --------------------------o O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhhihgoohfongfaogdanfdoaheob"));
    EXPECT_TRUE(bit_board.IsDoubleSemiThreeMove<kBlackTurn>(kMoveID));
  }
  {
    // 出展: 連珠世界2001年09月号
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . x o . x . o * . . | D 
    // E | . . . . . x o . . . . . . | E 
    // F | . . . . . x . x . . . . . | F 
    // G | . . . . . . . . x x . . . | G 
    // H | . . . . o . x . . o . . . | H 
    // I | . . . . x . x o . . . . . | I 
    // J | . . . . . o x . . . . . . | J 
    // K | . . . . . . o . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhiihigjhjhkfifhjgkhifhegegdgfkdidppkgppfdpp"));
    EXPECT_TRUE(bit_board.IsDoubleSemiThreeMove<kBlackTurn>(kMoveHG));
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

      EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kBlackTurn>(move));
      EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kWhiteTurn>(move));
    }
  }
  {
    // 盤内以外
    BitBoard bit_board;

    for(auto move : GetAllMove()){
      if(IsInBoardMove(move)){
        continue;
      }

      EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kBlackTurn>(move));
      EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kWhiteTurn>(move));
    }
  }
}

TEST_F(BitBoardTest, IsDoubleSemiThreeMoveWhiteTest)
{
  const auto in_board_move_list = GetAllInBoardMove();
  
  {
    // 三々(白番)
    //   A B C D E F G H I J K L M N O 
    // A x --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G | . . . . x x x . . . . . . | G 
    // H | . . . . . x o . o . . . . | H 
    // I | . . . . . o . . . o . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("aahhhggiggjhghkifg"));

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveIG){
        EXPECT_TRUE(bit_board.IsDoubleSemiThreeMove<kWhiteTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kWhiteTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kBlackTurn>(move));
    }
  }
  {
    // 三々
    //   A B C D E F G H I J K L M N O 
    // A x --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . x . . . x . . . x . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . o . . | F 
    // G | . . . . . . . . . o . . . | G 
    // H | . . . . . o o . . . . . . | H 
    // I | . . . . . . . . o . . . . | I 
    // J | . . . . . . . . o . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . x . . . x . . . x . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("aahhhdlfldjjddkgllghdljihl"));

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveJH || move == kMoveII){
        EXPECT_TRUE(bit_board.IsDoubleSemiThreeMove<kWhiteTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kWhiteTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kBlackTurn>(move));
    }
  }
  {
    // 三々(判定路が必要になる複雑なケース)
    //   A B C D E F G H I J K L M N O 
    // A x --------------------------x A 
    // B | . . . . . . . . . . . . . | B 
    // C | . x . . . . x . . . x . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . . . . . . . . . . | E 
    // F | . . . . . . . . . . . . . | F 
    // G x . . . . . . . . . . . . . | G 
    // H x . o . . . . o . . . . . . | H 
    // I | . o o . . . . . . . . . . | I 
    // J | o . o . . . . . . . . . . | J 
    // K | . . . . . o . . . . . . . | K 
    // L | . . * o . . . . . . * . . | L 
    // M | . . . o o . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + ------x --x --------x ----x O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("aahhoobjahciagemeodigochlofmccellcgkhcdjoa"));
    
    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      bool is_semi_double_three = move == kMoveDH;
      is_semi_double_three |= move == kMoveCJ;
      is_semi_double_three |= move == kMoveEJ;
      is_semi_double_three |= move == kMoveCK;
      is_semi_double_three |= move == kMoveDK;
      is_semi_double_three |= move == kMoveEK;
      is_semi_double_three |= move == kMoveDL;
      is_semi_double_three |= move == kMoveFL;

      if(is_semi_double_three){
        EXPECT_TRUE(bit_board.IsDoubleSemiThreeMove<kWhiteTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kWhiteTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsDoubleSemiThreeMove<kBlackTurn>(move));
    }
  }
  {
    // 出展: 連珠世界2000年11月号
    //   A B C D E F G H I J K L M N O 
    // A x --------------------------x A 
    // B | . . . . . . . . . . . . . x B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . o o . . . . * . . | D 
    // E | . . . . . . o . . . . . . | E 
    // F | . . . . . o o . . . . . . | F 
    // G | . . . . . . o . . . . . . | G 
    // H | . . . . . . o . . . . . . | H 
    // I | . . . . . . x . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N x . . . . . . . . . . . . . x N 
    // O x --------------------------x O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("aahhhihgoohfongfaogdanfdoaheob"));
    EXPECT_TRUE(bit_board.IsDoubleSemiThreeMove<kWhiteTurn>(kMoveID));
  }
  {
    // 出展: 連珠世界2001年09月号
    //   A B C D E F G H I J K L M N O 
    // A x --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . o x . o . x * . . | D 
    // E | . . . . . o x . . . . . . | E 
    // F | . . . . . o . o . . . . . | F 
    // G | . . . . . . . . o o . . . | G 
    // H | . . . . x . o . . x . . . | H 
    // I | . . . . o . o x . . . . . | I 
    // J | . . . . . x o . . . . . . | J 
    // K | . . . . . . x . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("aahhiihigjhjhkfifhjgkhifhegegdgfkdidppkgppfdpp"));
    EXPECT_TRUE(bit_board.IsDoubleSemiThreeMove<kWhiteTurn>(kMoveHG));
  }
}

TEST_F(BitBoardTest, EnumerateMiseMovesTest)
{
  //   A B C D E F G H I J K L M N O 
  // A x --------------------------+ A 
  // B | x . . . . . . . . . . . . | B 
  // C | . x . . . . . . . . . . . | C 
  // D | . . * . . . . . . . * . . | D 
  // E | . . . . . . . . . . . . . | E 
  // F | . . . . . . . . . . . . . | F 
  // G | . . . x o o o . . . . . . | G 
  // H | . . . . . . x o . . . . . | H 
  // I | . . . . . . . . . . . . . | I 
  // J | . . . . . . x . . . . . . | J 
  // K | . . . . . . . . . . . . . | K 
  // L | . . * . . . . . x x * . . | L 
  // M | . . o . . . . . . . . . . | M 
  // N | o . . o . . . . . . . . . | N 
  // O + o ------------------------+ O 
  //   A B C D E F G H I J K L M N O 
  MoveList board_move_list("hhhghjfgjlihklggegboccbnbbdmaaen");
  BitBoard bit_board(board_move_list);

  BoardOpenState board_open_state;
  bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);
  
  {
    // 黒番
    MoveBitSet mise_bit, multi_mise_bit;
    bit_board.EnumerateMiseMoves<kBlackTurn>(board_open_state, &mise_bit, &multi_mise_bit);

    ASSERT_EQ(mise_bit.count(), 5);
    EXPECT_TRUE(mise_bit[kMoveHI]);
    EXPECT_TRUE(mise_bit[kMoveHK]);
    EXPECT_TRUE(mise_bit[kMoveED]);
    EXPECT_TRUE(mise_bit[kMoveEF]);
    EXPECT_TRUE(mise_bit[kMoveEH]);

    ASSERT_TRUE(multi_mise_bit.none());
  }
  {
    // 白番
    MoveBitSet mise_bit, multi_mise_bit;
    bit_board.EnumerateMiseMoves<kWhiteTurn>(board_open_state, &mise_bit, &multi_mise_bit);
    
    ASSERT_EQ(mise_bit.count(), 12);
    EXPECT_TRUE(mise_bit[kMoveIE]);
    EXPECT_TRUE(mise_bit[kMoveIF]);
    EXPECT_TRUE(mise_bit[kMoveII]);
    EXPECT_TRUE(mise_bit[kMoveIJ]);
    EXPECT_TRUE(mise_bit[kMoveLE]);
    EXPECT_TRUE(mise_bit[kMoveKF]);
    EXPECT_TRUE(mise_bit[kMoveHI]);
    EXPECT_TRUE(mise_bit[kMoveGJ]);
    EXPECT_TRUE(mise_bit[kMoveFK]);
    EXPECT_TRUE(mise_bit[kMoveEL]);
    EXPECT_TRUE(mise_bit[kMoveBL]);
    EXPECT_TRUE(mise_bit[kMoveBM]);

    ASSERT_TRUE(multi_mise_bit.none());
  }
}

TEST_F(BitBoardTest, EnumerateMultiMiseMovesTest)
{
  //   A B C D E F G H I J K L M N O 
  // A + --------------------------+ A 
  // B | . . . . . . . . . . . . . | B 
  // C | . . . . . . . . . . . . . | C 
  // D | . . * . . . . . . . * . . | D 
  // E | . . . . o . . . . . . . . | E 
  // F | . . . . . x x x o . . . . | F 
  // G | . . . . . o x o . . . . . | G 
  // H | . . . . . . x x o . . . . | H 
  // I | . . . . . . o . . . . . . | I 
  // J | . . . . . . . . . . . . . | J 
  // K | . . . . . . . . . . . . . | K 
  // L | . . * . . . . . . . * . . | L 
  // M | . . . . . . . . . . . . . | M 
  // N | . . . . . . . . . . . . . | N 
  // O + --------------------------+ O 
  //   A B C D E F G H I J K L M N O 
  MoveList board_move_list("hhigihjhhfgghghigffeifjf");
  BitBoard bit_board(board_move_list);

  BoardOpenState board_open_state;
  bit_board.GetBoardOpenState(kUpdateAllOpenState, &board_open_state);
  
  {
    // 黒番
    MoveBitSet mise_bit, multi_mise_bit;
    bit_board.EnumerateMiseMoves<kBlackTurn>(board_open_state, &mise_bit, &multi_mise_bit);

    ASSERT_EQ(multi_mise_bit.count(), 1);
    EXPECT_TRUE(multi_mise_bit[kMoveEH]);
  }
}

}   // namespace realcore
