#include "template.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void Lab2::addatbegin(std::string text) {

  Node* tmp = new Node();
  tmp->text = text;

  tmp->next = head;
  head = tmp;
  size = size + 1;
}

void Lab2::addatend(Node* head_ref, std::string text) {

  Node* tmp = new Node();
  tmp->text = text;

  if (size == 0)
  {
    addatbegin(text);
  }
  else {
    Node* cu = head;
    while (cu != nullptr)
    {
      if (cu->next == nullptr)
      {
        cu->next = tmp;
        size = size + 1;
        return;
      }
      cu = cu->next;
    }
  }
}


void Lab2::addatpos(Node* head_ref, int pos, std::string text) {

  Node* tmp = new Node();
  tmp->text = text;


  Node* cu = head;
  Node* prev = nullptr;
  int i = 0;
  while (i <= pos)
  {
    if (i == pos) {
      tmp->next = prev->next;
      prev->next = tmp;
      size = size + 1;
      return;
    }
    prev = cu;
    cu = cu->next;
    i++;
  }
}


// Given a reference (pointer to pointer)
void Lab2::deleted(Node* head_ref, std::string type, std::string text)
{
  // Store head node
  std::vector<Node*> vec;

  Node* temp = head_ref;
  Node* prev = NULL;
  
  int i = 0;
  while (temp != NULL)
  {
    if (i == 0 && temp->text == text) {
      vec.push_back(temp);
      head = temp->next;
      temp = temp->next;
    }
    else if (temp->text == text) {
      vec.push_back(temp);
      prev->next = temp->next;
      temp = temp->next;
      i++;
    } else {
      prev = temp;
      temp = temp->next;
      i++;
    }
  }

  for (size_t i = 0; i < vec.size(); i++)
  {
    delete vec[i];
    size = size - 1;
  }
  
  return;

}


void Lab2::print(std::string outputFile, std::string type) {

  // Write to output file
  std::ofstream ofs(outputFile);

  Node *cu = head;
  while (cu != nullptr)
  {
      ofs << cu->text << " ";
      cu = cu->next;
  }
}

void Lab2::print2(std::string type) {

  Node *cu = head;
  while (cu != nullptr)
  {
      cout << cu->text << " ";
      cu = cu->next;
  }
  cout << endl;
}