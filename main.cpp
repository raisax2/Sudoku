#include "SudokuSolver.hpp"


int main()
{
    SudokuSolver puzzle;
    puzzle.display();

    std:: cout << puzzle.isPuzzleSolvable();

}