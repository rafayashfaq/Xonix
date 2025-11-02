/*
    MinHeap.h
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - DSA Project

    Description:
    This file contains the MinHeap class implementation for the leaderboard system.
*/

#ifndef MINHEAP_H
#define MINHEAP_H

#include <string>
#include <ctime>

struct PlayerScore {
    std::string username;
    int score;
    time_t timestamp;

    PlayerScore() : score(0), timestamp(0) {}
    PlayerScore(const std::string& uname, int s) : username(uname), score(s), timestamp(time(nullptr)) {}
};

class MinHeap {
private:
    static const int MAX_SIZE = 10;  // Keep top 10 players
    PlayerScore* heap;
    int size;

    // Helper functions
    void heapifyUp(int index);
    void heapifyDown(int index);
    int parent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return 2 * i + 1; }
    int rightChild(int i) const { return 2 * i + 2; }
    void swap(int i, int j);

public:
    // Constructor and Destructor
    MinHeap();
    ~MinHeap();

    // Core operations
    void insert(const std::string& username, int score);
    PlayerScore removeMin();
    PlayerScore getMin() const;
    bool isEmpty() const;
    int getSize() const;

    // Leaderboard specific operations
    void updateScore(const std::string& username, int newScore);
    PlayerScore* getTopPlayers() const;
    void clear();
};

#endif // MINHEAP_H