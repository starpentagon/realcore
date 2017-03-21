#include <random>

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
    const StateBit bit_board = GetStateBit(state_str);

    constexpr StateBit expect_bit = (kOverBoard << 6) | (kOpenPosition << 4) | (kWhiteStone << 2) | kBlackStone;
    EXPECT_EQ(expect_bit, bit_board);
  }
  {
    // 境界値パターン: (盤外,空点,白石,黒石)×8
    const string state_str = "XOWBXOWBXOWBXOWBXOWBXOWBXOWBXOWB";
    const StateBit bit_board = GetStateBit(state_str);
    
    constexpr StateBit base_pattern = (kOverBoard << 6) | (kOpenPosition << 4) | (kWhiteStone << 2) | kBlackStone;
    constexpr StateBit bit_pattern = base_pattern | (base_pattern << 8) | (base_pattern << 16) | (base_pattern << 24);  
    constexpr StateBit expect_bit = bit_pattern | (bit_pattern << 32);
    EXPECT_EQ(expect_bit, bit_board);
 }
}

TEST(BitSearchTest, GetStateBitStringTest)
{
  constexpr StateBit state_bit = (kOverBoard << 6) | (kOpenPosition << 4) | (kWhiteStone << 2) | kBlackStone;
  const string bit_str = GetStateBitString(state_bit);

  EXPECT_EQ("XXXXXXXX XXXXXXXX XXXXXXXX XXXXXOWB", bit_str);
}

TEST(BitSearchTest, GetBlackStoneBitTest)
{
  // (盤外,空点,白石,黒石)×8をテストパターンとする
  const string state_str = "XOWB XOWB XOWB XOWB XOWB XOWB XOWB XOWB";
  const StateBit bit_board = GetStateBit(state_str);

  const StateBit black_bit = GetBlackStoneBit(bit_board);

  constexpr StateBit base_bit = 0b00000001;
  constexpr StateBit expect_bit_pattern = base_bit | (base_bit << 8) | (base_bit << 16) | (base_bit << 24); 
  constexpr StateBit expect_bit = expect_bit_pattern | (expect_bit_pattern << 32);

  EXPECT_EQ(expect_bit, black_bit);
}

TEST(BitSearchTest, GetWhiteStoneBitTest)
{
  // (盤外,空点,白石,黒石)×8をテストパターンとする
  const string state_str = "XOWB XOWB XOWB XOWB XOWB XOWB XOWB XOWB";
  const StateBit bit_board = GetStateBit(state_str);

  const StateBit white_bit = GetWhiteStoneBit(bit_board);

  constexpr StateBit base_bit = 0b00000100;
  constexpr StateBit expect_bit_pattern = base_bit | (base_bit << 8) | (base_bit << 16) | (base_bit << 24); 
  constexpr StateBit expect_bit = expect_bit_pattern | (expect_bit_pattern << 32);

  EXPECT_EQ(expect_bit, white_bit);
}

TEST(BitSearchTest, GetOpenPositionBitTest)
{
  // (盤外,空点,白石,黒石)×8をテストパターンとする
  const string state_str = "XOWB XOWB XOWB XOWB XOWB XOWB XOWB XOWB";
  const StateBit bit_board = GetStateBit(state_str);

  const StateBit open_bit = GetOpenPositionBit(bit_board);

  constexpr StateBit base_bit = 0b00010000;
  constexpr StateBit expect_bit_pattern = base_bit | (base_bit << 8) | (base_bit << 16) | (base_bit << 24); 
  constexpr StateBit expect_bit = expect_bit_pattern | (expect_bit_pattern << 32);

  EXPECT_EQ(expect_bit, open_bit);
}

TEST(BitSearchTest, GetConsectiveStoneBit)
{
  // 8個連続する黒石パターンに対して6個連続する位置を検索する
  const StateBit test_pattern = GetStateBit("BBBBBBBB");

  constexpr size_t kSixStones = 6;
  const StateBit consective_bit = GetConsectiveStoneBit<kSixStones>(test_pattern);

  constexpr StateBit expect_bit = 0b010101;
  EXPECT_EQ(expect_bit, consective_bit);
}

