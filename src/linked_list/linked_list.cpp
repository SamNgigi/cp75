#include "linked_list/linked_list.hpp"
#include <stdexcept>

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

void LinkedList::reverse(){
  std::unique_ptr<Node> prev = nullptr;
  std::unique_ptr<Node> current = std::move(head);

  while(current){
    std::unique_ptr<Node> next = std::move(current->next);
    current->next = std::move(prev);
    prev = std::move(current);
    current = std::move(next);
  }
  head = std::move(prev);
}

void LinkedList::traverse(std::function<void(int)>func) const{
  Node* current = head.get();
  while(current){
    func(current->val);
    current = current->next.get();
  }
}

int LinkedList::find_middle(){
  if(!head){
    throw std::runtime_error("List is empty!");
  }
  Node *slow = head.get(), *fast = head.get();
  while(fast->next && fast->next->next){
    fast = fast->next->next.get();
    slow = slow->next.get();
  }
  return slow->val;
}

int LinkedList::pop_front(){
  if(!head){
    throw std::runtime_error("List is empty!");
  }

  int res = head->val;
  head = std::move(head->next);
  size--;
  return res;
}

int LinkedList::pop_back(){
  if(!head) throw std::runtime_error("List is empty");


  if(!head->next){
    int res = head->val;
    head = nullptr;
    size--;
    return res;
  }

  Node* current = head.get();
  while(current->next->next){
    current = current->next.get();
  }
  int res = current->next->val;
  current->next = nullptr;
  size--;
  return res;
}

void LinkedList::insert(size_t position, int val){
  if(position > size) throw std::runtime_error("Position is out of range");
  if(position == 0){
    push_front(val);
    return;
  }
  if(position == size){
    push_back(val);
    return;
  }
  Node *current = head.get();
  while(position-1){
    position--;
    current = current->next.get();
  }
  auto new_node = std::make_unique<Node>(val);
  new_node->next = std::move(current->next);
  current->next = std::move(new_node);
  size++;
}

void LinkedList::remove(int val){
  if(!head) throw std::runtime_error("List is empty");
  if(head->val == val){
    head = std::move(head->next);
    size--;
  }
  Node* current = head.get();
  while(current->next && current->next->val != val){
    current = current->next.get();
  }
  if(current->next){
    current->next = std::move(current->next->next);
    size--;
  }
  
}

// Additional useful methods
size_t LinkedList::get_size() const{
  return size;
}

bool LinkedList::isEmpty() const{
  return size == 0; 
}

void LinkedList::clear(){ head = nullptr; size = 0; }

bool LinkedList::contains(int val) const {
  if(!head) return false;
  if(head->val == val) return true;
  Node* current = head->next.get();
  while(current && current->val != val){
    current = current->next.get();
  }
  if(current) return true;
  return false;
}

std::vector<int> LinkedList::to_vector() const {
  std::vector<int> res = {};
  traverse([&res](int val){ res.push_back(val); });
  return res;
}

