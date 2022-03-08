#include "bar.h"

#include <iostream>
#include <fstream>
#include <string>

// Add elements to top of link list
void Bar::push(std::string id, std::string bar) 
{
  Node* tmp = new Node();
  tmp->id   = id;
  tmp->bar  = bar;

  int i = 0;
  while (id[i] == '0') {
    i++;
  }
  tmp->num = id.erase(0, i);
  
  tmp->next = head;
  head = tmp;
  size++;
}

// Initialize stack
void Bar::queue(Bar* cls, Node* current) 
{
  if (current == NULL) {
    return;
  }
 
  cls->push(current->id, current->bar);
  queue(cls, current->next);
}

// Remove head of a linked list
void Bar::pop()
{
  Node* tmp = head;
  head = head->next;
  delete tmp;
}

// Delete all element of linked list
void Bar::deque() 
{
  if (head == NULL) {
    return;
  }

  pop();
  deque();
}


// Recursively sort a singly linked list using insertion
void Bar::insertionSort(Node* current)
{
  if (current == NULL) {
    head = sorted;
    return;
  }

  Node* next = current->next;
  sortedInsert(current);
  insertionSort(next);
} 

// Return true or false depending on whether str1 is greater or less than str2
bool greater(std::string str1, std::string str2) {
  size_t len1 = str1.length();
  size_t len2 = str2.length(); 

  if (len1 > len2) {
    return true;
  }
  if (len1 < len2) {
    return false;
  }
  
  if (len1 == len2) {
    if (str1.compare(str2) == 0) {
      return false;
    }

    for (size_t i = 0; i < len1; i++) {
      if ((str1[i] - '0') > (str2[i] - '0')) {
        return true;
      } else if ((str1[i] - '0') < (str2[i] - '0')) {
        return false;
      }
    }
  }
  return false;
}

void Bar::sortedInsert(Node* newnode)
{
  if (sorted == NULL || greater(sorted->num, newnode->num)) {
    newnode->next = sorted;
    sorted = newnode;
  }
  else {
    Node* current = sorted;
    //Locate the node before the point of insertion 
    while (current->next != NULL && !greater(current->next->num, newnode->num)) {
      current = current->next;
    }
    newnode->next = current->next;
    current->next = newnode;
  }
}

void Bar::print() 
{
  Node* current = head;
  while(current != NULL) {
    std::cout << current->num << std::endl;
    current = current->next;
  }
}
