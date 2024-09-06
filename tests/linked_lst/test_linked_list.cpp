#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "linked_list/linked_list.hpp"

  TEST(LinkedListTests, TestPushAndTraverse){
    LinkedList lst;
    lst.push_front(0);
    lst.push_back(1);
    lst.push_back(2);

    std::vector<int> expected = {0,1,2};
    ASSERT_THAT(lst.to_vector(), expected);
    ASSERT_EQ(lst.get_size(), expected.size());

  }
