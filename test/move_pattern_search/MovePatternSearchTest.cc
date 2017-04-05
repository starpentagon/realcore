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
          ASSERT_EQ(0, search_bit);
          ASSERT_EQ(0, open_state_bit);
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
          ASSERT_EQ(0, search_bit);
          ASSERT_EQ(0, open_state_bit);
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

        ASSERT_EQ(0, search_bit);
        ASSERT_EQ(0, open_state_bit);
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

      constexpr uint64_t expect_bit = 0b1 << 0;
      EXPECT_EQ(expect_bit, search_bit);
    }
    {
      // 達四パターン(黒番)
      const StateBit test_bit = GetStateBit("OBBBBOW");
      
      const uint64_t black_bit = GetBlackStoneBit(test_bit);
      const uint64_t open_bit = GetOpenPositionBit(test_bit);
      const uint64_t search_bit = SearchOpenFour<kBlackTurn>(black_bit, open_bit);

      constexpr uint64_t expect_bit = 0b1 << 2;
      EXPECT_EQ(expect_bit, search_bit);      
    }
    {
      // 達四パターン(黒番)
      const StateBit test_bit = GetStateBit("OBBBBO");
      
      const uint64_t black_bit = GetBlackStoneBit(test_bit);
      const uint64_t open_bit = GetOpenPositionBit(test_bit);
      const uint64_t search_bit = SearchOpenFour<kBlackTurn>(black_bit, open_bit);

      constexpr uint64_t expect_bit = 0b1 << 0;
      EXPECT_EQ(expect_bit, search_bit);      
    }
    {
      // 達四パターン(黒番)
      const StateBit test_bit = GetStateBit("BWOBBBBOXB");
      
      const uint64_t black_bit = GetBlackStoneBit(test_bit);
      const uint64_t open_bit = GetOpenPositionBit(test_bit);
      const uint64_t search_bit = SearchOpenFour<kBlackTurn>(black_bit, open_bit);

      constexpr uint64_t expect_bit = 0b1 << 4;
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

      constexpr uint64_t expect_bit = 0b1 << 4;
      EXPECT_EQ(expect_bit, search_bit);      
    }
    {
      // 達四パターン(白番, 長連筋)
      const StateBit test_bit = GetStateBit("BWOWWWWOWB");
      
      const uint64_t white_bit = GetWhiteStoneBit(test_bit);
      const uint64_t open_bit = GetOpenPositionBit(test_bit);
      const uint64_t search_bit = SearchOpenFour<kWhiteTurn>(white_bit, open_bit);

      constexpr uint64_t expect_bit = 0b1 << 4;
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
          ASSERT_EQ(0, search_bit);
          ASSERT_EQ(0, open_state_bit);
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
          ASSERT_EQ(0, search_bit);
          ASSERT_EQ(0, open_state_bit);
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
          ASSERT_EQ(0, search_bit);
          ASSERT_EQ(0, open_state_bit);
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

        ASSERT_EQ(0, search_bit);
        ASSERT_EQ(0, open_state_bit);
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

        ASSERT_EQ(0, search_bit);
        ASSERT_EQ(0, open_state_bit);
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
          ASSERT_EQ(0, search_bit);
          ASSERT_EQ(0, open_state_bit);
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
          ASSERT_EQ(0, search_bit);
          ASSERT_EQ(0, open_state_bit);
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
          ASSERT_EQ(0, search_bit);
          ASSERT_EQ(0, open_state_bit);
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

        ASSERT_EQ(0, search_bit);
        ASSERT_EQ(0, open_state_bit);
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

  TEST(MovePatternSearchTest, SearchSemiThreeTest)
  {
    {
      // 三パターン(黒番)
      const StateBit test_bit = GetStateBit("OOBBBOO");
      
      const uint64_t black_bit = GetBlackStoneBit(test_bit);
      const uint64_t open_bit = GetOpenPositionBit(test_bit);
      uint64_t next_open_four_bit = 0;
      const uint64_t search_bit = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &next_open_four_bit);

      constexpr uint64_t expect_bit = (0b1 << 0) | (0b1 << 2);
      EXPECT_EQ(expect_bit, search_bit);

      constexpr uint64_t expect_open_four_bit = (0b1 << 2) | (0b1 << 10);
      EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
    }
    {
      // 三パターン(黒番)
      const StateBit test_bit = GetStateBit("OOBBOBO");
      
      const uint64_t black_bit = GetBlackStoneBit(test_bit);
      const uint64_t open_bit = GetOpenPositionBit(test_bit);
      uint64_t next_open_four_bit = 0;
      const uint64_t search_bit = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &next_open_four_bit);

      constexpr uint64_t expect_bit = (0b1 << 0);
      EXPECT_EQ(expect_bit, search_bit);

      constexpr uint64_t expect_open_four_bit = (0b1 << 4);
      EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
    }
    {
      // 三パターン(黒番)
      const StateBit test_bit = GetStateBit("OOBOBBO");
      
      const uint64_t black_bit = GetBlackStoneBit(test_bit);
      const uint64_t open_bit = GetOpenPositionBit(test_bit);
      uint64_t next_open_four_bit = 0;
      const uint64_t search_bit = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &next_open_four_bit);

      constexpr uint64_t expect_bit = (0b1 << 0);
      EXPECT_EQ(expect_bit, search_bit);

      constexpr uint64_t expect_open_four_bit = (0b1 << 6);
      EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
    }
    {
      // 三パターン(黒番)
      const StateBit test_bit = GetStateBit("OOOBBBO");
      
      const uint64_t black_bit = GetBlackStoneBit(test_bit);
      const uint64_t open_bit = GetOpenPositionBit(test_bit);
      uint64_t next_open_four_bit = 0;
      const uint64_t search_bit = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &next_open_four_bit);

      constexpr uint64_t expect_bit = (0b1 << 0);
      EXPECT_EQ(expect_bit, search_bit);

      constexpr uint64_t expect_open_four_bit = (0b1 << 8);
      EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
    }
    {
      // 三パターン(黒番, 長連筋)
      const StateBit test_bit = GetStateBit("BOOBBBOO");
      
      const uint64_t black_bit = GetBlackStoneBit(test_bit);
      const uint64_t open_bit = GetOpenPositionBit(test_bit);
      uint64_t next_open_four_bit = 0;
      const uint64_t search_bit = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &next_open_four_bit);

      constexpr uint64_t expect_bit = (0b1 << 0);
      EXPECT_EQ(expect_bit, search_bit);

      constexpr uint64_t expect_open_four_bit = (0b1 << 2);
      EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
    }
    {
      // 三パターン(黒番, 長連筋)
      const StateBit test_bit = GetStateBit("OOBBBOOB");
      
      const uint64_t black_bit = GetBlackStoneBit(test_bit);
      const uint64_t open_bit = GetOpenPositionBit(test_bit);
      uint64_t next_open_four_bit = 0;
      const uint64_t search_bit = SearchSemiThree<kBlackTurn>(black_bit, open_bit, &next_open_four_bit);

      constexpr uint64_t expect_bit = (0b1 << 4);
      EXPECT_EQ(expect_bit, search_bit);

      constexpr uint64_t expect_open_four_bit = (0b1 << 12);
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

      constexpr uint64_t expect_bit = (0b1 << 0) | (0b1 << 2);
      EXPECT_EQ(expect_bit, search_bit);

      constexpr uint64_t expect_open_four_bit = (0b1 << 2) | (0b1 << 10);
      EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
    }
    {
      // 三パターン(白番)
      const StateBit test_bit = GetStateBit("OOWWOWO");
      
      const uint64_t white_bit = GetWhiteStoneBit(test_bit);
      const uint64_t open_bit = GetOpenPositionBit(test_bit);
      uint64_t next_open_four_bit = 0;
      const uint64_t search_bit = SearchSemiThree<kWhiteTurn>(white_bit, open_bit, &next_open_four_bit);

      constexpr uint64_t expect_bit = (0b1 << 0);
      EXPECT_EQ(expect_bit, search_bit);

      constexpr uint64_t expect_open_four_bit = (0b1 << 4);
      EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
    }
    {
      // 三パターン(白番)
      const StateBit test_bit = GetStateBit("OOWOWWO");
      
      const uint64_t white_bit = GetWhiteStoneBit(test_bit);
      const uint64_t open_bit = GetOpenPositionBit(test_bit);
      uint64_t next_open_four_bit = 0;
      const uint64_t search_bit = SearchSemiThree<kWhiteTurn>(white_bit, open_bit, &next_open_four_bit);

      constexpr uint64_t expect_bit = (0b1 << 0);
      EXPECT_EQ(expect_bit, search_bit);

      constexpr uint64_t expect_open_four_bit = (0b1 << 6);
      EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
    }
    {
      // 三パターン(白番)
      const StateBit test_bit = GetStateBit("OOOWWWO");
      
      const uint64_t white_bit = GetWhiteStoneBit(test_bit);
      const uint64_t open_bit = GetOpenPositionBit(test_bit);
      uint64_t next_open_four_bit = 0;
      const uint64_t search_bit = SearchSemiThree<kWhiteTurn>(white_bit, open_bit, &next_open_four_bit);

      constexpr uint64_t expect_bit = (0b1 << 0);
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

      constexpr uint64_t expect_bit = (0b1 << 0);
      EXPECT_EQ(expect_bit, search_bit);

      constexpr uint64_t expect_open_four_bit = (0b1 << 8);
      EXPECT_EQ(expect_open_four_bit, next_open_four_bit);
    }
    {
      // 三パターン(白番, 長連筋)
      const StateBit test_bit = GetStateBit("WOOWWWOOW");
      
      const uint64_t white_bit = GetWhiteStoneBit(test_bit);
      const uint64_t open_bit = GetOpenPositionBit(test_bit);
      uint64_t next_open_four_bit = 0;
      const uint64_t search_bit = SearchSemiThree<kWhiteTurn>(white_bit, open_bit, &next_open_four_bit);

      constexpr uint64_t expect_bit = (0b1 << 2) | (0b1 << 4);
      EXPECT_EQ(expect_bit, search_bit);

      constexpr uint64_t expect_open_four_bit = (0b1 << 4) | (0b1 << 12);
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
}   // namespace realcore


