#include "Board.hpp"

int Board::numberOfInversions()
{
    int numInversions = 0;
    for (int i = 0; i < board.size() - 1; i++)
    {
        for (int j = i + 1; j < board.size(); j++)
        {
            if (board[i] > board[j] && board[i] != 0 && board[j] != 0)
                numInversions++;
        }
    }
    return numInversions;
}

pair<int, int> Board::calcRowAndColumn(int indexTile)
{
    int row = indexTile / getDimension();
    int column = indexTile % getDimension();
    return std::make_pair(row, column);
}

Board Board::getTargetBoard()
{
    if (positionBlank == -1)
    {
        positionBlank = board.size() - 1;
    }
    vector<int> result;

    for (int i = 0; i < positionBlank; ++i)
    {
        result.push_back(i + 1);
    }
    result.push_back(0);
    for (int i = positionBlank + 1; i < board.size(); ++i)
    {
        result.push_back(i);
    }

    return Board(this->N, this->positionBlank, result);
}

int Board::getDimension()
{
    return sqrt(board.size());
}

int Board::findIdxOfTile(vector<int> currentBoard, int tileNumber)
{
    for (int i = 0; i < currentBoard.size(); i++)
    {
        if (tileNumber == currentBoard[i])
        {
            return i;
        }
    }

    return -1;
}

Board::Board() {}

Board::Board(int N, int emptyTileIndex, vector<int> board)
{
    this->N = N;
    this->positionBlank = emptyTileIndex;
    this->board = board;
}

int Board::tileAt(int row, int col)
{
    return board[row * getDimension() + col];
}

int Board::getBoardSize()
{
    return board.size();
}

vector<int> Board::getBoard()
{
    return board;
}

int Board::numberOfWrongPositions()
{
    return 0;
}

int Board::calcManhattanBetweenTwoPoints(pair<int, int> objectOne, pair<int, int> objectTwo)
{
    int dist = abs(objectTwo.first - objectOne.first) + abs(objectTwo.second - objectOne.second);
    return dist;
}

int Board::calculateManhattanHeuristic()
{
    int sumDistance = 0;

    for (int i = 0; i < board.size(); i++)
    {
        if (board[i] != 0)
        {
            pair<int, int> posInBoard = calcRowAndColumn(i);

            int idx = findIdxOfTile(getTargetBoard().board, board[i]);
            pair<int, int> posInTargetBoard = calcRowAndColumn(idx);

            sumDistance += calcManhattanBetweenTwoPoints(posInBoard, posInTargetBoard);
        }
    }

    return sumDistance;
}

bool Board::isTargetBoard()
{
    Board targetBoard = getTargetBoard();
    return areEqualBoards(targetBoard);
}

bool Board::areEqualBoards(Board other)
{
    for (int i = 0; i < other.board.size(); i++)
    {
        if (this->board[i] != other.board[i])
        {
            return false;
        }
    }
    return true;
}

vector<pair<Board, string>> Board::boardNeighbors()
{
    vector<pair<Board, string>> newBoards = {};

    int blankPos = std::find(board.begin(), board.end(), 0) - board.begin();
    pair<int, int> position = calcRowAndColumn(blankPos);
    vector<pair<int, int>> directions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    vector<string> stringDirections = {"LEFT", "DOWN", "RIGHT", "UP"};

    // std::cout << "WOWW: " << position.first << " " << position.second << std::endl;

    for (int i = 0; i < directions.size(); i++)
    {
        int newRow = position.first + directions[i].first;
        int newCol = position.second + directions[i].second;

        // std::cout << "NEIGHBOUR CHOSEN " << newRow << " " << newCol << std::endl;

        if (newRow >= 0 && newRow < getDimension() && newCol >= 0 && newCol < getDimension())
        {
            int newPos = newRow * getDimension() + newCol;

            Board newBoard = Board(N, positionBlank, board);
            std::swap(newBoard.board[blankPos], newBoard.board[newPos]);
            newBoards.push_back({newBoard, stringDirections[i]});
        }
    }

    return newBoards;
}

bool Board::isSolvable()
{
    int blank = std::find(board.begin(), board.end(), 0) - board.begin();
    return (getDimension() % 2 == 0 && ((numberOfInversions() +
                                         calcRowAndColumn(blank).first + calcRowAndColumn(positionBlank).first) % 2 == 1)) ||
           (getDimension() % 2 == 1 && numberOfInversions() % 2 == 0);
}

void Board::printBoard()
{
    int cnt = 0;
    std::cout << std::endl;
    for (int i = 0; i < board.size(); ++i)
    {
        cnt++;
        std::cout << board[i] << " ";
        if (cnt == getDimension())
        {
            std::cout << std::endl;
            cnt = 0;
        }
    }
    std::cout << std::endl;
}
