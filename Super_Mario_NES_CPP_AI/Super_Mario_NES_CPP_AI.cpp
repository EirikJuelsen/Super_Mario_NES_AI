// Super_Mario_NES_CPP_AI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <thread> // For std::this_thread::sleep_for
#include <chrono> // For std::chrono::milliseconds
#include <algorithm>
#include <cmath>
#include <random>


class Agent {
public:
    std::vector<double> weights; // The agent's genes
    double fitness;              // The agent's fitness score

    Agent(int num_weights) : fitness(0.0) {
        // Initialize weights randomly
        weights.resize(num_weights);
        for (auto& weight : weights) {
            weight = (rand() / double(RAND_MAX)) * 2 - 1; // Random values between -1 and 1
        }
    }

    // Simulate playing the game (stub function)
    void playGame() {
        // Implement the logic to control the emulator and play the game here.
        // For demonstration purposes, we'll set a random fitness score.
        fitness = rand() % 1000; // Replace with actual fitness evaluation
        
        /* HOW TO PLAY THE GAME AND GET GAME STATE
        const int frameDurationMs = 1000 / 60; // For 60 FPS: 16.67 milliseconds

        while (true) {
            // Continuously read the position from the file
            readValuesFromFile();

            // Calculate which input should be used

            // Send input to BizHawk
            // EXAMPLE: "RIGHT"
            sendInputToBizHawk("RIGHT");

            // Wait for the duration of one frame
            std::this_thread::sleep_for(std::chrono::milliseconds(frameDurationMs));
        }
        */

    }
};

class GeneticAlgorithm {
public:
    unsigned int population_size;
    unsigned int num_generations;
    double mutation_rate;
    unsigned int num_weights; //The number of avaible actions / buttons in the game
    std::vector<Agent> population; // Variable namede population that is a vector (dynamic array) of Agent (the class) objects.

    GeneticAlgorithm(int pop_size, int num_gen, double mut_rate, int num_w)
        : population_size(pop_size), num_generations(num_gen), mutation_rate(mut_rate), num_weights(num_w)
    {
        // Initialize the population with random "Agents"
        for (unsigned int i = 0; i < population_size; ++i)
        {
            population.emplace_back(num_weights); // Emplace_back creates a new element directly in the vector "population"
                                                  // instead of having to create an element and then move it in to the vector
        }
    }

    // Function to evaluate the fitness for the entire population
    void evalutateFitness() {
        for (auto &agent : population) { // Range based for loop that iterates through all Agents in the population
                                         // auto &agent is a reference to each "Agent" vector in the population
                                         // allowing the function to modify the agents in population.
            agent.playGame();
        }
    }


    // Function to select the top performing "Agents"
    std::vector<Agent> selectTopAgents() {
        // Sorting the population by fitness in descending order
        // "[](const Agent& a, const Agent& b)" is a lambda funtion that provides the custom sorting logic
        // If a.fitness > b.fitness is true, then "a" should come before "b" in the sorted order.
        std::sort(population.begin(), population.end(), [](const Agent& a, const Agent& b) {
            return a.fitness > b.fitness;
            });

        // Select the top 50% of "Agents"
        int cutoff = population_size / 2;
        return std::vector<Agent>(population.begin(), population.begin() + cutoff);
    }

    // FUnction that performs a crossover between two parent "Agents", that return the combined "Agent"/child
    Agent crossover(const Agent& parent1, const Agent& parent2) {
        // Creating the new "Agent"/child
        Agent child(num_weights);
        
        // Looping through all the weights
        // Using ++i is supposedly more efficient than using i++, since you are incrementing 
        // the value instead of making a copy of the value and incrementing that
        for (unsigned int i = 0; i < num_weights; ++i) {
            // Crossover that inherits weight from either parent, if the random result is bigger than 0.5
            // it takes the weight from parent1, and if it is smaller it takes the weight from parent2
            child.weights[i] = (rand() / double(RAND_MAX)) > 0.5 ? parent1.weights[i] : parent2.weights[i];
        }
        return child;
    }

