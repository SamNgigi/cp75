#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "linked_list/linked_list.hpp"

 //

class LinkedListTests : public testing::Test {

protected:
    LinkedListTests(){
    
      lst1.push_front(0);
      lst1.push_back(1);
      lst1.push_back(2);
      
      lst2.push_front(4);
      lst2.push_front(3);
    }

    LinkedList lst1;
    LinkedList lst2;

};

TEST_F(LinkedListTests, TestPushAndTraverse){

  std::vector<int> expected = {0,1,2};
  EXPECT_THAT(lst1.to_vector(), expected);
  EXPECT_EQ(lst1.get_size(), expected.size());

}
 
TEST_F(LinkedListTests, TestReverse){
  lst1.reverse();
  std::vector<int> expected = {2,1,0};
  EXPECT_THAT(lst1.to_vector(), expected);
}

TEST_F(LinkedListTests, TestFindMiddle){
  EXPECT_EQ(lst1.find_middle(), 1);
  lst1.push_back(3);
  EXPECT_EQ(lst1.find_middle(), 1);
}

TEST_F(LinkedListTests, TestPopFrontAndBack){
  EXPECT_EQ(lst1.pop_front(), 0);
  EXPECT_EQ(lst1.pop_back(), 2);
  EXPECT_EQ(lst1.get_size(), 1);
}

TEST_F(LinkedListTests, TestInsert){
  lst1.insert(1,3);
  std::vector<int> expected = {0,3,1,2};
  EXPECT_THAT(lst1.to_vector(), expected);

}

TEST_F(LinkedListTests, TestRemove){
  lst1.remove(0);
  std::vector<int> expected = {1,2};
  EXPECT_THAT(lst1.to_vector(), expected);
  EXPECT_EQ(lst1.get_size(), 2);
}

TEST_F(LinkedListTests, TestIsEmpty){
  EXPECT_FALSE(lst1.isEmpty());
}

TEST_F(LinkedListTests, TestClear){
  lst1.clear();
  EXPECT_TRUE(lst1.isEmpty());
}

TEST_F(LinkedListTests, TestContains){
  EXPECT_TRUE(lst1.contains(0));
  EXPECT_TRUE(lst1.contains(2));
  EXPECT_FALSE(lst1.contains(3));
}

TEST_F(LinkedListTests, TestSortV1){
  lst1.insert(1,3);
  std::vector<int> unsorted = {0,3,1,2};
  EXPECT_THAT(lst1.to_vector(), unsorted);
  lst1.sortv1();
  std::vector<int> sorted = {0,1,2,3};
  EXPECT_THAT(lst1.to_vector() , sorted);
}

TEST_F(LinkedListTests, TestMergeLinkedLists){
  lst1.merge(lst2);
  std::vector<int> expected = {0,1,2,3,4};
  EXPECT_THAT(lst1.to_vector(), expected);
  EXPECT_EQ(lst1.get_size(), expected.size());
}




