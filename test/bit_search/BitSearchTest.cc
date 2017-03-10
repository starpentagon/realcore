#include "gtest/gtest.h"

#include "BitSearch.h"

using namespace std;

namespace realcore
{

TEST(BitSearchTest, GetStateBitTest)
{
  {
    // 盤外,空点,白石,黒石
    const string state_str = "XOWB";
    const BitBoard bit_board = GetStateBit(state_str);

    constexpr BitBoard expect_bit = (kOverBoard << 6) | (kOpenPosition << 4) | (kWhiteStone << 2) | kBlackStone;
    EXPECT_EQ(expect_bit, bit_board);
  }
  {
    // 境界値パターン: (盤外,空点,白石,黒石)×8
    const string state_str = "XOWBXOWBXOWBXOWBXOWBXOWBXOWBXOWB";
    const BitBoard bit_board = GetStateBit(state_str);
    
    constexpr BitBoard base_pattern = (kOverBoard << 6) | (kOpenPosition << 4) | (kWhiteStone << 2) | kBlackStone;
    constexpr BitBoard bit_pattern = base_pattern | (base_pattern << 8) | (base_pattern << 16) | (base_pattern << 24);  
    constexpr BitBoard expect_bit = bit_pattern | (bit_pattern << 32);
    EXPECT_EQ(expect_bit, bit_board);
 }
}

TEST(BitSearchTest, GetBlackStoneBitTest)
{
  // (盤外,空点,白石,黒石)×8をテストパターンとする
  constexpr BitBoard base_pattern = (kOverBoard << 6) | (kOpenPosition << 4) | (kWhiteStone << 2) | kBlackStone;
  constexpr BitBoard bit_pattern = base_pattern | (base_pattern << 8) | (base_pattern << 16) | (base_pattern << 24);  
  constexpr BitBoard bit_board = bit_pattern | (bit_pattern << 32);

  constexpr BitBoard black_bit = GetBlackStoneBit(bit_board);

  constexpr BitBoard base_bit = 0b00000001;
  constexpr BitBoard expect_bit_pattern = base_bit | (base_bit << 8) | (base_bit << 16) | (base_bit << 24); 
  constexpr BitBoard expect_bit = expect_bit_pattern | (expect_bit_pattern << 32);

  EXPECT_EQ(expect_bit, black_bit);
}

TEST(BitSearchTest, GetWhiteStoneBitTest)
{
  // (盤外,空点,白石,黒石)×8をテストパターンとする
  constexpr BitBoard base_pattern = (kOverBoard << 6) | (kOpenPosition << 4) | (kWhiteStone << 2) | kBlackStone;
  constexpr BitBoard bit_pattern = base_pattern | (base_pattern << 8) | (base_pattern << 16) | (base_pattern << 24);  
  constexpr BitBoard bit_board = bit_pattern | (bit_pattern << 32);

  constexpr BitBoard white_bit = GetWhiteStoneBit(bit_board);

  constexpr BitBoard base_bit = 0b00000100;
  constexpr BitBoard expect_bit_pattern = base_bit | (base_bit << 8) | (base_bit << 16) | (base_bit << 24); 
  constexpr BitBoard expect_bit = expect_bit_pattern | (expect_bit_pattern << 32);

  EXPECT_EQ(expect_bit, white_bit);
}

TEST(BitSearchTest, GetOpenPositionBitTest)
{
  // (盤外,空点,白石,黒石)×8をテストパターンとする
  constexpr BitBoard base_pattern = (kOverBoard << 6) | (kOpenPosition << 4) | (kWhiteStone << 2) | kBlackStone;
  constexpr BitBoard bit_pattern = base_pattern | (base_pattern << 8) | (base_pattern << 16) | (base_pattern << 24);  
  constexpr BitBoard bit_board = bit_pattern | (bit_pattern << 32);

  constexpr BitBoard open_bit = GetOpenPositionBit(bit_board);

  constexpr BitBoard base_bit = 0b00010000;
  constexpr BitBoard expect_bit_pattern = base_bit | (base_bit << 8) | (base_bit << 16) | (base_bit << 24); 
  constexpr BitBoard expect_bit = expect_bit_pattern | (expect_bit_pattern << 32);

  EXPECT_EQ(expect_bit, open_bit);
}

TEST(BitSearchTest, GetConsectiveStoneBit)
{
  // 8個連続する黒石パターンに対して6個連続する位置を検索する
  constexpr BitBoard base_pattern = (kBlackStone << 6) | (kBlackStone << 4) | (kBlackStone << 2) | kBlackStone;
  constexpr BitBoard test_pattern = (base_pattern << 8) | base_pattern;

  constexpr size_t kSixStones = 6;
  const BitBoard consective_bit = GetConsectiveStoneBit<kSixStones>(test_pattern);

  constexpr BitBoard expect_bit = 0b010101;
  EXPECT_EQ(expect_bit, consective_bit);
}

TEST(BitSearchTest, GetStoneWithOneOpenBitTest)
{
  {
    // OBBBOからBBBO, OBBBを検索
    constexpr BitBoard test_pattern = (kOpenPosition << 8) | (kBlackStone << 6) | (kBlackStone << 4) | (kBlackStone << 2) | kOpenPosition;
    constexpr BitBoard black_bit = GetBlackStoneBit(test_pattern);
    constexpr BitBoard open_bit = GetOpenPositionBit(test_pattern);

    constexpr size_t kPatternLength = 4;
    array<BitBoard, kPatternLength> pattern_bit_list = {{0}};

    GetStoneWithOneOpenBit<kPatternLength>(black_bit, open_bit, &pattern_bit_list);

    constexpr BitBoard expect_bit_1 = 0b01;
    EXPECT_EQ(expect_bit_1, pattern_bit_list[0]);

    EXPECT_EQ(0, pattern_bit_list[1]);
    EXPECT_EQ(0, pattern_bit_list[2]);

    constexpr BitBoard expect_bit_2 = 0b0100;
    EXPECT_EQ(expect_bit_2, pattern_bit_list[3]);
  }
  {
    // OBBOBからBBOBを検索
    constexpr BitBoard test_pattern = (kOpenPosition << 8) | (kBlackStone << 6) | (kBlackStone << 4) | (kOpenPosition << 2) | kBlackStone;
    constexpr BitBoard black_bit = GetBlackStoneBit(test_pattern);
    constexpr BitBoard open_bit = GetOpenPositionBit(test_pattern);

    constexpr size_t kPatternLength = 4;
    array<BitBoard, kPatternLength> pattern_bit_list = {{0}};

    GetStoneWithOneOpenBit<kPatternLength>(black_bit, open_bit, &pattern_bit_list);

    constexpr BitBoard expect_bit_1 = 0b01;
    EXPECT_EQ(expect_bit_1, pattern_bit_list[0]);

    EXPECT_EQ(0, pattern_bit_list[1]);
    EXPECT_EQ(0, pattern_bit_list[2]);

    constexpr BitBoard expect_bit_2 = 0b0100;
    EXPECT_EQ(expect_bit_2, pattern_bit_list[3]);
  }
  {
    // OBOBBからBOBBを検索
  }
  {
    // OBBBOからを検索
  }
}

}   // namespace realcore