    // Funtion to mutate "Agents" with a small probability
    // rand() / double(RAND_MAX) normalizes the value between 0 and 1, 
    // and only if that value is higher than the mutation_rate we mutate.
    // Multiplying by 2 and then subtracting 1 transforms the value so that i can vary between 1 and negative 1,
    // and then multiplied by 0.1 to only make the random change minimal.
    // This funtion makes it so that you introduce some genetic variaty and this can prevent the model 
    // from reaching a local optimal value and "force" it so search for new solutions.

    void mutate(Agent& agent) {
        for (auto& weight : agent.weights) {
            if ((rand() / double(RAND_MAX)) < mutation_rate) {
                weight += ((rand() / double(RAND_MAX)) * 2 - 1) * 0.1; // Small random change
            }
        }
    }

    void nextGeneration() {
        std::vector<Agent> new_population;
        // Select top performing "Agents"
        auto top_agents = selectTopAgents();

        // Create new agents through crossover and mutation
        while (new_population.size() < population_size) {
            // Randomly select two parents by getting a random value and using the remainder operator witht the size of the sample
            const Agent& parent1 = top_agents[rand() % top_agents.size()];
            const Agent& parent2 = top_agents[rand() % top_agents.size()];

            // Crossover
            Agent child = crossover(parent1, parent2);

            // Mutation
            mutate(child);

            // Add the new child to the population
            // This also ensures that the total population size remains consistant
            // since we started with only the top 50% from the selectTopAgent function
            new_population.push_back(child);
        }

        // Replace the old population with the new one
        // std::move is more efficent than copying the actual elements and it does not require C++ to take up double the amount of memory.
        // std::move allows C++ to transfers the values from one vector to another. When the function is done the local vector
        // new_population is deleted anyway.
        population = std::move(new_population);
    }

    void run() {
        for (unsigned int generation = 0; generation < num_generations; ++generation) {
            evalutateFitness();
            nextGeneration();
            std::cout << "Generation " << generation + 1 << " - Best Fitness: " << population[0].fitness << std::endl;
        }
    }
};

void sendInputToBizHawk(const std::string& command) {
    std::ofstream outFile("C:/Users/eirik/Source/Repos/Super_Mario_NES_AI/input.txt");
    outFile << command;  // Command could be something like "LEFT", "A", etc.
    outFile.close();
}

void readValuesFromFile() {
    // Specify the path to the file
    std::string filePath = "C:/Users/eirik/Source/Repos/Super_Mario_NES_AI/frame_info.txt"; // For Windows

    // Open the file for reading
    std::ifstream file(filePath);

    // Check if the file is open
    if (file.is_open()) {
        std::string line;

        // Read the FrameCount from the file
        if (std::getline(file, line)) {
            // Extract the FrameCount position (the line format is "Frame Count: <value>")
            int FrameCount = std::stoi(line.substr(line.find(":") + 1));
            //std::cout << "Frame Count: " << FrameCount << std::endl;
        }

        // Read the X position on screen from the file
        if (std::getline(file, line)) {
            // Extract the X position (the line format is "X Position: <value>")
            int xScreenPos = std::stoi(line.substr(line.find(":") + 1));
            //std::cout << "X Position on screen: " << xScreenPos << std::endl;
        }

        // Read the Y position on screen from the file
        if (std::getline(file, line)) {
            // Extract the Y position (the line format is "Y Position: <value>")
            int yScreenPos = std::stoi(line.substr(line.find(":") + 1));
            //std::cout << "Y Position on screen: " << yScreenPos << std::endl;
        }

        // Read the X position in level from the file
        if (std::getline(file, line)) {
            // Extract the X position in level (the line format is "X Position in level: <value>")
            int xLevelPos = std::stoi(line.substr(line.find(":") + 1));
            //std::cout << "X Position in level: " << xLevelPos << std::endl;
        }

        // Close the file
        file.close();
    }
    else {
        std::cerr << "Error: Unable to open file." << std::endl;
    }
}

int main() {
    unsigned int population_size = 50;
    unsigned int num_generations = 100;
    double mutation_rate = 0.01;
    unsigned int num_weights = 10; // Number of weights/genes/available inputs for each "Agent"

    GeneticAlgorithm ga(population_size, num_generations, mutation_rate, num_weights);
    ga.run();

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
