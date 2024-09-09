#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "linked_list/linked_list_challenges.hpp"


class LinkedListChallengesTest: public testing::Test{
protected:
  LinkedListChallengesTest(){
    llc1.populate({5,4,2,1});
    llc2.populate({4,2,2,3});
    llc3.populate({1, 100000});
  }
  LinkedListChallenges llc1;
  LinkedListChallenges llc2;
  LinkedListChallenges llc3;
};

TEST_F(LinkedListChallengesTest, TestPopulate){
  std::vector<int> expected1 = {5,4,2,1};
  std::vector<int> expected2 = {4,2,2,3};
  std::vector<int> expected3 = {1,100000};
  EXPECT_THAT(llc1.to_vector(), expected1);
  EXPECT_THAT(llc2.to_vector(), expected2);
  EXPECT_THAT(llc3.to_vector(), expected3);
}

TEST_F(LinkedListChallengesTest, TestMaxTwinSum){
  EXPECT_EQ(llc1.med_maxTwinSum(), 6);
  EXPECT_EQ(llc2.med_maxTwinSum(), 7);
  EXPECT_EQ(llc3.med_maxTwinSum(), 100001);
}






