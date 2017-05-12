#include "gtest/gtest.h"

#include "MovePatternSearch.h"

using namespace std;

namespace realcore
{
TEST(MovePatternSearchTest, IsOverlineTest)
{
  {
    // 長連パターン(黒石6個)
    const StateBit test_bit = GetStateBit("BBBBBB");
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    EXPECT_TRUE(IsOverline(black_bit));
  }
  {
    // 長連パターン(黒石7個)
    const StateBit test_bit = GetStateBit("BBBBBBB");
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    EXPECT_TRUE(IsOverline(black_bit));
  }
  {
    // 非長連パターン(黒石5個)
    const StateBit test_bit = GetStateBit("BBBBB");
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    EXPECT_FALSE(IsOverline(black_bit));
  }
  {
    // 非長連パターン(白石6個)
    const StateBit test_bit = GetStateBit("WWWWWW");
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    EXPECT_FALSE(IsOverline(black_bit));
  }
}

TEST(MovePatternSearchTest, GetOpenStatePatternNumTest)
{
  EXPECT_EQ(4ULL, GetOpenStatePatternNum(kNextOverline));
  EXPECT_EQ(4ULL, GetOpenStatePatternNum(kNextOpenFourBlack));
  EXPECT_EQ(4ULL, GetOpenStatePatternNum(kNextOpenFourWhite));
  EXPECT_EQ(10ULL, GetOpenStatePatternNum(kNextFourBlack));
  EXPECT_EQ(10ULL, GetOpenStatePatternNum(kNextFourWhite));
  EXPECT_EQ(6ULL, GetOpenStatePatternNum(kNextSemiThreeBlack));
  EXPECT_EQ(6ULL, GetOpenStatePatternNum(kNextSemiThreeWhite));
  EXPECT_EQ(10ULL, GetOpenStatePatternNum(kNextPointOfSwordBlack));
  EXPECT_EQ(10ULL, GetOpenStatePatternNum(kNextPointOfSwordWhite));
  EXPECT_EQ(4ULL, GetOpenStatePatternNum(kNextTwoBlack));
  EXPECT_EQ(4ULL, GetOpenStatePatternNum(kNextTwoWhite));
}

TEST(MovePatternSearchTest, SearchNextOverlineTest)
{
  {
    // 長連点パターン
    const string pattern = "BBOBBB";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetBlackStoneBit(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kFourStonePattern> pattern_search_bit_list{{0}};
    SearchNextOverline(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kFourStonePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];
      const auto open_state_bit = GetOpenBitInPattern(i, pattern_search_bit_list[i]);

      if(i == 2){
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);

        constexpr uint64_t expect_open_state_bit = LeftShift<3>(0b1);
        ASSERT_EQ(expect_open_state_bit, open_state_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
        ASSERT_EQ(0ULL, open_state_bit);
      }
    }
  }
  {
    // 長連点パターン
    const string pattern = "BBBOBBB";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetBlackStoneBit(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kFourStonePattern> pattern_search_bit_list{{0}};
    SearchNextOverline(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kFourStonePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];
      const auto open_state_bit = GetOpenBitInPattern(i, pattern_search_bit_list[i]);

      if(i == 1){
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);

        constexpr uint64_t expect_open_state_bit = LeftShift<3>(0b1);
        ASSERT_EQ(expect_open_state_bit, open_state_bit);
      }else if(i == 2){
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);

        constexpr uint64_t expect_open_state_bit = LeftShift<3>(0b1);
        ASSERT_EQ(expect_open_state_bit, open_state_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
        ASSERT_EQ(0ULL, open_state_bit);
      }
    }
  }
  {
    // 非長連パターン
    const string pattern = "BBOBB";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetBlackStoneBit(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kFourStonePattern> pattern_search_bit_list{{0}};
    SearchNextOverline(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kFourStonePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];
      const auto open_state_bit = GetOpenBitInPattern(i, pattern_search_bit_list[i]);

      ASSERT_EQ(0ULL, search_bit);
      ASSERT_EQ(0ULL, open_state_bit);
    }
  }
}

