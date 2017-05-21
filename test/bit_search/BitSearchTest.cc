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

TEST(BitSearchTest, RightShiftTest)
{
  const StateBit state_bit = GetStateBit("BOWBXOWBXOWBXOWBXOWBXOWBXOWBXOWB");

  {
    // シフト量: 0
    const StateBit shift_bit = RightShift<0>(state_bit);
    const StateBit expect_bit = GetStateBit("BOWBXOWBXOWBXOWBXOWBXOWBXOWBXOWB");

    EXPECT_EQ(shift_bit, expect_bit);
  }
  {
    // シフト量: 0
    const StateBit shift_bit = RightShift(0, state_bit);
    const StateBit expect_bit = GetStateBit("BOWBXOWBXOWBXOWBXOWBXOWBXOWBXOWB");

    EXPECT_EQ(shift_bit, expect_bit);
  }
  {
    // シフト量: 1
    const StateBit shift_bit = RightShift<1>(state_bit);
    const StateBit expect_bit = GetStateBit("BOWBXOWBXOWBXOWBXOWBXOWBXOWBXOW");

    EXPECT_EQ(shift_bit, expect_bit);
  }
  {
    // シフト量: 1
    const StateBit shift_bit = RightShift(1, state_bit);
    const StateBit expect_bit = GetStateBit("BOWBXOWBXOWBXOWBXOWBXOWBXOWBXOW");

    EXPECT_EQ(shift_bit, expect_bit);
  }
  {
    // シフト量: 31(境界値)
    const StateBit shift_bit = RightShift<31>(state_bit);
    const StateBit expect_bit = GetStateBit("B");

    EXPECT_EQ(shift_bit, expect_bit);
  }
  {
    // シフト量: 31(境界値)
    const StateBit shift_bit = RightShift(31, state_bit);
    const StateBit expect_bit = GetStateBit("B");

    EXPECT_EQ(shift_bit, expect_bit);
  }
}

TEST(BitSearchTest, LeftShiftTest)
{
  const StateBit state_bit = GetStateBit("BOWBXOWBXOWBXOWBXOWBXOWBXOWBXOWB");

  {
    // シフト量: 0
    const StateBit shift_bit = LeftShift<0>(state_bit);
    const StateBit expect_bit = GetStateBit("BOWBXOWBXOWBXOWBXOWBXOWBXOWBXOWB");

    EXPECT_EQ(shift_bit, expect_bit);
  }
  {
    // シフト量: 0
    const StateBit shift_bit = LeftShift(0, state_bit);
    const StateBit expect_bit = GetStateBit("BOWBXOWBXOWBXOWBXOWBXOWBXOWBXOWB");

    EXPECT_EQ(shift_bit, expect_bit);
  }
  {
    // シフト量: 1
    const StateBit shift_bit = LeftShift<1>(state_bit);
    const StateBit expect_bit = GetStateBit("OWBXOWBXOWBXOWBXOWBXOWBXOWBXOWBX");

    EXPECT_EQ(shift_bit, expect_bit);
  }
  {
    // シフト量: 1
    const StateBit shift_bit = LeftShift(1, state_bit);
    const StateBit expect_bit = GetStateBit("OWBXOWBXOWBXOWBXOWBXOWBXOWBXOWBX");

    EXPECT_EQ(shift_bit, expect_bit);
  }
  {
    // シフト量: 31(境界値)
    const StateBit shift_bit = LeftShift<31>(state_bit);
    const StateBit expect_bit = GetStateBit("BXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");

    EXPECT_EQ(shift_bit, expect_bit);
  }
  {
    // シフト量: 31(境界値)
    const StateBit shift_bit = LeftShift(31, state_bit);
    const StateBit expect_bit = GetStateBit("BXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");

    EXPECT_EQ(shift_bit, expect_bit);
  }
}

