/*
    Leaderboard.h
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - DSA Project

    Description:
    This file contains the Leaderboard class that manages player scores
    using a MinHeap data structure.
*/

#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "MinHeap.h"
#include <string>

class Leaderboard {
private:
    MinHeap scoreHeap;
    std::string leaderboardFile;

    // Helper methods
    void saveToFile();
    void loadFromFile();

public:
    // Constructor
    Leaderboard(const std::string& filename = "leaderboard.dat");

    // Core operations
    void updateScore(const std::string& username, int score);
    PlayerScore* getTopScores(int& count) const;
    bool isTopScore(int score) const;
    void displayLeaderboard() const;
    void clearLeaderboard();
};

#endif // LEADERBOARD_H