TEST(MovePatternSearchTest, SearchOpenFourTest)
{
  {
    // 達四パターン(黒番)
    const StateBit test_bit = GetStateBit("BWOBBBBO");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    const uint64_t search_bit = SearchOpenFour<kBlackTurn>(black_bit, open_bit);

    constexpr uint64_t expect_bit = LeftShift<1>(0b1);
    EXPECT_EQ(expect_bit, search_bit);
  }
  {
    // 達四パターン(黒番)
    const StateBit test_bit = GetStateBit("OBBBBOW");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    const uint64_t search_bit = SearchOpenFour<kBlackTurn>(black_bit, open_bit);

    constexpr uint64_t expect_bit = LeftShift<2>(0b1);
    EXPECT_EQ(expect_bit, search_bit);      
  }
  {
    // 達四パターン(黒番)
    const StateBit test_bit = GetStateBit("OBBBBO");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    const uint64_t search_bit = SearchOpenFour<kBlackTurn>(black_bit, open_bit);

    constexpr uint64_t expect_bit = LeftShift<1>(0b1);;
    EXPECT_EQ(expect_bit, search_bit);      
  }
  {
    // 達四パターン(黒番)
    const StateBit test_bit = GetStateBit("BWOBBBBOXB");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    const uint64_t search_bit = SearchOpenFour<kBlackTurn>(black_bit, open_bit);

    constexpr uint64_t expect_bit = LeftShift<3>(0b1);;
    EXPECT_EQ(expect_bit, search_bit);      
  }
  {
    // 非達四パターン（黒番, 長連筋）
    const StateBit test_bit = GetStateBit("WOBBBBOB");

    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    const uint64_t search_bit = SearchOpenFour<kBlackTurn>(black_bit, open_bit);

    constexpr uint64_t expect_bit = 0;
    EXPECT_EQ(expect_bit, search_bit);      
  }
  {
    // 非達四パターン（黒番, 長連筋）
    const StateBit test_bit = GetStateBit("BOBBBBOX");

    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    const uint64_t search_bit = SearchOpenFour<kBlackTurn>(black_bit, open_bit);

    constexpr uint64_t expect_bit = 0;
    EXPECT_EQ(expect_bit, search_bit);      
  }
  {
    // 非達四パターン(黒番)
    const StateBit test_bit = GetStateBit("BWOBBBOXB");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    const uint64_t search_bit = SearchOpenFour<kBlackTurn>(black_bit, open_bit);

    constexpr uint64_t expect_bit = 0;
    EXPECT_EQ(expect_bit, search_bit);
  }
  {
    // 達四パターン(白番)
    const StateBit test_bit = GetStateBit("BBOWWWWOXB");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    const uint64_t search_bit = SearchOpenFour<kWhiteTurn>(white_bit, open_bit);

    constexpr uint64_t expect_bit = LeftShift<3>(0b1);;
    EXPECT_EQ(expect_bit, search_bit);      
  }
  {
    // 達四パターン(白番, 長連筋)
    const StateBit test_bit = GetStateBit("BWOWWWWOWB");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    const uint64_t search_bit = SearchOpenFour<kWhiteTurn>(white_bit, open_bit);

    constexpr uint64_t expect_bit = LeftShift<3>(0b1);;
    EXPECT_EQ(expect_bit, search_bit);      
  }
  {
    // 非達四パターン(白番)
    const StateBit test_bit = GetStateBit("BBOWWWOXB");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    const uint64_t search_bit = SearchOpenFour<kWhiteTurn>(white_bit, open_bit);

    constexpr uint64_t expect_bit = 0;
    EXPECT_EQ(expect_bit, search_bit);      
  }
}

TEST(MovePatternSearchTest, SearchNextOpenFourBlackTest)
{
  constexpr PlayerTurn P = kBlackTurn;

  {
    // 四連点パターン_1(黒, 三連)
    const string pattern = "OOBBBOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kFourStonePattern> pattern_search_bit_list{{0}};
    SearchNextOpenFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kFourStonePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];
      const auto open_state_bit = GetOpenBitInPattern(i, pattern_search_bit_list[i]);

      if(i == 0){
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);

        constexpr uint64_t expect_open_state_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_open_state_bit, open_state_bit);
      }else if(i == 3){
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);

        constexpr uint64_t expect_open_state_bit = LeftShift<5>(0b1);
        ASSERT_EQ(expect_open_state_bit, open_state_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
        ASSERT_EQ(0ULL, open_state_bit);
      }
    }
  }
  {
    // 四連点パターン_2(黒, トビ三)
    const string pattern = "OBOBBOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kFourStonePattern> pattern_search_bit_list{{0}};
    SearchNextOpenFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kFourStonePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];
      const auto open_state_bit = GetOpenBitInPattern(i, pattern_search_bit_list[i]);

      if(i == 2){
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);

        constexpr uint64_t expect_open_state_bit = LeftShift<4>(0b1);
        ASSERT_EQ(expect_open_state_bit, open_state_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
        ASSERT_EQ(0ULL, open_state_bit);
      }
    }
  }
  {
    // 四連点パターン_3(黒, 長連筋)
    const string pattern = "BOOBBBOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kFourStonePattern> pattern_search_bit_list{{0}};
    SearchNextOpenFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kFourStonePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];
      const auto open_state_bit = GetOpenBitInPattern(i, pattern_search_bit_list[i]);

      if(i == 0){
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);

        constexpr uint64_t expect_open_state_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_open_state_bit, open_state_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
        ASSERT_EQ(0ULL, open_state_bit);
      }
    }
  }
  {
    // 否四連点パターン(黒, 長連筋)
    const string pattern = "BOOBBBOOB";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kFourStonePattern> pattern_search_bit_list{{0}};
    SearchNextOpenFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kFourStonePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];
      const auto open_state_bit = GetOpenBitInPattern(i, pattern_search_bit_list[i]);

      ASSERT_EQ(0ULL, search_bit);
      ASSERT_EQ(0ULL, open_state_bit);
    }
  }
  {
    // 否四連点パターン(黒)
    const string pattern = "BOOBBOOB";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kFourStonePattern> pattern_search_bit_list{{0}};
    SearchNextOpenFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kFourStonePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];
      const auto open_state_bit = GetOpenBitInPattern(i, pattern_search_bit_list[i]);

      ASSERT_EQ(0ULL, search_bit);
      ASSERT_EQ(0ULL, open_state_bit);
    }
  }
}

