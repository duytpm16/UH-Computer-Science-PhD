#include <iostream>

struct Node{
  std::string id;
  std::string bar;
  std::string num;
  
  Node* next;
};


class Bar {
  private:
    Node* head;
    Node* sorted = NULL;
  
  public:
    int size = 0;
    Bar() { head = nullptr; }
    Node* top() { return head; };
    void push(std::string id, std::string bar);
    void pop();
    void queue(Bar* cls, Node* current);
    void deque();
    void insertionSort(Node* current);
    void sortedInsert(Node* newnode);
    std::string verdict(Node* current, std::string guilty, std::string innocent);
    void print();
};