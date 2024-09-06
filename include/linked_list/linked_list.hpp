#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <memory>
#include <vector>
#include <functional>


class LinkedList{
  private:
  struct Node{
    int val;
    std::unique_ptr<Node> next;
    Node(): val(0), next(nullptr) {} 
    Node(int val): val(val), next(nullptr) {} 
    Node(int val, std::unique_ptr<Node> next): val(val), next(std::move(next)){}
  };

  std::unique_ptr<Node> head;
  size_t size;

public:
  LinkedList(): head(nullptr), size(0) {}
  ~LinkedList() = default;

  // Adding new Linked List Node
  void push_front(int value);
  void push_back(int value);

  // Traversing/Iterating through the Linked List
  void traverse(std::function<void(int)> func) const; 
  
  // Reversing a Linked List
  void reverse();

  // Finding middle element
  int find_middle();
  
  // Removing first node/element in a linked list;
  int pop_front();

  // Removing last node/element in a linked list;
  int pop_back();

  // Insert node at x with value y
  void insert(size_t position, int val);

  // Removing a node with a provided value;
  void remove(int val);


  // Additional Useful Methods
  size_t get_size() const;
  // Checking whether a linked list is empty
  bool isEmpty() const;
  // Clearing/Emptying the linked list;
  void clear();
  // Checking if linked list has node with val provided
  bool contains(int val) const;

  std::vector<int> to_vector() const;

};


#endif // LINKED_LIST_HPP
