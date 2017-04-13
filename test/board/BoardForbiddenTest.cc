// @brief メンバ関数(禁手チェック)のテスト
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
};

TEST_F(BoardTest, IsOverlineTest)
{
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
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    ASSERT_EQ(1, forbidden_move_set.count());
    EXPECT_TRUE(forbidden_move_set[kMoveKH]);
  }
}

TEST_F(BoardTest, IsOneLineDoubleFourTest)
{
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
    Board board(MoveList("hhignhghkhkilhoh"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    ASSERT_EQ(1, forbidden_move_set.count());
    EXPECT_TRUE(forbidden_move_set[kMoveJH]);
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
    Board board(MoveList("hhhihbhahdgehfie"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    ASSERT_EQ(1, forbidden_move_set.count());
    EXPECT_TRUE(forbidden_move_set[kMoveHE]);
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
    Board board(MoveList("hhghnhohkhjgjhli"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    ASSERT_EQ(1, forbidden_move_set.count());
    EXPECT_TRUE(forbidden_move_set[kMoveLH]);
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
    Board board(MoveList("hhgiigiioaggnbjhldhf"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    ASSERT_EQ(1, forbidden_move_set.count());
    EXPECT_TRUE(forbidden_move_set[kMoveKE]);
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
    Board board(MoveList("hhgiigiioaggnbjhkehf"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    ASSERT_EQ(1, forbidden_move_set.count());
    EXPECT_TRUE(forbidden_move_set[kMoveLD]);
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
    Board board(MoveList("hhkkjjkiiiikddaabbbaccab"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    ASSERT_EQ(1, forbidden_move_set.count());
    EXPECT_TRUE(forbidden_move_set[kMoveFF]);
  }
}

TEST_F(BoardTest, IsTwoLineDoubleFourTest)
{
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
    Board board(MoveList("hhkkjjkiiikchfjcjdidielj"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    ASSERT_EQ(1, forbidden_move_set.count());
    EXPECT_TRUE(forbidden_move_set[kMoveGG]);
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
    Board board(MoveList("hhkkiiddeglddgdlffllfgbg"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    ASSERT_EQ(1, forbidden_move_set.count());
    EXPECT_TRUE(forbidden_move_set[kMoveGG]);
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
    Board board(MoveList("hhkkiiddeglddgdlffjjfgbg"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    ASSERT_EQ(1, forbidden_move_set.count());
    EXPECT_TRUE(forbidden_move_set[kMoveGG]);
  }
  {
    // 四々
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . . * . . | D 
    // E | . . . . o . . x . . . . . | E 
    // F | . . . x . x o o o x o . . | F 
    // G | . . . . o o x x x o . . . | G 
    // H | . . . . . o x . x . . . . | H 
    // I | . . . . . x o o o x . . . | I 
    // J | . . . . o . o x x o o o x o J 
    // K | . . . . . x . o . x . . x | K 
    // L | . . * . . . . . o x x x x o L 
    // M | . . . . . . . . . o x . . | M 
    // N | . . . . . . . . . . . . . | N 
    // O + --------------------------+ O 
    //   A B C D E F G H I J K L M N O 
    Board board(MoveList("hhhihghfjgghiggggijfiefgefifgffejjjikkikkiljijhjlliigklfkfkgjhfjklkjmljllmmjnlolnkojnjkm"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    ASSERT_EQ(1, forbidden_move_set.count());
    EXPECT_TRUE(forbidden_move_set[kMoveNN]);
  }
  {
    // 四々
    //   A B C D E F G H I J K L M N O 
    // A + --------------------------+ A 
    // B | . . . . . . . . . . . . . | B 
    // C | . . . . . . . . . . . . . | C 
    // D | . . * . . . . . . o * . . | D 
    // E | . . . . . . . . x . . . . | E 
    // F | . . . . . . x x o x . . . | F 
    // G | . . . . . . . o o x o . . | G 
    // H | . . . . . . x o o x o . . | H 
    // I | . . . . . . o o x o x . . | I 
    // J | . . . . o o . o x . o . . | J 
    // K | . . . . x x o x x x o x . | K 
    // L | . . * . . . x x x o * . . | L 
    // M | . . . . . . o x o . . . . | M 
    // N | . . . . . . x x x . . . . | N 
    // O + ----------o --o --o ------+ O 
    //   A B C D E F G H I J K L M N O 
    Board board(MoveList("hhhijjiiikkijijhkgighfihifljmklhkhlglijgkfjfjekdjkgjfkjmilhmimijgkhkhlfjjlkliniokklkhngojnko"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    ASSERT_EQ(1, forbidden_move_set.count());
    EXPECT_TRUE(forbidden_move_set[kMoveFN]);
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
    Board board(MoveList("hhldihddghdl"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    EXPECT_TRUE(forbidden_move_set.none());
  }
}

TEST_F(BoardTest, IsDoubleThreeTest)
{
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
    Board board(MoveList("hhhggiggjhghkifg"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    ASSERT_EQ(1, forbidden_move_set.count());
    EXPECT_TRUE(forbidden_move_set[kMoveIG]);
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
    Board board(MoveList("hhhdlfldjjddkgllghdljihl"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    ASSERT_EQ(1, forbidden_move_set.count());
    EXPECT_TRUE(forbidden_move_set[kMoveJH]);
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
    Board board(MoveList("hhoobjahciagemeodigochlofmccellcgkhcdjoa"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    ASSERT_EQ(8, forbidden_move_set.count());
    EXPECT_TRUE(forbidden_move_set[kMoveDH]);
    EXPECT_TRUE(forbidden_move_set[kMoveCJ]);
    EXPECT_TRUE(forbidden_move_set[kMoveEJ]);
    EXPECT_TRUE(forbidden_move_set[kMoveCK]);
    EXPECT_TRUE(forbidden_move_set[kMoveDK]);
    EXPECT_TRUE(forbidden_move_set[kMoveEK]);
    EXPECT_TRUE(forbidden_move_set[kMoveDL]);
    EXPECT_TRUE(forbidden_move_set[kMoveFL]);
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
    Board board(MoveList("hhbdddamecbnfdhdeecojdoakdndleaolfeoghgoeiioejkochmo"));
    MoveBitSet forbidden_move_set;

    board.EnumerateForbiddenMoves(&forbidden_move_set);

    EXPECT_TRUE(forbidden_move_set.none());
  }
}

TEST_F(BoardTest, EnumerateForbiddenTest1)
{
  //   A B C D E F G H I J K L M N O 
  // A + --------------x ----------+ A 
  // B | . x . . x x x o . . . . . | B 
  // C | . x o x o o . o x x . . . | C 
  // D | . o o o x x . o o o x . . | D 
  // E | . . . x o . o o x o x o . | E 
  // F | . . . . . o x x . o o o x | F 
  // G | . . . . x o x o x x x x o | G 
  // H | . . . . o . x . x o o o x o H 
  // I | . . . . . x o . o . x . . | I 
  // J | . . x . o x . . . . . . . | J 
  // K | . . . . . . . . . . . . . | K 
  // L | . . * . . . . . . . * . . | L 
  // M | . . . . . . . . . . . . . | M 
  // N | . . . . . . . . . . . . . | N 
  // O + --------------------------+ O 
  //   A B C D E F G H I J K L M N O 
  Board board(MoveList("hhhijggggiigfgfhhfhedjidifgfgdkfjekdjcjdldfeeeedhgfcfbkekcichblfmgmfnfmekgdccblhlikhlgnggbibccieiaohlemhnhddjhjifdgcgjfjeccd"));
  MoveBitSet forbidden_move_set;

  board.EnumerateForbiddenMoves(&forbidden_move_set);
//  EXPECT_TRUE(forbidden_move_set.none());
}


}