TEST(BitSearchTest, GetIndexDifferenceTest)
{
  {
    // 差分値が正
    constexpr size_t index_from = 1, index_to = 10;
    constexpr int expect_diff = 9;
    
    const int diff = GetIndexDifference(index_from, index_to);
    EXPECT_EQ(expect_diff, diff);
  }
  {
    // 差分値が0
    constexpr size_t index_from = 1, index_to = 1;
    constexpr int expect_diff = 0;

    const int diff = GetIndexDifference(index_from, index_to);
    EXPECT_EQ(expect_diff, diff);
  }
  {
    // 差分値が負
    constexpr size_t index_from = 1, index_to = 0;
    constexpr int expect_diff = -1;

    const int diff = GetIndexDifference(index_from, index_to);
    EXPECT_EQ(expect_diff, diff);
  }
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

  const auto player_bit = GetPlayerStoneBit<kBlackTurn>(bit_board);
  EXPECT_EQ(expect_bit, player_bit);
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

  const auto player_bit = GetPlayerStoneBit<kWhiteTurn>(bit_board);
  EXPECT_EQ(expect_bit, player_bit);
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
    // OBBBOから[B3O1]を検索
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
    // OBBOBから[B3O1]を検索
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
    // OBOBBから[B3O1]を検索
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

TEST(BitSearchTest, GetStoneWithTwoOpenBitTest)
{
  {
    // OBBOBOBから[B2O2]を検索
    const StateBit test_pattern = GetStateBit("OBBOBOB");
    const StateBit stone_bit = GetBlackStoneBit(test_pattern);
    const StateBit open_bit = GetOpenPositionBit(test_pattern);
    constexpr size_t kPattern = kTwoOfFourPattern;
    
    array<uint64_t, kPattern> pattern_bit_list{{0}};
    GetStoneWithTwoOpenBit<kPattern>(stone_bit, open_bit, &pattern_bit_list);

    for(size_t i=0; i<kPattern; i++){
      const auto pattern_bit = pattern_bit_list[i];

      if(i == 4){
        // OBB|OBOB|
        constexpr uint64_t expect = LeftShift<0>(0b1);
        EXPECT_EQ(expect, pattern_bit);
      }else if(i == 1){
        // OB|BOBO|B
        constexpr uint64_t expect = LeftShift<1>(0b1);
        EXPECT_EQ(expect, pattern_bit);
      }else if(i == 3){
        // |OBBO|BOB
        constexpr uint64_t expect = LeftShift<3>(0b1);
        EXPECT_EQ(expect, pattern_bit);
      }else{
        EXPECT_EQ(0, pattern_bit);
      }
    }
  }
  {
    // OOBBBOOから[B3O2]を検索
    const StateBit test_pattern = GetStateBit("OOBBBOO");
    const StateBit stone_bit = GetBlackStoneBit(test_pattern);
    const StateBit open_bit = GetOpenPositionBit(test_pattern);
    constexpr size_t kPattern = kTwoOfFivePattern;
    
    array<uint64_t, kPattern> pattern_bit_list{{0}};
    GetStoneWithTwoOpenBit<kPattern>(stone_bit, open_bit, &pattern_bit_list);

    for(size_t i=0; i<kPattern; i++){
      const auto pattern_bit = pattern_bit_list[i];

      if(i == 0){
        // OO|BBBOO|
        constexpr uint64_t expect = LeftShift<0>(0b1);
        EXPECT_EQ(expect, pattern_bit);
      }else if(i == 6){
        // O|OBBBO|O
        constexpr uint64_t expect = LeftShift<1>(0b1);
        EXPECT_EQ(expect, pattern_bit);
      }else if(i == 9){
        // |OOBBB|OO
        constexpr uint64_t expect = LeftShift<2>(0b1);
        EXPECT_EQ(expect, pattern_bit);
      }else{
        EXPECT_EQ(0, pattern_bit);
      }
    }
  }
  {
    // WOWOWWWBから[W3O2]を検索
    const StateBit test_pattern = GetStateBit("WOWOWWWB");
    const StateBit stone_bit = GetWhiteStoneBit(test_pattern);
    const StateBit open_bit = GetOpenPositionBit(test_pattern);
    constexpr size_t kPattern = kTwoOfFivePattern;
    
    array<uint64_t, kPattern> pattern_bit_list{{0}};
    GetStoneWithTwoOpenBit<kPattern>(stone_bit, open_bit, &pattern_bit_list);

    for(size_t i=0; i<kPattern; i++){
      const auto pattern_bit = pattern_bit_list[i];

      if(i == 8){
        // W|OWOWW|WB
        constexpr uint64_t expect = LeftShift<2>(0b1);
        EXPECT_EQ(expect, pattern_bit);
      }else if(i == 4){
        // |WOWOW|WWB
        constexpr uint64_t expect = LeftShift<3>(0b1);
        EXPECT_EQ(expect, pattern_bit);
      }else{
        EXPECT_EQ(0, pattern_bit);
      }
    }
  }
}

TEST(BitSearchTest, GetStoneWithThreeOpenBitTest)
{
  {
    // OBOOBOBから[B2O3]を検索
    const StateBit test_pattern = GetStateBit("OBOOBOB");
    const StateBit stone_bit = GetBlackStoneBit(test_pattern);
    const StateBit open_bit = GetOpenPositionBit(test_pattern);
    constexpr size_t kPattern = kThreeOfFivePattern;
    
    array<uint64_t, kPattern> pattern_bit_list{{0}};
    GetStoneWithThreeOpenBit<kPattern>(stone_bit, open_bit, &pattern_bit_list);

    for(size_t i=0; i<kPattern; i++){
      const auto pattern_bit = pattern_bit_list[i];

      if(i == 8){
        // OB|OOBOB|
        constexpr uint64_t expect = LeftShift<0>(0b1);
        EXPECT_EQ(expect, pattern_bit);
      }else if(i == 2){
        // O|BOOBO|B
        constexpr uint64_t expect = LeftShift<1>(0b1);
        EXPECT_EQ(expect, pattern_bit);
      }else if(i == 7){
        // |OBOOB|OB
        constexpr uint64_t expect = LeftShift<2>(0b1);
        EXPECT_EQ(expect, pattern_bit);
      }else{
        EXPECT_EQ(0, pattern_bit);
      }
    }
  }
  {
    // OOBOBOOから[B1O3]を検索
    const StateBit test_pattern = GetStateBit("OOBOBOO");
    const StateBit stone_bit = GetBlackStoneBit(test_pattern);
    const StateBit open_bit = GetOpenPositionBit(test_pattern);
    constexpr size_t kPattern = kThreeOfFourPattern;
    
    array<uint64_t, kPattern> pattern_bit_list{{0}};
    GetStoneWithThreeOpenBit<kPattern>(stone_bit, open_bit, &pattern_bit_list);

    for(size_t i=0; i<kPattern; i++){
      const auto pattern_bit = pattern_bit_list[i];

      if(i == 1){
        // OOB|OBOO|
        constexpr uint64_t expect = LeftShift<0>(0b1);
        EXPECT_EQ(expect, pattern_bit);
      }else if(i == 2){
        // |OOBO|BOO
        constexpr uint64_t expect = LeftShift<3>(0b1);
        EXPECT_EQ(expect, pattern_bit);
      }else{
        EXPECT_EQ(0, pattern_bit);
      }
    }
  }
  {
    // WOWOWOWBから[W2O3]を検索
    const StateBit test_pattern = GetStateBit("WOWOWOWB");
    const StateBit stone_bit = GetWhiteStoneBit(test_pattern);
    const StateBit open_bit = GetOpenPositionBit(test_pattern);
    constexpr size_t kPattern = kThreeOfFivePattern;
    
    array<uint64_t, kPattern> pattern_bit_list{{0}};
    GetStoneWithThreeOpenBit<kPattern>(stone_bit, open_bit, &pattern_bit_list);

    for(size_t i=0; i<kPattern; i++){
      const auto pattern_bit = pattern_bit_list[i];

      if(i == 5){
        // W|OWOWO|WB
        constexpr uint64_t expect = LeftShift<2>(0b1);
        EXPECT_EQ(expect, pattern_bit);
      }else{
        EXPECT_EQ(0, pattern_bit);
      }
    }
  }
}

TEST(BitSearchTest, GetIndexOfTwoTest)
{
  set<pair<size_t, size_t>> index_pair;

  for(size_t i=0; i<10; i++){
    const auto less_index = GetLessIndexOfTwo(i);
    const auto greater_index = GetGreaterIndexOfTwo(i);

    index_pair.insert(pair<size_t, size_t>(less_index, greater_index));
  }

  for(size_t less_index=0; less_index<5; less_index++){
    for(size_t greater_index=less_index+1; greater_index<5; greater_index++){
      const auto pair_index = pair<size_t, size_t>(less_index, greater_index);

      ASSERT_TRUE(index_pair.find(pair_index) != index_pair.end());
    }
  }
}

TEST(BitSearchTest, GetIndexOfThreeTest)
{
  set<tuple<size_t, size_t, size_t>> index_tuple;

  for(size_t i=0; i<10; i++){
    const auto min_index = GetMinIndexOfThree(i);
    const auto median_index = GetMedianIndexOfThree(i);
    const auto max_index = GetMaxIndexOfThree(i);

    index_tuple.insert(tuple<size_t, size_t, size_t>(min_index, median_index, max_index));
  }

  for(size_t min_index=0; min_index<5; min_index++){
    for(size_t median_index=min_index+1; median_index<5; median_index++){
      for(size_t max_index=median_index+1; max_index<5; max_index++){
        const auto tuple_index = tuple<size_t, size_t, size_t>(min_index, median_index, max_index);

        ASSERT_TRUE(index_tuple.find(tuple_index) != index_tuple.end());
      }
    }
  }
}

TEST(BitSearchTest, IsSingleBit)
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

      for(size_t index=0; index<bit_count; ++index){
        random_bit |= 1ULL << shift_val_list[index];
      }

      const bool is_single_bit = IsSingleBit(random_bit);
      const bool is_fail = (bit_count == 1 && !is_single_bit) || (bit_count != 1 && is_single_bit);

      if(is_fail){
        cerr << "bit: " << random_bit << endl;
      }

      ASSERT_FALSE(is_fail);
    }
  }
}

