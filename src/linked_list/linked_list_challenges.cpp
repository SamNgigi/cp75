#include <vector>
#include <algorithm>

#include "linked_list/linked_list_challenges.hpp"

void LinkedListChallenges::populate(std::vector<int> values){
  clear(); // Clear linked_list in case it had any values
  for(int i : values) push_back(i);
}

int LinkedListChallenges::med_maxTwinSum(){
  if(!head || this->size % 2 != 0) return 0;
  /**
    * Below implementation uses unique_ptr features such as releas and reset to be able
    * to reverse a linked_list without deleting nodes
    * 
    * 1. We iterate through the linked_lst while reversing the first half until we get to the
    * middle node
    * 2. We then iterate through the second half of the linked_list while adding the values
    * 3. We finally restore the linked list to its original state because we modified the
    * list while reversing 
  */
  Node* slow = head.get(), *fast = head.get(), *prev = nullptr;
  // Getting to middle node & reversing first half
  while(fast && fast->next){

    fast = fast->next->next.get();

    Node *temp = slow;

    slow = slow->next.get();

    // Below code basically does the following
    // temp->next = prev
    // It does so by first releasing whatever temp->next pointed to
    // Then re-assigning value to prev.
    // Makes code a bit more complex but ensures better memory management.
    [[maybe_unused]] auto unused = temp->next.release();
    temp->next.reset(prev);
    prev = temp;
  }

  int maxSum = 0;
  Node* firstHalf = prev;
  Node* secondHalf = slow;
  while(firstHalf && secondHalf){
    maxSum = std::max(maxSum, (firstHalf->val + secondHalf->val));
    firstHalf = firstHalf->next.get();
    secondHalf = secondHalf->next.get();
  }

  Node* restored = nullptr;
  while(prev->next){
    Node *temp = prev->next.get();

    // Below is equivalent to prev->next = restored;
    [[maybe_unused]] auto unused = prev->next.release();
    prev->next.reset(restored);

    restored = prev;
    prev = temp;
  }

  head.reset(restored);

  return maxSum;

}

