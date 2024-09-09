#ifndef LINKED_LIST_CHALLENGES_HPP
#define LINKED_LIST_CHALLENGES_HPP

#include <vector>

#include "linked_list.hpp"

class LinkedListChallenges : public LinkedList {
public:
  void populate(std::vector<int> values);
  int med_maxTwinSum();
};

#endif // LINKED_LIST_CHALLENGES_HPP