TEST(BitSearchTest, IsMultipleOneBitVar)
{
  constexpr size_t kMaxBitCount = 64; 
  array<size_t, kMaxBitCount> shift_val_list;

  iota(shift_val_list.begin(), shift_val_list.end(), 0);

  constexpr size_t kRandomTestCount = 50000;   // テスト数
  size_t test_count = 0;

  random_device seed_generator;
  size_t seed = seed_generator();
  mt19937_64 random_generator(seed);

  while(test_count < kRandomTestCount)
  {
    ++test_count;

    uint64_t bit_count = random_generator() % 65ULL;

    // ランダムデータをつくるためシフト量のリストをランダムシャッフルする
    shuffle(shift_val_list.begin(), shift_val_list.end(), random_generator);

    uint64_t random_bit = 0ULL;

    for(size_t index=0; index<bit_count; ++index){
      random_bit |= 1ULL << shift_val_list[index];
    }

    shuffle(shift_val_list.begin(), shift_val_list.end(), random_generator);
    
    const bool is_multiple_bit = IsMultipleBit(random_bit);
    bool is_fail = (bit_count >= 2 && !is_multiple_bit);
    is_fail |= (bit_count < 2 && is_multiple_bit);

    if(is_fail){
      cerr << "bit: " << random_bit << endl;
    }

    ASSERT_FALSE(is_fail);
  }
}

