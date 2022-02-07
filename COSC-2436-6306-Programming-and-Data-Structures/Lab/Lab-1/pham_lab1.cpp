#include "ArgumentManager.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool recursion_func(int pos, int x, int y, std::string target, std::vector<std::vector<char>> board, std::vector<bool> visited);

int main(int argc, char* argv[]) {
  
  // Read command line
  ArgumentManager am;

  am.parse(argc, argv, ';');
  if (am.get("input").length() == 0 || am.get("output").length() == 0)
  {
    am.parse(argc, argv, ' ');
  }
  if (am.get("input").length() == 0|| am.get("output").length() == 0)
  {
    am.parse(argc, argv, ',');
  }
  
  std::string inputFile   = am.get("input");
  std::string outputFile  = am.get("output");
  if (inputFile.length() == 0)
  {
    cout << "Error: input argument required." << endl;
    return 1;
  }
  if (outputFile.length() == 0)
  {
    cout << "Error: output argument required." << endl;
    return 1;
  }


  // Read input file
  ifstream input(inputFile);
  ofstream output(outputFile);
  
  int row; //integer m, # of row in the matrix
  int col; //integer n, # of column in the matrix
  char value;
  std::string target; //the string to be search in the matrix
  vector<vector<char>> board; //this 2d vector will be the character matrix

  // Read input file
  input >> row;
  input >> col;
  for(int i=0; i<row; i++) {
    vector<char> temp;
    for(int j=0; j<col; j++) {
        input >> value;
        temp.push_back(value);
    }
    board.push_back(temp);
  }
  input >> target;



  // Recursion
  std::string result = "false";
  if (board.size() != 0 && target.length() != 0)
  {
    std::vector<bool> visited(row * col);
    for(int i=0; i<row; i++) {
      for(int j=0; j<col; j++) {
        if (recursion_func(0, i, j, target, board, visited)){
          result = "true";
          break;
        }
      }
    }
  }

  // Write to output file
  std::ofstream ofs(outputFile);
  ofs << result;

  return 0;
}



// Recursive function
bool recursion_func(int pos, int x, int y, std::string target, std::vector<std::vector<char>> board, std::vector<bool> visited)
{
  // If pos is the length of target then return true;
  if (pos == target.length()) {
    return true;
  }

  // Check for conditions 
  if (x < 0 || y < 0 || x >= board.size() || y >= board[0].size() || board[x][y] != target[pos])
  {
    return false;
  }

  visited[x * board.size() + y] = true;

  // Recursion if character matches character of target at position
  if (recursion_func(pos + 1, x + 1, y, target, board, visited) ||
      recursion_func(pos + 1, x - 1, y, target, board, visited) ||
      recursion_func(pos + 1, x, y + 1, target, board, visited) ||
      recursion_func(pos + 1, x, y - 1, target, board, visited))
  {
    return true;
  }

  visited[x * board.size() + y] = false;

  return false;
}