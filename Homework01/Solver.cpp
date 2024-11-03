#include "Solver.hpp"

Solver::Solver(Board initialBoard)
{
    this->initialBoard = initialBoard;
}

void Solver::IDAStar()
{
    int limit = initialBoard.calculateManhattanHeuristic();

    while (true)
    {
        std::pair<Board *, int> elem = limitedSearch(initialBoard, limit);
        if (elem.first)
        {
            return;
        }
        if (elem.second != INT16_MAX)
        {
            limit = elem.second;
        }
    }
    return;
}

std::pair<Board *, int> Solver::limitedSearch(Board &currentBoard, int limit)
{
    int fCost = currentBoard.calculateManhattanHeuristic() + pathToSolvedBoard.size();
    if (fCost > limit)
    {
        return std::make_pair(nullptr, fCost);
    }
    if (currentBoard.isTargetBoard())
    {
        return std::make_pair(&currentBoard, fCost);
    }

    int minValue = INT16_MAX;
    vector<pair<Board, string>> successors = currentBoard.boardNeighbors();
    for (auto neighbour : successors)
    {
        if (std::find_if(pathToSolvedBoard.begin(), pathToSolvedBoard.end(), [&neighbour](std::pair<Board, std::string> &p)
                         { return p.first.areEqualBoards(neighbour.first); }) != pathToSolvedBoard.end())
        {
            continue;
        }
        pathToSolvedBoard.push_back(neighbour);

        std::pair<Board *, int> newElem = limitedSearch(neighbour.first, limit);

        if (newElem.first)
        {
            return newElem;
        }
        if (newElem.second < minValue)
        {
            minValue = newElem.second;
        }

        pathToSolvedBoard.pop_back();
    }
    return std::make_pair(nullptr, minValue);
}

void Solver::printSolution()
{
    std::cout << pathToSolvedBoard.size() << std::endl;
    for (int i = 0; i < pathToSolvedBoard.size(); ++i)
    {
        std::cout << pathToSolvedBoard[i].second << std::endl;
    }
}