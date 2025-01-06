#ifndef __QUEEN_HPP
#define __QUEEN_HPP

#include <vector>

using std::vector;

class Queen
{
private:
    int N;

    vector<int> nQueens;
    vector<int> queensPerRow;
    vector<int> queensPerD1;
    vector<int> queensPerD2;

public:
    Queen(int N);

    int getColWithQueenWithMaxConf();
    void updateBoardWithMinConflictRow(int column);
    int solve();

    void printBoard();
    void setInitialBoard();
    bool hasConflicts();

};

#endif