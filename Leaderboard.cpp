/*
    Leaderboard.cpp
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - DSA Project

    Description:
    Implementation of the Leaderboard class methods.
*/
#define _CRT_SECURE_NO_WARNINGS
#include "Leaderboard.h"
#include <fstream>
#include <iostream>
#include <ctime>

Leaderboard::Leaderboard(const std::string& filename) : leaderboardFile(filename) {
    loadFromFile();
}

void Leaderboard::updateScore(const std::string& username, int score) {
    // First check if this score is worthy of the leaderboard
    std::cout << "Updating leaderboard for user: '" << username << "' with score: " << score << std::endl;

    if (scoreHeap.getSize() < 10 || score > scoreHeap.getMin().score) {
        scoreHeap.updateScore(username, score);
        std::cout << "Score added to leaderboard. Current heap size: " << scoreHeap.getSize() << std::endl;
        saveToFile();
    }
    else {
        std::cout << "Score not high enough for leaderboard." << std::endl;
    }
}

PlayerScore* Leaderboard::getTopScores(int& count) const {
    // Get array of top scores from heap
    std::cout << "Leaderboard::getTopScores - Getting scores from heap" << std::endl;
    PlayerScore* topPlayers = scoreHeap.getTopPlayers();
    count = scoreHeap.getSize();
    std::cout << "Retrieved " << count << " scores from heap" << std::endl;

    // Create a copy to sort
    PlayerScore* sortedScores = new PlayerScore[count];
    for (int i = 0; i < count; i++) {
        sortedScores[i] = topPlayers[i];
        std::cout << "  Score " << i + 1 << ": " << sortedScores[i].username << " - " << sortedScores[i].score << std::endl;
    }

    // Sort in descending order by score using bubble sort
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (sortedScores[j].score < sortedScores[j + 1].score) {
                // Swap
                PlayerScore temp = sortedScores[j];
                sortedScores[j] = sortedScores[j + 1];
                sortedScores[j + 1] = temp;
            }
        }
    }

    std::cout << "Sorted scores:" << std::endl;
    for (int i = 0; i < count; i++) {
        std::cout << "  Position " << i + 1 << ": " << sortedScores[i].username << " - " << sortedScores[i].score << std::endl;
    }

    delete[] topPlayers;  // Clean up the original array
    return sortedScores;
}

bool Leaderboard::isTopScore(int score) const {
    // If leaderboard isn't full yet, any score can get in
    if (scoreHeap.getSize() < 10) {
        return true;
    }

    // Otherwise, score needs to be higher than current minimum
    return score > scoreHeap.getMin().score;
}

void Leaderboard::displayLeaderboard() const {
    int count;
    PlayerScore* sortedScores = getTopScores(count);

    std::cout << "\n======= LEADERBOARD =======\n";
    std::cout << "Rank | Player         | Score | Date\n";
    std::cout << "---------------------+-------+------------\n";

    for (int i = 0; i < count; i++) {
        std::string dateStr = std::ctime(&sortedScores[i].timestamp);
        dateStr = dateStr.substr(0, dateStr.length() - 1);  // Remove trailing newline

        std::cout << i + 1 << ".   | ";
        std::cout << sortedScores[i].username;

        // Pad username for alignment
        int padding = 15 - sortedScores[i].username.length();
        for (int j = 0; j < padding; j++) {
            std::cout << " ";
        }

        std::cout << " | " << sortedScores[i].score << " | " << dateStr << "\n";
    }

    std::cout << "==========================\n";

    delete[] sortedScores;  // Clean up
}

void Leaderboard::clearLeaderboard() {
    scoreHeap.clear();
    saveToFile();
}

void Leaderboard::saveToFile() {
    std::cout << "Saving leaderboard to file: " << leaderboardFile << std::endl;

    std::ofstream file(leaderboardFile.c_str(), std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open leaderboard file for writing\n";
        return;
    }

    // Get all scores currently in the heap
    PlayerScore* scores = scoreHeap.getTopPlayers();
    int size = scoreHeap.getSize();

    std::cout << "Saving " << size << " scores to leaderboard file" << std::endl;

    // Write size first
    file.write(reinterpret_cast<char*>(&size), sizeof(int));

    // Write each score record
    for (int i = 0; i < size; i++) {
        // Write username length then username string
        int usernameLength = scores[i].username.length();
        std::cout << "  Score " << i + 1 << ": " << scores[i].username << " - " << scores[i].score << std::endl;

        file.write(reinterpret_cast<char*>(&usernameLength), sizeof(int));
        file.write(scores[i].username.c_str(), usernameLength);

        // Write score and timestamp
        file.write(reinterpret_cast<char*>(&scores[i].score), sizeof(int));
        file.write(reinterpret_cast<char*>(&scores[i].timestamp), sizeof(time_t));
    }

    delete[] scores;  // Clean up the array
    file.close();
    std::cout << "Leaderboard saved successfully" << std::endl;
}

void Leaderboard::loadFromFile() {
    std::cout << "Loading leaderboard from file: " << leaderboardFile << std::endl;

    std::ifstream file(leaderboardFile.c_str(), std::ios::binary);
    if (!file) {
        std::cerr << "Note: Leaderboard file not found. Starting with empty leaderboard.\n";
        return;
    }

    // Clear existing heap
    scoreHeap.clear();

    // Read size
    int size;
    file.read(reinterpret_cast<char*>(&size), sizeof(int));
    std::cout << "Found " << size << " scores in leaderboard file" << std::endl;

    // Read each score record
    for (int i = 0; i < size; i++) {
        // Read username
        int usernameLength;
        file.read(reinterpret_cast<char*>(&usernameLength), sizeof(int));

        char* usernameBuffer = new char[usernameLength + 1];
        file.read(usernameBuffer, usernameLength);
        usernameBuffer[usernameLength] = '\0';
        std::string username(usernameBuffer);
        delete[] usernameBuffer;

        // Read score and timestamp
        int score;
        time_t timestamp;
        file.read(reinterpret_cast<char*>(&score), sizeof(int));
        file.read(reinterpret_cast<char*>(&timestamp), sizeof(time_t));

        std::cout << "  Loaded score " << i + 1 << ": " << username << " - " << score << std::endl;

        // We need to add this player score to the heap
        // Since we're loading from a saved file, we'll insert directly
        scoreHeap.insert(username, score);
    }

    std::cout << "Leaderboard loaded successfully. Heap size: " << scoreHeap.getSize() << std::endl;
    file.close();
}