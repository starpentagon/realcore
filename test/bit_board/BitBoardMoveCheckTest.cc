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
}   // namespace realcore
