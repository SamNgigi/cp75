#include "linked_list/linked_list.hpp"

void LinkedList::push_front(int value){
  auto new_node = std::make_unique<Node>(value);
  new_node->next = std::move(head);
  head = std::move(new_node);
  size++;
}

void LinkedList::push_back(int value){
  std::unique_ptr<Node> new_node = std::make_unique<Node>(value);
  if(!head){
    head = std::move(new_node);
  } else {
    Node* current = head.get();
    while(current->next){
      current = current->next.get();
    }
    current->next = std::move(new_node); 
  }
  size++;
}

void LinkedList::traverse(std::function<void(int)>func) const{
  Node* current = head.get();
  while(current){
    func(current->val);
    current = current->next.get();
  }
}

// Additional useful methods
size_t LinkedList::get_size() const{
  return size;
}

std::vector<int> LinkedList::to_vector() const {
  std::vector<int> res = {};
  traverse([&res](int val){ res.push_back(val); });
  return res;
}
