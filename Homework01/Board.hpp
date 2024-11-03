#ifndef __BOARD_HPP
#define __BOARD_HPP

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using std::pair;
using std::string;
using std::vector;

class Board
{
private:
    int N;
    vector<int> board; // presented as one-dimensional vector
    int positionBlank;

    vector<Board> visitedBoards;

    int numberOfInversions();
    pair<int, int> calcRowAndColumn(int indexTile);
    int getDimension();
    int findIdxOfTile(vector<int> currentBoard, int tileNumber);

public:
    Board();
    Board(int N, int emptyTileIndex, vector<int> board);

    Board getTargetBoard();
    int getBoardSize();
    vector<int> getBoard();

    int calcManhattanBetweenTwoPoints(pair<int, int> objectOne, pair<int, int> objectTwo);
    int calculateManhattanHeuristic();

    bool isTargetBoard();
    bool areEqualBoards(Board other);
    vector<pair<Board, string>> boardNeighbors();
    bool isSolvable();

};

#endif