TEST(MovePatternSearchTest, SearchNextOpenFourWhiteTest)
{
  constexpr PlayerTurn P = kWhiteTurn;

  {
    // 四連点パターン_1(白, 三連)
    const string pattern = "OOWWWOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kFourStonePattern> pattern_search_bit_list{{0}};
    SearchNextOpenFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kFourStonePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];
      const auto open_state_bit = GetOpenBitInPattern(i, pattern_search_bit_list[i]);

      if(i == 0){
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);

        constexpr uint64_t expect_open_state_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_open_state_bit, open_state_bit);
      }else if(i == 3){
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);

        constexpr uint64_t expect_open_state_bit = LeftShift<5>(0b1);
        ASSERT_EQ(expect_open_state_bit, open_state_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
        ASSERT_EQ(0ULL, open_state_bit);
      }
    }
  }
  {
    // 四連点パターン_2(白, トビ三)
    const string pattern = "OWOWWOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kFourStonePattern> pattern_search_bit_list{{0}};
    SearchNextOpenFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kFourStonePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];
      const auto open_state_bit = GetOpenBitInPattern(i, pattern_search_bit_list[i]);

      if(i == 2){
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);

        constexpr uint64_t expect_open_state_bit = LeftShift<4>(0b1);
        ASSERT_EQ(expect_open_state_bit, open_state_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
        ASSERT_EQ(0ULL, open_state_bit);
      }
    }
  }
  {
    // 四連点パターン_3(白, 長連筋)
    const string pattern = "WOOWWWOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kFourStonePattern> pattern_search_bit_list{{0}};
    SearchNextOpenFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kFourStonePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];
      const auto open_state_bit = GetOpenBitInPattern(i, pattern_search_bit_list[i]);

      if(i == 0){
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);

        constexpr uint64_t expect_open_state_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_open_state_bit, open_state_bit);
      }else if(i == 3){
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);

        constexpr uint64_t expect_open_state_bit = LeftShift<5>(0b1);
        ASSERT_EQ(expect_open_state_bit, open_state_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
        ASSERT_EQ(0ULL, open_state_bit);
      }
    }
  }
  {
    // 否四連点パターン(白)
    const string pattern = "WOOWWOOW";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kFourStonePattern> pattern_search_bit_list{{0}};
    SearchNextOpenFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kFourStonePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];
      const auto open_state_bit = GetOpenBitInPattern(i, pattern_search_bit_list[i]);

      ASSERT_EQ(0ULL, search_bit);
      ASSERT_EQ(0ULL, open_state_bit);
    }
  }
}

