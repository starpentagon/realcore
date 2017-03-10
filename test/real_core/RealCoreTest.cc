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
}
