#include <iostream>
#include <string>

struct Node {
  std::string text;
  
  Node *next;
};

class Lab2 {
  private:
    Node *head;
 
  public:
    int size = 0;
    Lab2() { head = nullptr; }
    Node* gethead() { return head; };
    void addatbegin(std::string text);
    void addatend(Node* head_ref, std::string text);
    void addatpos(Node* head_ref, int pos, std::string text);
    void deleted(Node* head_ref, std::string type, std::string text);
    void print(std::string outputFile, std::string type);
    void print2(std::string type);
};