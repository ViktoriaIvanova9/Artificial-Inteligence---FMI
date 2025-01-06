#include <bits/stdc++.h>

#include "Queen.hpp"

int main()
{
    int N;
    clock_t start, end;
    std::cout << "Enter number of queens: ";
    std::cin >> N;

    Queen boardGame(N);

    start = clock();
    boardGame.solve();
    end = clock();

    if (N <= 100)
    {
       boardGame.printBoard();
    }

    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "Time taken by program is : " << std::fixed << time_taken << std::setprecision(3) << " sec " << std::endl;

    return 0;
}