TEST(MovePatternSearchTest, SearchFourTest)
{
  {
    // 四パターン(黒番, 達四)
    const StateBit test_bit = GetStateBit("BWOBBBBO");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t guard_bit = 0;
    const uint64_t search_bit = SearchFour<kBlackTurn>(black_bit, open_bit, &guard_bit);

    constexpr uint64_t expect_bit = (0b1 << 0) | (0b1 << 2);
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_guard_bit = (0b1 << 0) | (0b1 << 10);
    EXPECT_EQ(expect_guard_bit, guard_bit);
  }
  {
    // 四パターン(黒番)
    const StateBit test_bit = GetStateBit("OBBBOBOW");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t guard_bit = 0;
    const uint64_t search_bit = SearchFour<kBlackTurn>(black_bit, open_bit, &guard_bit);

    constexpr uint64_t expect_bit = (0b1 << 4);
    EXPECT_EQ(expect_bit, search_bit);      

    constexpr uint64_t expect_guard_bit = (0b1 << 6);
    EXPECT_EQ(expect_guard_bit, guard_bit);
  }
  {
    // 四パターン(黒番)
    const StateBit test_bit = GetStateBit("OBBOBBOW");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t guard_bit = 0;
    const uint64_t search_bit = SearchFour<kBlackTurn>(black_bit, open_bit, &guard_bit);

    constexpr uint64_t expect_bit = (0b1 << 4);
    EXPECT_EQ(expect_bit, search_bit);      

    constexpr uint64_t expect_guard_bit = (0b1 << 8);
    EXPECT_EQ(expect_guard_bit, guard_bit);
  }
  {
    // 四パターン(黒番)
    const StateBit test_bit = GetStateBit("OBOBBBOW");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t guard_bit = 0;
    const uint64_t search_bit = SearchFour<kBlackTurn>(black_bit, open_bit, &guard_bit);

    constexpr uint64_t expect_bit = (0b1 << 4);
    EXPECT_EQ(expect_bit, search_bit);      

    constexpr uint64_t expect_guard_bit = (0b1 << 10);
    EXPECT_EQ(expect_guard_bit, guard_bit);
  }
  {
    // 四パターン(黒番, 達四)
    const StateBit test_bit = GetStateBit("OOBBBBOW");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t guard_bit = 0;
    const uint64_t search_bit = SearchFour<kBlackTurn>(black_bit, open_bit, &guard_bit);

    constexpr uint64_t expect_bit = (0b1 << 2) | (0b1 << 4);
    EXPECT_EQ(expect_bit, search_bit);      

    constexpr uint64_t expect_guard_bit = (0b1 << 2) | (0b1 << 12);
    EXPECT_EQ(expect_guard_bit, guard_bit);
  }
  {
    // 非四パターン（黒番, 長連筋）
    const StateBit test_bit = GetStateBit("WBBBBOB");

    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t guard_bit = 0;
    const uint64_t search_bit = SearchFour<kBlackTurn>(black_bit, open_bit, &guard_bit);

    constexpr uint64_t expect_bit = 0;
    EXPECT_EQ(expect_bit, search_bit);      

    constexpr uint64_t expect_guard_bit = 0;
    EXPECT_EQ(expect_guard_bit, guard_bit);
  }
  {
    // 非四パターン（黒番, 長連筋）
    const StateBit test_bit = GetStateBit("BOBBBBX");

    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t guard_bit = 0;
    const uint64_t search_bit = SearchFour<kBlackTurn>(black_bit, open_bit, &guard_bit);

    constexpr uint64_t expect_bit = 0;
    EXPECT_EQ(expect_bit, search_bit);      

    constexpr uint64_t expect_guard_bit = 0;
    EXPECT_EQ(expect_guard_bit, guard_bit);
  }
  {
    // 非四パターン(黒番)
    const StateBit test_bit = GetStateBit("BWOBBBOXB");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t guard_bit = 0;
    const uint64_t search_bit = SearchFour<kBlackTurn>(black_bit, open_bit, &guard_bit);

    constexpr uint64_t expect_bit = 0;
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_guard_bit = 0;
    EXPECT_EQ(expect_guard_bit, guard_bit);
  }
  {
    // 四パターン(白番, 達四)
    const StateBit test_bit = GetStateBit("BBOWWWWOXB");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t guard_bit = 0;
    const uint64_t search_bit = SearchFour<kWhiteTurn>(white_bit, open_bit, &guard_bit);

    constexpr uint64_t expect_bit = (0b1 << 4) | (0b1 << 6);
    EXPECT_EQ(expect_bit, search_bit);      

    constexpr uint64_t expect_guard_bit = (0b1 << 4) | (0b1 << 14);
    EXPECT_EQ(expect_guard_bit, guard_bit);
  }
  {
    // 四パターン(白番)
    const StateBit test_bit = GetStateBit("BBOWWWOWXB");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t guard_bit = 0;
    const uint64_t search_bit = SearchFour<kWhiteTurn>(white_bit, open_bit, &guard_bit);

    constexpr uint64_t expect_bit = (0b1 << 4);
    EXPECT_EQ(expect_bit, search_bit);      

    constexpr uint64_t expect_guard_bit = (0b1 << 6);
    EXPECT_EQ(expect_guard_bit, guard_bit);
  }
  {
    // 四パターン(白番)
    const StateBit test_bit = GetStateBit("BBOWWOWWXB");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t guard_bit = 0;
    const uint64_t search_bit = SearchFour<kWhiteTurn>(white_bit, open_bit, &guard_bit);

    constexpr uint64_t expect_bit = (0b1 << 4);
    EXPECT_EQ(expect_bit, search_bit);      

    constexpr uint64_t expect_guard_bit = (0b1 << 8);
    EXPECT_EQ(expect_guard_bit, guard_bit);
  }
  {
    // 四パターン(白番)
    const StateBit test_bit = GetStateBit("BBOWOWWWXB");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t guard_bit = 0;
    const uint64_t search_bit = SearchFour<kWhiteTurn>(white_bit, open_bit, &guard_bit);

    constexpr uint64_t expect_bit = (0b1 << 4);
    EXPECT_EQ(expect_bit, search_bit);      

    constexpr uint64_t expect_guard_bit = (0b1 << 10);
    EXPECT_EQ(expect_guard_bit, guard_bit);
  }
  {
    // 四パターン(白番)
    const StateBit test_bit = GetStateBit("BBOOWWWWXB");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t guard_bit = 0;
    const uint64_t search_bit = SearchFour<kWhiteTurn>(white_bit, open_bit, &guard_bit);

    constexpr uint64_t expect_bit = (0b1 << 4);
    EXPECT_EQ(expect_bit, search_bit);      

    constexpr uint64_t expect_guard_bit = (0b1 << 12);
    EXPECT_EQ(expect_guard_bit, guard_bit);
  }
  {
    // 四パターン(白番, 長連筋)
    const StateBit test_bit = GetStateBit("BBWOWWWWXB");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t guard_bit = 0;
    const uint64_t search_bit = SearchFour<kWhiteTurn>(white_bit, open_bit, &guard_bit);

    constexpr uint64_t expect_bit = (0b1 << 4) | (0b1 << 6);
    EXPECT_EQ(expect_bit, search_bit);      

    constexpr uint64_t expect_guard_bit = (0b1 << 12);
    EXPECT_EQ(expect_guard_bit, guard_bit);
  }
  {
    // 非四パターン(白番)
    const StateBit test_bit = GetStateBit("BBOWWWOXB");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t guard_bit = 0;
    const uint64_t search_bit = SearchFour<kWhiteTurn>(white_bit, open_bit, &guard_bit);

    constexpr uint64_t expect_bit = 0;
    EXPECT_EQ(expect_bit, search_bit);      

    constexpr uint64_t expect_guard_bit = 0;
    EXPECT_EQ(expect_guard_bit, guard_bit);
  }
}

