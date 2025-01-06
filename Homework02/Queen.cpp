#include "Queen.hpp"

#include <iostream>
#include <utility>
#include <random>

using std::pair;

Queen::Queen(int N)
{
    this->N = N;
    this->nQueens = vector<int>(N, -1);
    this->queensPerRow = vector<int>(N, 0);
    this->queensPerD1 = vector<int>(2 * N - 1, 0);
    this->queensPerD2 = vector<int>(2 * N - 1, 0);
}

int Queen::getColWithQueenWithMaxConf()
{
    vector<int> rowsWithSameConflict;
    int maxConflicts = -1;

    for (int col = 0; col < N; ++col)
    {
        int queenRow = nQueens[col];
        int currConflicts = queensPerRow[queenRow] + queensPerD1[queenRow - col + N - 1] + queensPerD2[queenRow + col] - 3;

        if (maxConflicts < currConflicts)
        {
            maxConflicts = currConflicts;
            rowsWithSameConflict.clear();
            rowsWithSameConflict.push_back(col);
        }
        else if (currConflicts == maxConflicts)
        {
            rowsWithSameConflict.push_back(col);
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, rowsWithSameConflict.size() - 1);
    int randomIndex = distribution(gen);

    return rowsWithSameConflict[randomIndex];
}

void Queen::updateBoardWithMinConflictRow(int col)
{
    int minConflictedRow = 0;
    int totalConflicts = N;

    for (int row = 0; row < N; row++)
    {
        int numConflicts = queensPerRow[row] + queensPerD1[row - col + N - 1] + queensPerD2[row + col] - 3;

        if (numConflicts < totalConflicts)
        {
            totalConflicts = numConflicts;
            minConflictedRow = row;
        }
    }

    int currentQueenRow = nQueens[col];

    queensPerD1[currentQueenRow - col + N - 1]--;
    queensPerD2[currentQueenRow + col]--;
    queensPerRow[currentQueenRow]--;

    queensPerD1[minConflictedRow - col + N - 1]++;
    queensPerD2[minConflictedRow + col]++;
    queensPerRow[minConflictedRow]++;
    nQueens[col] = minConflictedRow;
}

bool Queen::hasConflicts()
{
    for (int i = 0; i < N; ++i) {
        if (queensPerRow[i] > 1) {
            return true;
        }
    }
    for (int i = 0; i < 2 * N - 1; ++i) {
        if (queensPerD1[i] > 1 || queensPerD2[i] > 1) {
            return true;
        }
    }
    return false;
}

int Queen::solve()
{
    setInitialBoard();
    int iter = 0;
    while (iter++ <= 200)
    {
        if (!hasConflicts())
        {
            std::cout << "Solution found!\n";
            return 0;
        }

        int col = getColWithQueenWithMaxConf();
        updateBoardWithMinConflictRow(col);
        // printBoard();
        // std::cout<<std::endl;
    }
    if (hasConflicts())
    {
        return solve();
    }

    return 0;
}

void Queen::printBoard()
{
    for (int row = 0; row < N; ++row)
    {
        for (int col = 0; col < N; ++col)
        {
            if (nQueens[col] == row)
            {
                std::cout << "* ";
            }
            else
            {
                std::cout << "_ ";
            }
        }
        std::cout << std::endl;
    }
}

void Queen::setInitialBoard()
{
    this->nQueens = vector<int>(N, -1);
    this->queensPerRow = vector<int>(N, 0);
    this->queensPerD1 = vector<int>(2 * N - 1, 0);
    this->queensPerD2 = vector<int>(2 * N - 1, 0);

    // pair<int, int> moveKnight = {1, 2};
    // pair<int, int> position = {0, 0};
    // nQueens[position.second] = position.first;
    // queensPerRow[position.first]++;
    // queensPerD1[position.first - position.second + N - 1]++;
    // queensPerD2[position.first + position.second]++;

    // for (int i = 0; i < N - 1; i++)
    // {
    //     position.first += moveKnight.first;
    //     position.second += moveKnight.second;

    //     if (position.second >= N)
    //     {
    //         if (position.first >= N)
    //         {
    //             return;
    //         }
    //         position.second = 1;
    //     }
    //     nQueens[position.second] = position.first;
    //     queensPerRow[position.first]++;
    //     queensPerD1[position.first - position.second + N - 1]++;
    //     queensPerD2[position.first + position.second]++;
    // }

    // return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distribution(0, N - 1);

    for (int col = 0; col < N; ++col)
    {
        int row = distribution(gen);
        nQueens[col] = row;
        queensPerRow[row]++;
        queensPerD1[row - col + N - 1]++;
        queensPerD2[row + col]++;
    }
}