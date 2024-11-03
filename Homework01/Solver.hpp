#ifndef __SOLVER_HPP
#define __SOLVER_HPP

#include <limits>
#include <stack>
#include <utility>
#include <cmath>

#include "Board.hpp"

using std::stack;

class Solver
{
private:
    Board initialBoard;
    vector<pair<Board,string>> pathToSolvedBoard = {};

public:
    Solver(Board initialBoard);

    void IDAStar();
    std::pair<Board *, int> limitedSearch(Board &currentBoard, int limit);

    void printSolution();
};

#endif