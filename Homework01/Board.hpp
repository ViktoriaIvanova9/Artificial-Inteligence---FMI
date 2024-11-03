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
    int tileAt(int row, int col); // tile at (row, col) or 0 if blank
    int getBoardSize();           // board size n
    vector<int> getBoard();
    int numberOfWrongPositions(); // number of tiles out of place

    int calcManhattanBetweenTwoPoints(pair<int, int> objectOne, pair<int, int> objectTwo);
    int calculateManhattanHeuristic();

    bool isTargetBoard();                         // is this board the goal board?
    bool areEqualBoards(Board other);       // does this board equal y?
    vector<pair<Board, string>> boardNeighbors(); // all neighboring boards
    bool isSolvable();                            // is this board solvable?

    void printBoard();
};

#endif