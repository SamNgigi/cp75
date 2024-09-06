#include <iostream>
#include <vector>

#include "linked_list/linked_list.hpp"

int main(){

  std::cout << "Hello World\n";
  std::vector<int> a = {1,2,3,4};

  std::cout << "lst1.push_front\n";
  LinkedList lst1;
  for(int i : a) lst1.push_front(i);
  lst1.traverse([](int val){std::cout << val << " ";});
  std::cout << "\n";


  std::cout << "lst2.push_back\n";
  LinkedList lst2;
  for(int i: a) lst2.push_back(i);
  lst2.traverse([](int val){std::cout << val << " ";});
  std::cout << "\n";
  
  return 0;
}
