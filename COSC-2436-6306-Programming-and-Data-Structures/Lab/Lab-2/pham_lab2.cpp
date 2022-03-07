// Author: Duy Pham
// ID: 2085253
// Lab2

#include "template.h"
#include "ArgumentManager.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

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


  Lab2 lab2;


  // Read Input file and add values to linked list
  std::string line = "";
  std::string type = "";
  std::ifstream ifs(inputFile);
  if (!ifs.is_open()) {
    std::cout << "Error: opening input file" << std::endl;
    return 1;
  }

  int i = 0;
  while(getline(ifs, line)) {
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    if (line == "") { continue; }
    
    std::istringstream iss(line);
    if (i == 0) {
        iss >> type;
        std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c) { return std::tolower(c); });

        if (type != "char" && type != "string" && type != "int") {
            cout << "Error: the first line in input file is not char, string, or int" << endl;
            return 1;
        }
    }
    else {
        std::string command; 
        std::string index;
        std::string value;

        iss >> command;
        std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c) { return std::tolower(c); });

        if (command == "add") {
          iss >> index >> value;
          index.erase(0,1);
          index.pop_back();
          value.erase(0,1);
          value.pop_back();

          int pos = std::stoi(index);
          if (pos == 0) {
            lab2.addatbegin(value);
          }
          else if (pos == lab2.size) {
            lab2.addatend(lab2.gethead(), value);
          } 
          else if (pos > 0 && pos < lab2.size) {
            lab2.addatpos(lab2.gethead(), pos, value);
          } 
          else {
            continue;
          }
        } 
        
        if (command == "remove") {
          iss >> value;
          value.erase(0,1);
          value.pop_back();

          if (lab2.size > 0) {
            lab2.deleted(lab2.gethead(), type, value);
          }
        }

    }
    i++;
  }

  lab2.print(outputFile, type);

  return 0;
}
