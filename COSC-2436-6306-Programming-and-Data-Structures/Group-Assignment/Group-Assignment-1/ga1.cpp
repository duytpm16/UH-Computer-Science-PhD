#include "ArgumentManager.h"
#include "bar.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::string decode(std::string line);
std::string verdict(Node* current, std::string guilty, std::string innocent);

int main(int argc, char* argv[]) {
  // Read command line
  ArgumentManager am;

  am.parse(argc, argv, ';');
  if (am.get("input").length() == 0 || am.get("output").length() == 0)
  {
    am.parse(argc, argv, ' ');
  }
  if (am.get("input").length() == 0 || am.get("output").length() == 0)
  {
    am.parse(argc, argv, ',');
  }

  std::string inputFile  = am.get("input");
  std::string outputFile = am.get("output");
  if (inputFile.length() == 0)
  {
    cout << "Error: input file required." << endl;
    return 1;
  }
  if (outputFile.length() == 0)
  {
    cout << "Error: output file required." << endl;
    return 1;
  }


  // Read Input file and add values to linked list
  std::string line = "";
  std::ifstream ifs(inputFile);
  if (!ifs.is_open()) {
    std::cout << "Error: opening input file." << std::endl;
    return 1;
  }

  Bar bar;
  
  bool inBar1 = false;
  bool inBar2 = false;
  std::vector<std::string> bar1;
  std::vector<std::string> bar2;
  while(getline(ifs, line)) {
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    if (line == "") { continue; }
   
    std::transform(line.begin(), line.end(), line.begin(), [](unsigned char c) { return std::tolower(c); });

    size_t barLine = line.rfind("bar", 0);
    if (barLine == 0) {
      if (line == "bar1") {
        inBar1 = true;
        inBar2 = false;
      }
      if (line == "bar2") {
        inBar2 = true;
        inBar1 = false;
      }
    }


    if (barLine != 0 && inBar1) {
      // Recursively decode the id and add to linked list
      std::string id = decode(line);
      bar.push(id, "bar1");
    }
    if (barLine != 0 && inBar2) {
      // Recursively decode the id and add to linked list
      std::string id = decode(line);
      bar.push(id, "bar2");
    }
  }

  // Sort recursively
  bar.insertionSort(bar.top());


  
  // Initialie stack
  Bar stack;
  stack.queue(&stack, bar.top());

  // Output the result
  std::ofstream ofs(outputFile);
  ofs << verdict(stack.top(), "", "");
  
  // Deque stack
  stack.deque();
  
  return 0;
}

std::string reverseString(string s, int start, int end) 
{
  if (end <= 0) {
    return std::string(1, s[start]);
  }
  
  return std::string(1, s[end]) + reverseString(s, start, end - 1);
}


std::string decode(std::string encode) 
{
    int start = 0;
    int end   = encode.length() - 1;
    std::string rev = "";
    for(int i = 0; i < encode.length(); i++){
        if(encode[i] == '(') {
          start = i;
        }
        if(encode[i] == ')'){
          end = i;
          std::string rev = encode.substr(start + 1, end - start - 1);
          return decode(encode.substr(0, start) + reverseString(rev, 0, end - start - 2) + encode.substr(end + 1));
        }
    }
    return encode;
}


std::string verdict(Node* current, std::string guilty, std::string innocent) 
{

  if (current == nullptr) {
    if (guilty.length() != 0 && innocent.length() != 0) {
      return ("Guilty: \n" + guilty + "Innocent: \n" + innocent);
    } else if (guilty.length() == 0 && innocent.length() > 0) {
      return ("Innocent: \n" + innocent);
    } else if (guilty.length() != 0 && innocent.length() == 0) {
      return ("Guilty: \n" + guilty);
    } else {
      return ("");
    }
  }

  // Starting from the next node
  int count = 0;

  if (current->next != nullptr) {
    Node *next = current->next;
    while (current->id == next->id && next != nullptr) {
      if (current->bar != next->bar) {
        count++;
      }
      next = next->next;
      
      if (next == nullptr) {
        break;
      }
    }

    if (count) {
      guilty = current->id + "\n" + guilty;
    } else {
      innocent = current->id + "\n" + innocent;
    }

    return verdict(next, guilty, innocent);
  } else {
    return verdict(current->next, guilty, current->id + "\n" + innocent);
  }

}