#include "sliding_window/sliding_window.hpp"

#include <gtest/gtest.h>
#include <vector>

TEST(SlidingWindowTests, TestLongestOnes){
  std::vector<int> a = {1,1,1,0,0,0,1,1,1,1,0};
  std::vector<int> b = {0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1};

  EXPECT_EQ(longestOnes(a, 2), 6);
  EXPECT_EQ(longestOnes(b, 3), 10);
}
