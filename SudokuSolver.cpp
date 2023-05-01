/*
Author: Raisa Methila
Project 6: Sudoko
Date: April 25th 2023
CSCI 235 */

#include "SudokuSolver.hpp"

/* the default constructor initializes an emtpy board, that is, a board filled with zeros
(an array of 9 arrays, each filled with 9 zeros). This board will be marked unsolvable. */
SudokuSolver::SudokuSolver()
{
   array_ = new int *[9];
   for (int num = 0; num < 9; num++)
   {
      array_[num] = new int[9];
   }
   for (int i = 0; i < 9; i++)
   {
      for (int j = 0; j < 9; j++)
      {
         array_[i][j] = 0;
      }
   }
   solvable_ = false;
}

/// @brief reads the csv file
/// @param input_file name of the file to be read
void SudokuSolver::readFile(std::string input_file)
{
   array_ = new int *[9];
   int i = 0;

   for (int num = 0; num < 9; num++)
   {
      array_[num] = new int[9];
   }

   std::ifstream file(input_file);
   std::string line;
   if (file.is_open())
   {
      while (std::getline(file, line))
      {
         int j = 0;
         std::istringstream ss(line);
         std::string number;
         while (std::getline(ss, number, ','))
         { // process each comma-separated value then store it in a Sudoku board

            array_[i][j] = stoi(number);
            j++;
         }
         i++;
      }
      file.close();
   }
}

/// @brief checks if the column of the array has the numeber
/// @param col referes to column of the function
/// @param num refers to the number that the function will check
/// @return returns true if the number is found, false other wise
bool SudokuSolver::checkCol(Sudoku::Location col, int num) // is the parameter type correct?
{
   for (int row = 0; row < 9; row++)
      if (array_[row][col.col] == num)
         return true;
   return false;
}

/// @brief checks if the row of the array has the numeber
/// @param row referes to row of the function
/// @param num refers to the number that the function will check
/// @return returns true if the number is found, false other wise
bool SudokuSolver::checkRow(Sudoku::Location row, int num)
{ // what if Location row, col
   for (int col = 0; col < 9; col++)
      if (array_[row.row][col] == num)
         return true;
   return false;
}

/// @brief checks if the 3 by 3 box have the number inputed
/// @param b_row start of the 3 by 3 row in the 9 by 9 array
/// @param b_col start of the 3 by 3 column in the 9 by 9 array
/// @param num refers to the number that the function will check
/// @return returns true if the number is found, false other wise
bool SudokuSolver::checkBox(int b_row, int b_col, int num)
{
   for (int row = 0; row < 3; row++)
      for (int col = 0; col < 3; col++)
         if (array_[b_row + row][b_col + col] == num)
            return true;
   return false;
}

/// @brief returns true if the value parameter (which can take on values 1-9) can legally be placed at
// the given location in the 2D array without breaking the rules of sudoku. It returns false if placing
// value at location is not a legal move. Remember, in order to be a valid move, the value must be unique
// with a row, column, and sub-grid.
/// @param value
/// @param location
/// @return returns true if the value parameter (which can take on values 1-9) can legally be placed at
// the given location in the 2D array without breaking the rules of sudoku.It returns false if placing
// value at location is not a legal move.
bool SudokuSolver::checkLegalValue(int value, Sudoku::Location location)
{
   return !checkRow(location, value) && !checkCol(location, value) && !checkBox(location.row - location.row % 3, location.col - location.col % 3, value);
}

/// @brief  It is an empty square in the sudoku board. To find the first
// empty square you must read the 2D array from top-left (0,0) to bottom right (8,8), one row at
// a time. Set the row and col of the Location to the indices at which the first 0 is found in the
// 2D array. If there are no empty squares the Location object should have row and col values of -1.
/// @return  returns a Location object with the coordinates of the first 0
// found in the 2D pointer array, meaning that it is an empty square in the sudoku board.
// To find the first empty square you must read the 2D array from top-left (0,0) to bottom right (8,8),
// one row at a time. Set the row and col of the Location to the indices at which the first 0 is found
// in the 2D array. If there are no empty squares the Location object should have row and col values of -1.
Sudoku::Location SudokuSolver::returnNextEmpty()
{
   Sudoku::Location loc;
   for (int row = 0; row < 9; row++)
   {
      for (int col = 0; col < 9; col++)
      {
         if (array_[row][col] == 0)
         {
            loc = {row, col};
            return loc;
         }
      }
   }
   loc = {-1, -1};
   return loc;
}

/// @brief Solves the sudoku puzzel by using return next empty() to find the zeros, then uses checkLegal
// value to see if the value imputed is a valid number, calls solve recursively to solve other boxes
/// @return returns true when solved, false other wise
bool SudokuSolver::solve()
{
   Sudoku::Location location = returnNextEmpty();
   if (location.row == -1 && location.col == -1)
      return true; // when solved, return true
   for (int value = 1; value < 10; value++)
   { // valid numbers are 1 - 9
      if (checkLegalValue(value, location))
      { // check validation, if yes, put the number in the array
         array_[location.row][location.col] = value;
         if (solve()) // recursively calls the function for other rooms in the array
            return true;
         array_[location.row][location.col] = 0; // turn to unassigned space when conditions are not satisfied
      }
   }
   return false;
}

/*the parameterized constructor reads the input file to initialize the board.
The input files consist of 9 digits on each row separated by commas, and 9 rows separated
by new line characters \n, corresponding to a sudoku board. Empty squares contain the digit 0.
After reading the puzzle, the constructor tries to solve it and records whether the puzzle has
a solution or not.*/
SudokuSolver::SudokuSolver(std::string input_file)
{
   readFile(input_file); // read the file
   solvable_ = solve();
}

//........................................................................................................
//........................................................................................................
//........................................................................................................

/*returns true if the puzzle is solvable, false otherwise.
We suggest you keep track of this when solving the problem at construction,
rather than solving the puzzle again each time this function is called.*/
bool SudokuSolver::isPuzzleSolvable()
{
   return solvable_;
}

/// @brief records the puzzle as being solvable.
void SudokuSolver::setSolvable()
{
   solvable_ = true;
}

/// @brief  returns the  2D pointer array representing the puzzle
/// @return array of integers
int **SudokuSolver::getPuzzleNumbers()
{
   return array_;
}

/// @brief setPuzzleNumbers(int** puzzle)
/// @param puzzle  the array of numbers 9X9
void SudokuSolver::setPuzzleNumbers(int **puzzle)
{ // should this be void or int**?
   for (int i = 0; i < 9; i++)
   {
      for (int j = 0; j < 9; j++)
      {
         array_[i][j] = puzzle[i][j];
      }
   }
}

/// @brief displays the puzzel on screen
void SudokuSolver::display()
{
   for (int row = 0; row < 9; row++)
   {
      for (int col = 0; col < 9; col++)
      {
         if (col == 8) {
            if (array_[row][col]==0)
               std:: cout << 'X';
            else 
                std::cout << array_[row][col];
         }
         else {
            if (array_[row][col]==0)
               std:: cout << "X ";
            else 
               std::cout << array_[row][col] << " ";
         }
         if (col == 2 || col == 5)
         {
            std::cout << " | ";
         }
      }
      if (row == 2 || row == 5)
      {
         std::cout << std::endl;
         std::cout << "- - - - - - - - - - - -";
      }
      std::cout << std::endl;
   }
}
