#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

// Configuración del algoritmo genético
const int POPULATION_SIZE = 10;
const int GENE_LENGTH = 5; // Representa números de 0 a 31
const int GENERATIONS = 20;
const double MUTATION_RATE = 0.1;

// Función objetivo: Maximizar f(x) = x^2
int fitnessFunction(int x) {
    return x * x;
}

// Generar un número aleatorio en un rango
int randomInt(int min, int max) {
    static std::mt19937 rng(static_cast<unsigned>(time(nullptr)));
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

// Generar población inicial aleatoria
std::vector<std::vector<int>> generateInitialPopulation() {
    std::vector<std::vector<int>> population(POPULATION_SIZE, std::vector<int>(GENE_LENGTH));
    for (auto& individual : population) {
        for (auto& gene : individual) {
            gene = randomInt(0, 1);
        }
    }
    return population;
}

// Convertir un cromosoma binario a un número entero
int decodeChromosome(const std::vector<int>& chromosome) {
    int value = 0;
    for (size_t i = 0; i < chromosome.size(); ++i) {
        value = (value << 1) | chromosome[i];
    }
    return value;
}

// Selección por torneo
std::vector<int> tournamentSelection(const std::vector<std::vector<int>>& population) {
    int bestIndex = randomInt(0, POPULATION_SIZE - 1);
    for (int i = 0; i < 2; ++i) {
        int challengerIndex = randomInt(0, POPULATION_SIZE - 1);
        int challengerFitness = fitnessFunction(decodeChromosome(population[challengerIndex]));
        int bestFitness = fitnessFunction(decodeChromosome(population[bestIndex]));
        if (challengerFitness > bestFitness) {
            bestIndex = challengerIndex;
        }
    }
    return population[bestIndex];
}

// Cruzamiento de un punto
std::pair<std::vector<int>, std::vector<int>> crossover(const std::vector<int>& parent1, const std::vector<int>& parent2) {
    int crossoverPoint = randomInt(1, GENE_LENGTH - 1);
    std::vector<int> offspring1 = parent1, offspring2 = parent2;
    for (int i = crossoverPoint; i < GENE_LENGTH; ++i) {
        std::swap(offspring1[i], offspring2[i]);
    }
    return {offspring1, offspring2};
}

// Mutación
void mutate(std::vector<int>& individual) {
    for (auto& gene : individual) {
        if (static_cast<double>(randomInt(0, 100)) / 100.0 < MUTATION_RATE) {
            gene = 1 - gene; // Flip bit
        }
    }
}

// Algoritmo genético
void geneticAlgorithm() {
    auto population = generateInitialPopulation();

    for (int generation = 0; generation < GENERATIONS; ++generation) {
        std::vector<std::vector<int>> newPopulation;

        while (newPopulation.size() < POPULATION_SIZE) {
            auto parent1 = tournamentSelection(population);
            auto parent2 = tournamentSelection(population);
            auto [offspring1, offspring2] = crossover(parent1, parent2);
            mutate(offspring1);
            mutate(offspring2);
            newPopulation.push_back(offspring1);
            newPopulation.push_back(offspring2);
        }

        population = newPopulation;

        // Imprimir la mejor solución de la generación
        auto bestIndividual = *std::max_element(population.begin(), population.end(), [](const auto& a, const auto& b) {
            return fitnessFunction(decodeChromosome(a)) < fitnessFunction(decodeChromosome(b));
        });
        std::cout << "Generación " << generation + 1
                  << " - Mejor solución: x=" << decodeChromosome(bestIndividual)
                  << ", fitness=" << fitnessFunction(decodeChromosome(bestIndividual)) << "\n";
    }
}

int main() {
    std::cout << "Algoritmo Genético - Maximizar f(x) = x^2\n";
    geneticAlgorithm();
    return 0;
}