TEST(MovePatternSearchTest, SearchNextFourBlackTest)
{
  constexpr PlayerTurn P = kBlackTurn;

  {
    // 四ノビ点パターン_1(黒, 三連)
    const string pattern = "OOBBBOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kTwoOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kTwoOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 0){
        // OO|BBBOO|
        constexpr uint64_t expect_bit = LeftShift<0>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 6){
        // O|OBBBO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 9){
        // |OOBBB|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 四ノビ点パターン_2(黒, トビ三)
    const string pattern = "OBOBBOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kTwoOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kTwoOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 3){
        // O|BOBBO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 8){
        // |OBOBB|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 四ノビ点パターン_3(黒, 長連筋)
    const string pattern = "BOOBBBOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kTwoOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kTwoOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 0){
        // BOO|BBBOO|
        constexpr uint64_t expect_bit = LeftShift<0>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 6){
        // BO|OBBBO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 否四ノビ点パターン(黒, 長連筋)
    const string pattern = "BBOBBBOOB";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kTwoOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kTwoOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      ASSERT_EQ(0ULL, search_bit);
    }
  }
  {
    // 否四ノビ点パターン(黒)
    const string pattern = "WOOBBOOX";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kTwoOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kTwoOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      ASSERT_EQ(0ULL, search_bit);
    }
  }
}

TEST(MovePatternSearchTest, SearchNextFourWhiteTest)
{
  constexpr PlayerTurn P = kWhiteTurn;

  {
    // 四ノビ点パターン_1(白, 三連)
    const string pattern = "OOWWWOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kTwoOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kTwoOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 0){
        // OO|WWWOO|
        constexpr uint64_t expect_bit = LeftShift<0>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 6){
        // O|OWWWO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 9){
        // |OOWWW|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 四ノビ点パターン_2(白, トビ三)
    const string pattern = "OWOWWOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kTwoOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kTwoOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 3){
        // O|WOWWO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 8){
        // |OWOWW|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 四ノビ点パターン_3(白, 長連筋)
    const string pattern = "WOOWWWOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kTwoOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kTwoOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 0){
        // WOO|WWWOO|
        constexpr uint64_t expect_bit = LeftShift<0>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 6){
        // WO|OWWWO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 9){
        // W|OOWWW|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 5){
        // |WOOWW|WOO
        constexpr uint64_t expect_bit = LeftShift<3>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 否四ノビ点パターン(白)
    const string pattern = "BOOWWOOB";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kTwoOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextFour<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kTwoOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];
      ASSERT_EQ(0ULL, search_bit);
    }
  }
}

