#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "linked_list/linked_list.hpp"

 //

class LinkedListTests : public testing::Test {

protected:
    LinkedListTests(){
    
      lst.push_front(0);
      lst.push_back(1);
      lst.push_back(2);

    }

    LinkedList lst;

};

TEST_F(LinkedListTests, TestPushAndTraverse){

  std::vector<int> expected = {0,1,2};
  EXPECT_THAT(lst.to_vector(), expected);
  EXPECT_EQ(lst.get_size(), expected.size());

}
 
TEST_F(LinkedListTests, TestReverse){
  lst.reverse();
  std::vector<int> expected = {2,1,0};
  EXPECT_THAT(lst.to_vector(), expected);
}

TEST_F(LinkedListTests, TestFindMiddle){
  EXPECT_EQ(lst.find_middle(), 1);
  lst.push_back(3);
  EXPECT_EQ(lst.find_middle(), 1);
}

TEST_F(LinkedListTests, TestPopFrontAndBack){
  EXPECT_EQ(lst.pop_front(), 0);
  EXPECT_EQ(lst.pop_back(), 2);
  EXPECT_EQ(lst.get_size(), 1);
}

TEST_F(LinkedListTests, TestInsert){
  lst.insert(1,3);
  std::vector<int> expected = {0,3,1,2};
  EXPECT_THAT(lst.to_vector(), expected);

}

TEST_F(LinkedListTests, TestRemove){
  lst.remove(0);
  std::vector<int> expected = {1,2};
  EXPECT_THAT(lst.to_vector(), expected);
  EXPECT_EQ(lst.get_size(), 2);
}

TEST_F(LinkedListTests, TestIsEmpty){
  EXPECT_FALSE(lst.isEmpty());
}

TEST_F(LinkedListTests, TestClear){
  lst.clear();
  EXPECT_TRUE(lst.isEmpty());
}

TEST_F(LinkedListTests, TestContains){
  EXPECT_TRUE(lst.contains(0));
  EXPECT_TRUE(lst.contains(2));
  EXPECT_FALSE(lst.contains(3));
}