TEST(BitSearchTest, IsMultipleTwoBitVars)
{
  constexpr size_t kMaxBitCount = 64; 
  array<size_t, kMaxBitCount> shift_val_list;

  iota(shift_val_list.begin(), shift_val_list.end(), 0);

  constexpr size_t kRandomTestCount = 50000;   // テスト数
  size_t test_count = 0;

  random_device seed_generator;
  size_t seed = seed_generator();
  mt19937_64 random_generator(seed);

  while(test_count < kRandomTestCount)
  {
    ++test_count;

    uint64_t bit_count_1 = random_generator() % 65ULL;
    uint64_t bit_count_2 = random_generator() % 65ULL;

    // ランダムデータをつくるためシフト量のリストをランダムシャッフルする
    shuffle(shift_val_list.begin(), shift_val_list.end(), random_generator);

    uint64_t random_bit_1 = 0ULL;

    for(size_t index=0; index<bit_count_1; ++index){
      random_bit_1 |= 1ULL << shift_val_list[index];
    }

    shuffle(shift_val_list.begin(), shift_val_list.end(), random_generator);
    
    uint64_t random_bit_2 = 0ULL;

    for(size_t index=0; index<bit_count_2; ++index){
      random_bit_2 |= 1ULL << shift_val_list[index];
    }

    const bool is_multiple_bit = IsMultipleBit(random_bit_1, random_bit_2);
    bool is_fail = (bit_count_1 + bit_count_2 >= 2 && !is_multiple_bit);
    is_fail |= (bit_count_1 + bit_count_2 < 2 && is_multiple_bit);

    if(is_fail){
      cerr << "bit_1: " << random_bit_1 << ", bit_2: " << random_bit_2 << endl;
    }

    ASSERT_FALSE(is_fail);
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

TEST(BitSearchTest, GetOpenBitInPatternTest)
{
  constexpr uint64_t test_bit = 0b1;

  for(size_t index=0; index<5; index++){
    const uint64_t expect = 0b1 << (index * 2);
    const auto result = GetOpenBitInPattern(index, test_bit);
  
    EXPECT_EQ(expect, result);
  }
}

TEST(BitSearchTest, GetOpenBoardPositionTest)
{
  {
    const auto result = GetOpenBoardPosition(128, 0);
    const BoardPosition expect = 128;
    EXPECT_EQ(expect, result);
  }
  {
    const auto result = GetOpenBoardPosition(128, 1);
    const BoardPosition expect = 129;
    EXPECT_EQ(expect, result);
  }
}

TEST(BitSearchTest, GetCombinedBitTest)
{
  constexpr uint64_t bit_1 = 0b0101, bit_2 = 0b0100;
  constexpr auto result = GetCombinedBit(bit_1, bit_2);
  constexpr uint64_t expected = 0b1101;

  ASSERT_EQ(expected, result);
}
}   // namespace realcore
