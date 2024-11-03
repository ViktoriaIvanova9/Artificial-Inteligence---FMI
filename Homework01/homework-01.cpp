#include <bits/stdc++.h>

#include "Solver.hpp"

void readInput(int &N, int &emptyTileIndex, vector<int> &initialConfiguration)
{
    std::cout << "Enter number of tiles: ";
    std::cin >> N;
    std::cout << "Enter final position of the empty tile: ";
    std::cin >> emptyTileIndex;

    int dimensions = sqrt(N + 1);

    std::cout << "Enter the board:\n";
    for (int i = 0; i < dimensions; i++)
    {
        for (int j = 0; j < dimensions; j++)
        {
            int currentTile;
            std::cin >> currentTile;
            initialConfiguration.push_back(currentTile);
        }
    }
}

int main()
{
    clock_t start, end;
    int N;
    int emptyTileIndex;
    vector<int> initialConfiguration;

    readInput(N, emptyTileIndex, initialConfiguration);

    start = clock();
    std::ios_base::sync_with_stdio(false);
    Board initialBoard = Board(N, emptyTileIndex, initialConfiguration);
    Solver puzzleGame = Solver(initialBoard);

    if (initialBoard.isSolvable())
    {
        puzzleGame.IDAStar();
        puzzleGame.printSolution();
    }
    else
    {
        int numToSolve = -1;
        std::cout << numToSolve << std::endl;
    }

    end = clock();

    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "Time taken by program is : " << std::fixed << time_taken << std::setprecision(3) << " sec " << std::endl;

    return 0;
}