TEST(MovePatternSearchTest, SearchSemiThreeBlackTest)
{
  {
    // 三パターン(黒番)
    const StateBit test_bit = GetStateBit("OOBBBOO");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = LeftShift<1>(0b1) | LeftShift<2>(0b1);
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = LeftShift<1>(0b1) | LeftShift<5>(0b1);
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 三パターン(黒番)
    const StateBit test_bit = GetStateBit("OOBBOBO");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = LeftShift<1>(0b1);
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = LeftShift<2>(0b1);
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 三パターン(黒番)
    const StateBit test_bit = GetStateBit("OOBOBBO");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = LeftShift<1>(0b1);
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = LeftShift<3>(0b1);
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 三パターン(黒番)
    const StateBit test_bit = GetStateBit("OOOBBBO");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = LeftShift<1>(0b1);
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = LeftShift<4>(0b1);
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 三パターン(黒番, 長連筋)
    const StateBit test_bit = GetStateBit("BOOBBBOO");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = LeftShift<1>(0b1);
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = LeftShift<1>(0b1);
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 三パターン(黒番, 長連筋)
    const StateBit test_bit = GetStateBit("OOBBBOOB");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = LeftShift<3>(0b1);
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = LeftShift<6>(0b1);
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 非三パターン(黒番, 長連筋)
    const StateBit test_bit = GetStateBit("BOOBBBOOB");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = 0;
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = 0;
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 非三パターン(黒番, 夏止め)
    const StateBit test_bit = GetStateBit("WOBBBOW");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = 0;
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = 0;
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 非三パターン(黒番)
    const StateBit test_bit = GetStateBit("BBOBBOOBB");
    
    const uint64_t black_bit = GetBlackStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = 0;
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = 0;
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 三パターン(白番)
    const StateBit test_bit = GetStateBit("OOWWWOO");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kWhiteTurn>(white_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = LeftShift<1>(0b1) | LeftShift<2>(0b1);
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = LeftShift<1>(0b1) | LeftShift<5>(0b1);
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 三パターン(白番)
    const StateBit test_bit = GetStateBit("OOWWOWO");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kWhiteTurn>(white_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = LeftShift<1>(0b1);
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = LeftShift<2>(0b1);
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 三パターン(白番)
    const StateBit test_bit = GetStateBit("OOWOWWO");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kWhiteTurn>(white_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = LeftShift<1>(0b1);
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = LeftShift<3>(0b1);
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 三パターン(白番)
    const StateBit test_bit = GetStateBit("OOOWWWO");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kWhiteTurn>(white_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = LeftShift<1>(0b1);
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = (0b1 << 8);
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 三パターン(白番, 長連筋)
    const StateBit test_bit = GetStateBit("WOOOWWWO");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kWhiteTurn>(white_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = LeftShift<1>(0b1);
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = LeftShift<4>(0b1);
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 三パターン(白番, 長連筋)
    const StateBit test_bit = GetStateBit("WOOWWWOOW");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kWhiteTurn>(white_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = LeftShift<2>(0b1) | LeftShift<3>(0b1);
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = LeftShift<2>(0b1) | LeftShift<6>(0b1);
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 非三パターン(白番, 夏止め)
    const StateBit test_bit = GetStateBit("BOWWWOX");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kWhiteTurn>(white_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = 0;
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = 0;
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
  {
    // 非三パターン(白番)
    const StateBit test_bit = GetStateBit("OWWOWWOOWW");
    
    const uint64_t white_bit = GetWhiteStoneBit(test_bit);
    const uint64_t open_bit = GetOpenPositionBit(test_bit);
    uint64_t next_open_four_bit = 0;
    const uint64_t search_bit = SearchSemiThree<kWhiteTurn>(white_bit, open_bit, &next_open_four_bit);

    constexpr uint64_t expect_bit = 0;
    EXPECT_EQ(expect_bit, search_bit);

    constexpr uint64_t expect_open_four_bit = 0;
    EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
  }
}

TEST(MovePatternSearchTest, SearchNextSemiThreeBlackTest)
{
  constexpr PlayerTurn P = kBlackTurn;
  constexpr size_t kPatternCount = kTwoOfFourPattern;

  {
    // 見かけの三ノビ点パターン_1(黒, 三連)
    const string pattern = "OOBOBOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kPatternCount> pattern_search_bit_list{{0}};
    SearchNextSemiThree<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kPatternCount; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 1){
        // OO|BOBO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 4){
        // O|OBOB|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 見かけの三ノビ点パターン_2(黒, 長連筋)
    const string pattern = "BOOOBBOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kPatternCount> pattern_search_bit_list{{0}};
    SearchNextSemiThree<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kPatternCount; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 3){
        // BOO|OBBO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 否見かけの三ノビ点パターン(黒, 長連筋)
    const string pattern = "BBOBOBOOB";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kPatternCount> pattern_search_bit_list{{0}};
    SearchNextSemiThree<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kPatternCount; i++){
      const auto search_bit = pattern_search_bit_list[i];
      ASSERT_EQ(0ULL, search_bit);
    }
  }
  {
    // 否見かけの三ノビ点パターン(黒)
    const string pattern = "WOOBOOOX";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kPatternCount> pattern_search_bit_list{{0}};
    SearchNextSemiThree<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kPatternCount; i++){
      const auto search_bit = pattern_search_bit_list[i];
      ASSERT_EQ(0ULL, search_bit);
    }
  }
}

TEST(MovePatternSearchTest, SearchNextSemiThreeWhiteTest)
{
  constexpr PlayerTurn P = kWhiteTurn;
  constexpr size_t kPatternCount = kTwoOfFourPattern;

  {
    // 見かけの三ノビ点パターン_1(白, 三連)
    const string pattern = "OOWOWOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kPatternCount> pattern_search_bit_list{{0}};
    SearchNextSemiThree<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kPatternCount; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 1){
        // OO|WOWO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 4){
        // O|OWOW|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }
      else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 見かけの三ノビ点パターン_2(白, トビ三)
    const string pattern = "OWOOWOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kPatternCount> pattern_search_bit_list{{0}};
    SearchNextSemiThree<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kPatternCount; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 2){
        // O|WOOW|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 見かけの三ノビ点パターン_3(白, 長連筋)
    const string pattern = "WOOWOWOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kPatternCount> pattern_search_bit_list{{0}};
    SearchNextSemiThree<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kPatternCount; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 1){
        // WOO|WOWO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 4){
        // WO|OWOW|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 否見かけの三ノビ点パターン(白)
    const string pattern = "BOOWOOWB";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kPatternCount> pattern_search_bit_list{{0}};
    SearchNextSemiThree<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kPatternCount; i++){
      const auto search_bit = pattern_search_bit_list[i];
      ASSERT_EQ(0ULL, search_bit);
    }
  }
}

TEST(MovePatternSearchTest, SearchNextPointOfSwordBlackTest)
{
  constexpr PlayerTurn P = kBlackTurn;

  {
    // 剣先点パターン_1(黒, 二連)
    const string pattern = "OOOBBOOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kThreeOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextPointOfSword<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kThreeOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 0){
        // OOO|BBOOO|
        constexpr uint64_t expect_bit = LeftShift<0>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 4){
        // OO|OBBOO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 6){
        // O|OOBBO|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 9){
        // |OOOBB|OOO
        constexpr uint64_t expect_bit = LeftShift<3>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 剣先点パターン_2(黒, トビ二)
    const string pattern = "OBOOBOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kThreeOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextPointOfSword<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kThreeOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 2){
        // O|BOOBO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 7){
        // |OBOOB|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 剣先点パターン_3(黒, 長連筋)
    const string pattern = "BOOBOBOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kThreeOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextPointOfSword<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kThreeOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 1){
        // BOO|BOBOO|
        constexpr uint64_t expect_bit = LeftShift<0>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 5){
        // BO|OBOBO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 否剣先点パターン(黒, 長連筋)
    const string pattern = "BBOBOBOOB";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kThreeOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextPointOfSword<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kThreeOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      ASSERT_EQ(0ULL, search_bit);
    }
  }
  {
    // 否剣先点パターン(黒)
    const string pattern = "WOOOBOOX";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kThreeOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextPointOfSword<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kThreeOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      ASSERT_EQ(0ULL, search_bit);
    }
  }
}

TEST(MovePatternSearchTest, SearchNextPointOfSwordWhiteTest)
{
  constexpr PlayerTurn P = kWhiteTurn;

  {
    // 剣先点パターン_1(白, 二連)
    const string pattern = "OOOWWOOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kThreeOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextPointOfSword<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kThreeOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 0){
        // OOO|WWOOO|
        constexpr uint64_t expect_bit = LeftShift<0>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 4){
        // OO|OWWOO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 6){
        // O|OOWWO|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 9){
        // |OOOWW|OOO
        constexpr uint64_t expect_bit = LeftShift<3>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 剣先点パターン_2(白, トビ二)
    const string pattern = "OWOWOOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kThreeOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextPointOfSword<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kThreeOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 1){
        // O|WOWOO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 5){
        // |OWOWO|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 剣先点パターン_3(白, 長連筋)
    const string pattern = "WOOWOWOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kThreeOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextPointOfSword<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kThreeOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 1){
        // WOO|WOWOO|
        constexpr uint64_t expect_bit = LeftShift<0>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 5){
        // WO|OWOWO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 8){
        // W|OOWOW|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 2){
        // |WOOWO|WOO
        constexpr uint64_t expect_bit = LeftShift<3>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 否剣先点パターン(白)
    const string pattern = "BOOOWOOB";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kThreeOfFivePattern> pattern_search_bit_list{{0}};
    SearchNextPointOfSword<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kThreeOfFivePattern; i++){
      const auto search_bit = pattern_search_bit_list[i];
      ASSERT_EQ(0ULL, search_bit);
    }
  }
}

TEST(MovePatternSearchTest, SearchNextTwoBlackTest)
{
  constexpr PlayerTurn P = kBlackTurn;
  constexpr size_t kPatternCount = kThreeOfFourPattern;

  {
    // 二ノビ点パターン_1(黒, 三連)
    const string pattern = "OOOOBOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kPatternCount> pattern_search_bit_list{{0}};
    SearchNextTwo<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kPatternCount; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 2){
        // OO|OOBO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 3){
        // O|OOOB|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 二ノビ点パターン_2(黒, 長連筋)
    const string pattern = "BOOOOBOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kPatternCount> pattern_search_bit_list{{0}};
    SearchNextTwo<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kPatternCount; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 2){
        // BOO|OOBO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 否二ノビ点パターン(黒, 長連筋)
    const string pattern = "BBOOOBOOB";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kPatternCount> pattern_search_bit_list{{0}};
    SearchNextTwo<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kPatternCount; i++){
      const auto search_bit = pattern_search_bit_list[i];
      ASSERT_EQ(0ULL, search_bit);
    }
  }
  {
    // 否二ノビ点パターン(黒)
    const string pattern = "WOOOOOOX";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kPatternCount> pattern_search_bit_list{{0}};
    SearchNextTwo<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kPatternCount; i++){
      const auto search_bit = pattern_search_bit_list[i];
      ASSERT_EQ(0ULL, search_bit);
    }
  }
}