TEST(BitSearchTest, GetStoneWithOneOpenBitTest)
{
  {
    // OBBBOからBBBO, OBBBを検索
    const StateBit test_pattern = GetStateBit("OBBBO");
    const StateBit black_bit = GetBlackStoneBit(test_pattern);
    const StateBit open_bit = GetOpenPositionBit(test_pattern);

    constexpr size_t kPatternLength = 4;
    array<StateBit, kPatternLength> pattern_bit_list = {{0}};

    GetStoneWithOneOpenBit<kPatternLength>(black_bit, open_bit, &pattern_bit_list);

    constexpr StateBit expect_bit_1 = 0b01;
    EXPECT_EQ(expect_bit_1, pattern_bit_list[0]);

    EXPECT_EQ(0, pattern_bit_list[1]);
    EXPECT_EQ(0, pattern_bit_list[2]);

    constexpr StateBit expect_bit_2 = 0b0100;
    EXPECT_EQ(expect_bit_2, pattern_bit_list[3]);
  }
  {
    // OBBOBからBBOBを検索
    const StateBit test_pattern = GetStateBit("OBBOB");
    const StateBit black_bit = GetBlackStoneBit(test_pattern);
    const StateBit open_bit = GetOpenPositionBit(test_pattern);

    constexpr size_t kPatternLength = 4;
    array<StateBit, kPatternLength> pattern_bit_list = {{0}};

    GetStoneWithOneOpenBit<kPatternLength>(black_bit, open_bit, &pattern_bit_list);

    EXPECT_EQ(0, pattern_bit_list[0]);

    constexpr StateBit expect_bit_1 = 0b01;
    EXPECT_EQ(expect_bit_1, pattern_bit_list[1]);

    EXPECT_EQ(0, pattern_bit_list[2]);
    EXPECT_EQ(0, pattern_bit_list[3]);
  }
  {
    // OBOBBからBOBBを検索
    const StateBit test_pattern = GetStateBit("OBOBB");
    const StateBit black_bit = GetBlackStoneBit(test_pattern);
    const StateBit open_bit = GetOpenPositionBit(test_pattern);

    constexpr size_t kPatternLength = 4;
    array<StateBit, kPatternLength> pattern_bit_list = {{0}};

    GetStoneWithOneOpenBit<kPatternLength>(black_bit, open_bit, &pattern_bit_list);

    EXPECT_EQ(0, pattern_bit_list[0]);
    EXPECT_EQ(0, pattern_bit_list[1]);

    constexpr StateBit expect_bit_1 = 0b01;
    EXPECT_EQ(expect_bit_1, pattern_bit_list[2]);

    EXPECT_EQ(0, pattern_bit_list[3]);
  }
}

TEST(BitSearchTest, GetNumberOfTrailingZerosTest)
{
  constexpr size_t kMaxBitCount = 64; 
  array<size_t, kMaxBitCount> shift_val_list;

  iota(shift_val_list.begin(), shift_val_list.end(), 0);

  constexpr size_t kRandomTestCount = 100;   // テスト数

  for(size_t bit_count=1; bit_count<=kMaxBitCount; ++bit_count){
    // bitが立つ数がbit_countあるデータでテストする
    for(size_t i=0; i<kRandomTestCount; i++){
      // ランダムデータをつくるためシフト量のリストをランダムシャッフルする
      shuffle(shift_val_list.begin(), shift_val_list.end(), mt19937_64());

      StateBit random_bit = 0ULL;
      size_t min_shift_val = kMaxBitCount;  // 右端のビット位置

      for(size_t index=0; index<bit_count; ++index){
        random_bit |= 1ULL << shift_val_list[index];
        min_shift_val = min(min_shift_val, shift_val_list[index]);
      }

      const size_t trailing_zeros = GetNumberOfTrailingZeros(random_bit);
      ASSERT_EQ(trailing_zeros, min_shift_val);
    }
  }
}

TEST(BitSearchTest, GetConsectiveBitTest)
{
  {
    // 境界値　下限
    constexpr StateBit calc_bit = GetConsectiveBit<1>();
    constexpr StateBit expect_bit = 0b1;

    EXPECT_EQ(expect_bit, calc_bit);
  }
  {
    constexpr StateBit calc_bit = GetConsectiveBit<33>();
    constexpr StateBit expect_bit = 0b111111111111111111111111111111111;
    
    EXPECT_EQ(expect_bit, calc_bit);
  }
  {
    // 境界値　上限
    constexpr StateBit calc_bit = GetConsectiveBit<63>();
    constexpr StateBit expect_bit = 0b111111111111111111111111111111111111111111111111111111111111111;
    
    EXPECT_EQ(expect_bit, calc_bit);
  }
}

}   // namespace realcore
