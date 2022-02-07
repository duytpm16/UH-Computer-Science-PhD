/*
Author: Duy Pham - 2085253
Date: 02/01/2022
Course: COSC 2436 / 6306
Description: Recursion program using the input file and command file. Results are in the output file.
*/
#include "ArgumentManager.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool match_command(std::string command, std::string info);
bool read_inputFile(std::string inputFile, std::vector<std::string>& vec);
bool read_commandFile(std::string commandFile, std::string& command);
std::string recursion_func(int start, int end, int level, std::string command, std::vector<std::string> vec);

int main(int argc, char* argv[]) {
  
  // Read command line
  ArgumentManager am;

  am.parse(argc, argv, ';');
  if (am.get("input").length() == 0 || am.get("command").length() == 0 || am.get("output").length() == 0)
  {
    am.parse(argc, argv, ' ');
  }
  if (am.get("input").length() == 0 || am.get("command").length() == 0 || am.get("output").length() == 0)
  {
    am.parse(argc, argv, ',');
  }
  
  std::string inputFile   = am.get("input");
  std::string commandFile = am.get("command");
  std::string outputFile  = am.get("output");
  if (inputFile.length() == 0)
  {
    cout << "Error: input argument required." << endl;
    return 1;
  }
  if (commandFile.length() == 0)
  {
    cout << "Error: command argument required." << endl;
    return 1;
  }
  if (outputFile.length() == 0)
  {
    cout << "Error: output argument required." << endl;
    return 1;
  }

  // Read input file
  std::vector<std::string> input;
  if (!read_inputFile(inputFile, input))
  {
    return 1;
  }

  // Read command file
  std::string command;
  if (!read_commandFile(commandFile, command))
  {
    return 1;
  }

  // Recursion
  std::string results = recursion_func(0, input.size(), 0, command, input);

  // Write to output file
  std::ofstream ofs(outputFile);
  ofs << results;

  return 0;
}


// This function reads the input file
bool read_inputFile(std::string inputFile, std::vector<std::string>& vec)
{
  std::string line;

  std::ifstream ifs(inputFile);
  if (!ifs.is_open()) {
    std::cout << "Error opening file" << std::endl;
    return false;
  }

  getline(ifs, line);
  if (line.empty()){
    std::cout << "ERROR! Input file EMPTY!" << std::endl;
    return false;
  }
  ifs.clear();
  ifs.seekg(0);

  // Process the input file and store information into vector
  std::string str = "";
  while(getline(ifs, line))
  {
    size_t ll = line.length();

    for (size_t i = 0; i < ll; i++)
    {

      if (line[i] == ' ' || line[i] == '\n' || (line[i] == '\r' && i == (ll-1)))
      {
        continue;
      }
      else if (line[i] == '{')
      {
        str = std::string(1, std::tolower(line[i]));
      }
      else if (line[i] == '}')
      {
        str += std::string(1, std::tolower(line[i]));
        vec.push_back(str);
      }
      else if (line[i] == ',')
      {
        if (line[i-1] == '}')
        {
          continue;
        }
        else
        {
          str += std::string(1, std::tolower(line[i]));
        }
      }
      else
      {
        str += std::string(1, std::tolower(line[i]));
      }
    }
  }

  return true;
}


// This function reads the command file
bool read_commandFile(std::string commandFile, std::string& command)
{
  std::string line;

  std::ifstream ifs(commandFile);
  if (!ifs.is_open()) {
    std::cout << "Error opening command file";
    return false;
  }

  getline(ifs, line);
  if (line.empty()){
    std::cout << "ERROR! Input file EMPTY!" << std::endl;
    return false;
  }

  // Convert to lower case for matching
  size_t ll = line.length();
  for (size_t i = 0; i < ll; i++)
  {
    if (!(line[i] == '\r' && i == (ll-1)))
    {
      command += std::string(1, std::tolower(line[i]));
    }
  }
  
  return true;
}

// Match the command to the input
bool match_command(std::string command, std::string info)
{
  std::string str = "";
  for (size_t i = 1; i < info.length(); i++)
  {
    if (info[i] != ',' && info[i] != '}')
    {
      str += std::string(1, info[i]);
    }
    else 
    {
      if (str == command)
      {
        return true;
      }
      else
      {
        str = "";
      }
    }
  }

  return false;
}


// Recursive function
std::string recursion_func(int start, int end, int level, std::string command, std::vector<std::string> vec)
{
  std::string test = "";
  if ((start - (end-1)) == 0)
  {
    // Return level and information if matches command
    if (match_command(command, vec[start])) {
      return ("Level " + std::to_string(level) + ": " + vec[start] + "\n");
    }
    else {
      return "";
    }
  } else {
    // Recursion: split the vector in half 
    size_t mid = ceil((start + end) / 2.0);

    level++;
    test += recursion_func(start, mid, level, command, vec) + 
            recursion_func(mid, end, level, command, vec);
  }

  return test;
}