TEST(MovePatternSearchTest, SearchNextTwoWhiteTest)
{
  constexpr PlayerTurn P = kWhiteTurn;
  constexpr size_t kPatternCount = kThreeOfFourPattern;

  {
    // 二ノビ点パターン_1(白, 三連)
    const string pattern = "OOOOWOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kPatternCount> pattern_search_bit_list{{0}};
    SearchNextTwo<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kPatternCount; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 2){
        // OO|OOWO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 3){
        // O|OOOW|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }
      else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 二ノビ点パターン_3(白, 長連筋)
    const string pattern = "WOOOOWOO";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kPatternCount> pattern_search_bit_list{{0}};
    SearchNextTwo<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kPatternCount; i++){
      const auto search_bit = pattern_search_bit_list[i];

      if(i == 2){
        // WOO|OOWO|O
        constexpr uint64_t expect_bit = LeftShift<1>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else if(i == 3){
        // WO|OOOW|OO
        constexpr uint64_t expect_bit = LeftShift<2>(0b1);
        ASSERT_EQ(expect_bit, search_bit);
      }else{
        ASSERT_EQ(0ULL, search_bit);
      }
    }
  }
  {
    // 否二ノビ点パターン(白)
    const string pattern = "BOOWOOWB";
    const auto test_bit = GetStateBit(pattern);
    const auto stone_bit = GetPlayerStoneBit<P>(test_bit);
    const auto open_bit = GetOpenPositionBit(test_bit);

    array<uint64_t, kPatternCount> pattern_search_bit_list{{0}};
    SearchNextTwo<P>(stone_bit, open_bit, &pattern_search_bit_list);

    for(size_t i=0; i<kPatternCount; i++){
      const auto search_bit = pattern_search_bit_list[i];
      ASSERT_EQ(0ULL, search_bit);
    }
  }
}

}   // namespace realcore


