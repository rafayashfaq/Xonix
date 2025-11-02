/*
    PriorityQueue.cpp
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - DSA Project

    Description:
    This file contains the implementation of the PriorityQueue class methods.
*/

#include "PriorityQueue.h"

PriorityQueue::PriorityQueue() {
    queue = new MatchmakingPlayer[MAX_SIZE];
    size = 0;
}

PriorityQueue::~PriorityQueue() {
    delete[] queue;
}

void PriorityQueue::swap(int i, int j) {
    MatchmakingPlayer temp = queue[i];
    queue[i] = queue[j];
    queue[j] = temp;
}

void PriorityQueue::heapifyUp(int index) {
    while (index > 0 && queue[parent(index)].score < queue[index].score) {
        swap(index, parent(index));
        index = parent(index);
    }
}

void PriorityQueue::heapifyDown(int index) {
    int largest = index;
    int left = leftChild(index);
    int right = rightChild(index);

    if (left < size && queue[left].score > queue[largest].score)
        largest = left;

    if (right < size && queue[right].score > queue[largest].score)
        largest = right;

    if (largest != index) {
        swap(index, largest);
        heapifyDown(largest);
    }
}

void PriorityQueue::enqueue(const std::string& username, int score) {
    if (size >= MAX_SIZE) {
        return;  // Queue is full
    }

    queue[size] = MatchmakingPlayer(username, score);
    heapifyUp(size);
    size++;
}

MatchmakingPlayer PriorityQueue::dequeue() {
    if (isEmpty()) {
        return MatchmakingPlayer();
    }

    MatchmakingPlayer max = queue[0];
    queue[0] = queue[size - 1];
    size--;
    heapifyDown(0);
    return max;
}

MatchmakingPlayer PriorityQueue::peek() const {
    if (isEmpty()) {
        return MatchmakingPlayer();
    }
    return queue[0];
}

bool PriorityQueue::isEmpty() const {
    return size == 0;
}

int PriorityQueue::getSize() const {
    return size;
}

bool PriorityQueue::findMatch(MatchmakingPlayer& player1, MatchmakingPlayer& player2) {
    if (size < 2) {
        return false;  // Not enough players for a match
    }

    // Get the two highest priority players
    player1 = dequeue();
    player2 = dequeue();
    return true;
}

void PriorityQueue::removePlayer(const std::string& username) {
    // Find the player in the queue
    for (int i = 0; i < size; i++) {
        if (queue[i].username == username) {
            // Move the last element to this position
            queue[i] = queue[size - 1];
            size--;

            // If we moved the last element, no need to heapify
            if (i == size) {
                return;
            }

            // Heapify from the position where we made the change
            int parent = (i - 1) / 2;
            if (i > 0 && queue[i].score > queue[parent].score) {
                heapifyUp(i);
            }
            else {
                heapifyDown(i);
            }
            return;
        }
    }
}

void PriorityQueue::clear() {
    size = 0;
}