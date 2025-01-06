#include <vector>
#include <utility>
#include <random>
#include <iostream>
#include <unordered_set>
#include <algorithm>

using std::pair;
using std::unordered_set;
using std::vector;

std::random_device rd;
std::mt19937 gen(rd());

int M, N;
vector<pair<int, int>> items;

const double MUTATION_RATE = 0.1;
const double CROSSOVER_RATE = 0.6;
const double REPRODUCTION_RATE = 0.3;

struct Individual
{
    vector<bool> itemsPresence;

    int calculateFitnessCoeff() const
    {
        int weightSum = 0;
        int valueSum = 0;

        for (int i = 0; i < items.size(); i++)
        {
            weightSum += (items[i].first * itemsPresence[i]);
            valueSum += (items[i].second * itemsPresence[i]);
        }

        if (weightSum > M)
            return 0;

        return valueSum;
    }

    struct HashFunction
    {
        size_t operator()(const Individual &individual) const
        {
            std::size_t h = 0;

            for (bool elem : individual.itemsPresence)
            {
                h ^= std::hash<bool>()(elem) + 0x9e3779b9 + (h << 6) + (h >> 2);
            }

            return h;
        }
    };

    bool operator==(const Individual &other) const
    {
        return itemsPresence == other.itemsPresence;
    }

    bool operator<(const Individual &other) const
    {
        return this->calculateFitnessCoeff() < other.calculateFitnessCoeff();
    }
};

void readInput()
{
    std::cin >> M >> N;

    int weight, value;

    for (int i = 0; i < N; ++i)
    {
        std::cin >> weight >> value;
        items.push_back(std::make_pair(weight, value));
    }
}

vector<Individual> initialPopulation(int individualsCount)
{
    unordered_set<Individual, Individual::HashFunction> population;
    std::uniform_int_distribution<int> distribution(0, 1);

    while (population.size() < individualsCount)
    {
        vector<bool> currentIndividual;

        for (int i = 0; i < N; ++i)
        {
            currentIndividual.push_back(distribution(gen));
        }

        population.insert(Individual{currentIndividual});
    }

    return vector<Individual>(population.begin(), population.end());
}

vector<Individual> performSelection(vector<Individual> &population)
{
    vector<Individual> parents;
    int parentsSize = 2;
    int tournamentParticipants = 3;

    for (int i = 0; i < parentsSize; ++i)
    {
        std::vector<Individual> tournamentVector;

            while (tournamentVector.size() < tournamentParticipants)
            {
                int i = rand() % population.size();
                tournamentVector.push_back(population[i]);
            }

            Individual tournamentWinner = tournamentVector[0];
            for (int i = 0; i < tournamentVector.size(); ++i)
            {
                if (tournamentVector[i].calculateFitnessCoeff() > tournamentWinner.calculateFitnessCoeff())
                {
                    tournamentWinner = tournamentVector[i];
                }
            }
            parents.push_back(tournamentWinner);
    }

    return parents;
}

vector<Individual> performCrossover(vector<Individual> &parents)
{
    vector<Individual> children;
    std::uniform_real_distribution<double> distribution(0.00, 1.00);
    vector<bool> childOne, childTwo;

    for (int i = 0; i < N; ++i)
    {
        if (distribution(gen) < CROSSOVER_RATE)
        {
            childOne.push_back(parents[0].itemsPresence[i]);
            childTwo.push_back(parents[1].itemsPresence[i]);
        }
        else
        {
            childOne.push_back(parents[1].itemsPresence[i]);
            childTwo.push_back(parents[0].itemsPresence[i]);
        }
    }

    children.push_back(Individual{childOne});
    children.push_back(Individual{childTwo});

    return children;
}

void performMutation(vector<Individual> &individuals)
{
    std::uniform_real_distribution<double> distribution(0.00, 1.00);
    for (auto &element : individuals)
    {
        for (int i = 0; i < N; ++i)
        {
            if (distribution(gen) < MUTATION_RATE)
            {
                element.itemsPresence[i] = !element.itemsPresence[i];
            }
        }
    }
}

vector<Individual> nextGeneration(vector<Individual> &population)
{
    std::uniform_real_distribution<double> distribution(0.00, 1.00);
    vector<Individual> currentGeneration;

    while (currentGeneration.size() < population.size())
    {
        vector<Individual> children;
        vector<Individual> parents = performSelection(population);

        if (distribution(gen) < REPRODUCTION_RATE)
        {
            children = parents;
        }
        else
        {
            if (distribution(gen) < CROSSOVER_RATE)
            {
                children = performCrossover(parents);
            }

            if (distribution(gen) < MUTATION_RATE)
            {
                performMutation(children);
            }
        }

        currentGeneration.insert(currentGeneration.end(), children.begin(), children.end());
    }

    return vector<Individual>(currentGeneration.begin(), currentGeneration.begin() + population.size());
}

void printCurrentGeneration(vector<Individual> &population)
{
    int maxFitness = 0.0;
    int fitnessCoeff = 0.0;
    for (auto individual : population)
    {
        fitnessCoeff = individual.calculateFitnessCoeff();
        maxFitness = std::max(fitnessCoeff, maxFitness);
    }

    std::cout << "Current fitness: " << fitnessCoeff << std::endl;
}

void knapsackProblem()
{
    vector<Individual> population = initialPopulation(N);

    for (int i = 0; i < 10000; ++i)
    {
        if (i % 1000 == 0)
        {
            printCurrentGeneration(population);
        }
        population = nextGeneration(population);
    }
}

int main()
{
    readInput();

    std::cout << "Calculating result..." << std::endl;

    knapsackProblem();

    return 0;
}