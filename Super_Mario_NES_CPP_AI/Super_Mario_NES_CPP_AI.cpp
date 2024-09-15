// Super_Mario_NES_CPP_AI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <thread> // For std::this_thread::sleep_for
#include <chrono> // For std::chrono::milliseconds


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
