#include <vector>
#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>

using std::vector;

// green frogs = >
// yellow frogs = <
// calculate moves = (numberOfFrogs + 1)^2 - 1

void setInitialConfiguration(int numberOfFrogs, vector<char> &configFrogs)
{
    for (int i = 0; i < numberOfFrogs; i++)
    {
        configFrogs.push_back('>');
    }
    configFrogs.push_back('_');
    for (int i = 0; i < numberOfFrogs; i++)
    {
        configFrogs.push_back('<');
    }
}

bool canMoveNext(vector<char> &configuration, int place)
{
    return (configuration[place] == '>' && place + 1 < configuration.size() && configuration[place + 1] == '_') ||
           (configuration[place] == '<' && place - 1 >= 0 && configuration[place - 1] == '_');
}

bool canMoveAfterNext(vector<char> &configuration, int place)
{
    return (!canMoveNext(configuration, place) && configuration[place] == '>' && place + 2 < configuration.size() && configuration[place + 2] == '_') ||
           (!canMoveNext(configuration, place) && configuration[place] == '<' && place - 2 >= 0 && configuration[place - 2] == '_');
}

void printCurrentConfiguration(vector<char> &configuration)
{
    for (char elem : configuration)
    {
        std::cout << elem;
    }
    std::cout << std::endl;
}

void moveFrog(vector<char> &currentState, int &indexEmptyRock, bool isGreen)
{
    char frogType = isGreen ? '>' : '<';
    int i = 1;
    while (i < 3)
    {
        if (indexEmptyRock - i >= 0 && currentState[indexEmptyRock - i] == frogType &&
            (canMoveNext(currentState, indexEmptyRock - i) || canMoveAfterNext(currentState, indexEmptyRock - i)))
        {
            std::swap(currentState[indexEmptyRock - i], currentState[indexEmptyRock]);
            indexEmptyRock = indexEmptyRock - i;
            printCurrentConfiguration(currentState);
            return;
        }
        else if (indexEmptyRock + i < currentState.size() && currentState[indexEmptyRock + i] == frogType &&
                 (canMoveNext(currentState, indexEmptyRock + i) || canMoveAfterNext(currentState, indexEmptyRock + i)))
        {
            std::swap(currentState[indexEmptyRock + i], currentState[indexEmptyRock]);
            indexEmptyRock = indexEmptyRock + i;
            printCurrentConfiguration(currentState);
            return;
        }
        i++;
    }
}

void solvingAlgorithm(int numberOfFrogs, vector<char> &currentConfiguration, int &indexEmptyRock)
{
    bool isGreenFrogNow = false;

    for (int i = 1; i <= numberOfFrogs; i++)
    {
        for (int j = 0; j < i; j++)
        {
            moveFrog(currentConfiguration, indexEmptyRock, isGreenFrogNow);
        }
        isGreenFrogNow = !isGreenFrogNow;
    }
    for (int i = 0; i < numberOfFrogs; i++)
    {
        moveFrog(currentConfiguration, indexEmptyRock, isGreenFrogNow);
    }
    isGreenFrogNow = !isGreenFrogNow;

    for (int i = numberOfFrogs; i >= 0; i--)
    {
        for (int j = 0; j < i; j++)
        {
            moveFrog(currentConfiguration, indexEmptyRock, isGreenFrogNow);
        }
        isGreenFrogNow = !isGreenFrogNow;
    }
}

int main()
{
    clock_t start, end;
    int numberOfFrogs;
    std::cout << "Number of frogs: ";
    std::cin >> numberOfFrogs;

    int indexEmptyRock = numberOfFrogs;

    vector<char> initialConfiguration;
    setInitialConfiguration(numberOfFrogs, initialConfiguration);
    printCurrentConfiguration(initialConfiguration);

    start = clock();
    std::ios_base::sync_with_stdio(false);
    solvingAlgorithm(numberOfFrogs, initialConfiguration, indexEmptyRock);
    end = clock();

    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "Time taken by program is : " << std::fixed << time_taken << std::setprecision(3) << " sec " << std::endl;

    return 0;
}