/* Author: Duy Pham
   Id: 2085253
   Lab 3 programming assignment
   COSC 2436 / 6306
*/

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

  // Read Input file
  std::string line = "";
  std::ifstream ifs(inputFile);
  if (!ifs.is_open()) {
    std::cout << "Error: opening input file" << std::endl;
    return 1;
  }

  int i = 0;
  std::vector<double> dbl_queue;
  std::vector<std::string> str_queue;
  while(getline(ifs, line)) 
  {
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    
    if (line == "") { continue; }

    std::string value = "";
    std::vector<std::string> values;

    int size = 0;
    std::istringstream iss(line);
    while (getline(iss, value, ' ')) {
      values.push_back(value);
      values.push_back(" ");
      size += 2;
    }

    double time = std::stod(values[size-2]);
    
    std::string task = "";
    for (int j = 0; j < size - 2; j++) {
      task += values[j]; 
    }

    if (i == 0) {
      dbl_queue.push_back(time);
      str_queue.push_back(task);
      i++;
    }
    else {
      if (time < dbl_queue[0]) {
        dbl_queue.insert(dbl_queue.begin(), time);
        str_queue.insert(str_queue.begin(), task);
        i++;
      }
      else if (time > dbl_queue[i - 1]) {
        dbl_queue.push_back(time);
        str_queue.push_back(task);
        i++;
      } else {
        bool inserted = false;
        for (int j = 0; j < i; j++) {
          if (time < dbl_queue[j]) {
            dbl_queue.insert(dbl_queue.begin() + j, time);
            str_queue.insert(str_queue.begin() + j, task);
            inserted = true;
            break;
          }
        }

        if (!inserted) {
          dbl_queue.push_back(time);
          str_queue.push_back(task);      
        }
        i++;
      }
    }
  }


  // Write to output file
  std::ofstream ofs(outputFile);
  for (int j = 0; j < i; j++) {
    ofs << str_queue[j] + "\n";
  }

  return 0;
}