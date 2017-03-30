#include <algorithm>

#include "gtest/gtest.h"
#include "RealCore.h"

using namespace std;

namespace realcore{

TEST(RealCoreTest, GetBoardDirectionTest)
{
  // 全数テスト
  const auto &direction_list = GetBoardDirection();

  EXPECT_EQ(kBoardDirectionNum, direction_list.size());

  auto find_it = find(direction_list.begin(), direction_list.end(), kLateralDirection);
  EXPECT_TRUE(find_it != direction_list.end());

  find_it = find(direction_list.begin(), direction_list.end(), kVerticalDirection);
  EXPECT_TRUE(find_it != direction_list.end());

  find_it = find(direction_list.begin(), direction_list.end(), kLeftDiagonalDirection);
  EXPECT_TRUE(find_it != direction_list.end());
  
  find_it = find(direction_list.begin(), direction_list.end(), kRightDiagonalDirection);
  EXPECT_TRUE(find_it != direction_list.end());
}

TEST(RealCoreTest, GetBoardSymmetryTest)
{
  // 全数テスト
  const auto &symmetry_list = GetBoardSymmetry();

  EXPECT_EQ(kBoardSymmetryNum, symmetry_list.size());

  auto find_it = find(symmetry_list.begin(), symmetry_list.end(), kIdenticalSymmetry);
  EXPECT_TRUE(find_it != symmetry_list.end());

  find_it = find(symmetry_list.begin(), symmetry_list.end(), kHorizontalSymmetry);
  EXPECT_TRUE(find_it != symmetry_list.end());

  find_it = find(symmetry_list.begin(), symmetry_list.end(), kVerticalSymmetry);
  EXPECT_TRUE(find_it != symmetry_list.end());

  find_it = find(symmetry_list.begin(), symmetry_list.end(), kCentricSymmetry);
  EXPECT_TRUE(find_it != symmetry_list.end());

  find_it = find(symmetry_list.begin(), symmetry_list.end(), kDiagonalSymmetry1);
  EXPECT_TRUE(find_it != symmetry_list.end());

  find_it = find(symmetry_list.begin(), symmetry_list.end(), kDiagonalSymmetry2);
  EXPECT_TRUE(find_it != symmetry_list.end());

  find_it = find(symmetry_list.begin(), symmetry_list.end(), kDiagonalSymmetry3);
  EXPECT_TRUE(find_it != symmetry_list.end());

  find_it = find(symmetry_list.begin(), symmetry_list.end(), kDiagonalSymmetry4);
  EXPECT_TRUE(find_it != symmetry_list.end());
}
}

