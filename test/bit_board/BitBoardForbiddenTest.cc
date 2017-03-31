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
    // G | . . . . . . . o . . . . . | G 
    // H | . . . . . o x . . x x . x o H 
    // I | . . . . . . . . . o . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhignhghkhkilhoh"));

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
    // 両頭の四々_2
    //   A B C D E F G H I J K L M N O 
    // A + ------------o ------------+ A 
    // B | . . . . . . x . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . x . . . * . . | D 
    // E | . . . . . o . o . . . . . | E 
    // F | . . . . . . x . . . . . . | F 
    // G | . . . . . . . . . . . . . | G 
    // H | . . . . . . x . . . . . . | H 
    // I | . . . . . . o . . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhhihbhahdgehfie"));

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveHE){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
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
    // G | . . . . . . . . o . . . . | G 
    // H | . . . . . o x . x x . . x o H 
    // I | . . . . . . . . . . o . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . . . . . | K 
    // L | . . * . . . . . . . * . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhghnhohkhjgjhli"));

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveLH){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }

      EXPECT_FALSE(bit_board.IsForbiddenMove<kWhiteTurn>(move));
    }
  }
  {
    // 長蛇の四々_1
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------x A 
    // B | . . . . . . . . . . . . x | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . x . . | D 
    // E | . . . . . . . . . . . . . | E 
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
    BitBoard bit_board(MoveList("hhgiigiioaggnbjhldhf"));

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveKE){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
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

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveLD){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
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

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveFF){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
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

    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveGG){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
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
    // F | . . . . x . . . . . . . . | F 
    // G | o . x x x . . . . . . . . | G 
    // H | . . . . . . x . . . . . . | H 
    // I | . . . . . . . x . . . . . | I 
    // J | . . . . . . . . . . . . . | J 
    // K | . . . . . . . . . o . . . | K 
    // L | . . o . . . . . . . o . . | L 
    // M | . . . . . . . . . . . . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    BitBoard bit_board(MoveList("hhkkiiddeglddgdlffllfgbg"));
  
    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveGG){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
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
  
    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      if(move == kMoveGG){
        EXPECT_TRUE(bit_board.IsForbiddenMove<kBlackTurn>(move));
      }else{
        EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
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
  
    for(const auto move : in_board_move_list){
      if(bit_board.GetState(move) != kOpenPosition){
        continue;
      }
      
      EXPECT_FALSE(bit_board.IsForbiddenMove<kBlackTurn>(move));
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