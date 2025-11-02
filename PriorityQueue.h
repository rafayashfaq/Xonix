/*
    PriorityQueue.h
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - DSA Project

    Description:
    This file contains the PriorityQueue class implementation for the matchmaking system.
*/

#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <string>
#include <ctime>

struct MatchmakingPlayer {
    std::string username;
    int score;
    time_t joinTime;

    MatchmakingPlayer() : score(0), joinTime(0) {}
    MatchmakingPlayer(const std::string& uname, int s) : username(uname), score(s), joinTime(time(nullptr)) {}
};

class PriorityQueue {
private:
    static const int MAX_SIZE = 100;  // Maximum number of players in queue
    MatchmakingPlayer* queue;
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
    PriorityQueue();
    ~PriorityQueue();

    // Core operations
    void enqueue(const std::string& username, int score);
    MatchmakingPlayer dequeue();
    MatchmakingPlayer peek() const;
    bool isEmpty() const;
    int getSize() const;

    // Matchmaking specific operations
    bool findMatch(MatchmakingPlayer& player1, MatchmakingPlayer& player2);
    void removePlayer(const std::string& username);
    void clear();
};

#endif // PRIORITYQUEUE_H