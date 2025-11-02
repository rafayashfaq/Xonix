/*
    MinHeap.cpp
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - DSA Project

    Description:
    This file contains the implementation of the MinHeap class methods.
*/

#include "MinHeap.h"
#include <algorithm>
#include <iostream>

MinHeap::MinHeap() {
    heap = new PlayerScore[MAX_SIZE];
    size = 0;
}

MinHeap::~MinHeap() {
    delete[] heap;
}

void MinHeap::swap(int i, int j) {
    PlayerScore temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void MinHeap::heapifyUp(int index) {
    while (index > 0 && heap[parent(index)].score > heap[index].score) {
        swap(index, parent(index));
        index = parent(index);
    }
}

void MinHeap::heapifyDown(int index) {
    int smallest = index;
    int left = leftChild(index);
    int right = rightChild(index);

    if (left < size && heap[left].score < heap[smallest].score)
        smallest = left;

    if (right < size && heap[right].score < heap[smallest].score)
        smallest = right;

    if (smallest != index) {
        swap(index, smallest);
        heapifyDown(smallest);
    }
}

void MinHeap::insert(const std::string& username, int score) {
    std::cout << "MinHeap::insert - Username: '" << username << "', Score: " << score << std::endl;

    if (size < MAX_SIZE) {
        // If heap is not full, simply insert
        std::cout << "  Heap not full, inserting at position " << size << std::endl;
        heap[size] = PlayerScore(username, score);
        heapifyUp(size);
        size++;
    }
    else if (score > heap[0].score) {
        // If new score is higher than minimum, replace root
        std::cout << "  Heap full, replacing minimum score " << heap[0].score << " with " << score << std::endl;
        heap[0] = PlayerScore(username, score);
        heapifyDown(0);
    }
    else {
        std::cout << "  Score not higher than minimum, not inserted" << std::endl;
    }
}

PlayerScore MinHeap::removeMin() {
    if (isEmpty()) {
        return PlayerScore();
    }

    PlayerScore min = heap[0];
    heap[0] = heap[size - 1];
    size--;
    heapifyDown(0);
    return min;
}

PlayerScore MinHeap::getMin() const {
    if (isEmpty()) {
        return PlayerScore();
    }
    return heap[0];
}

bool MinHeap::isEmpty() const {
    return size == 0;
}

int MinHeap::getSize() const {
    return size;
}

void MinHeap::updateScore(const std::string& username, int newScore) {
    std::cout << "MinHeap::updateScore - Username: '" << username << "', New Score: " << newScore << std::endl;

    // Find the player in the heap
    for (int i = 0; i < size; i++) {
        if (heap[i].username == username) {
            int oldScore = heap[i].score;
            std::cout << "  Found existing user at index " << i << " with score " << oldScore << std::endl;

            heap[i].score = newScore;

            // If score increased, heapify up
            if (newScore > oldScore) {
                std::cout << "  Score increased, heapifying up" << std::endl;
                heapifyUp(i);
            }
            // If score decreased, heapify down
            else if (newScore < oldScore) {
                std::cout << "  Score decreased, heapifying down" << std::endl;
                heapifyDown(i);
            }
            return;
        }
    }

    // If player not found, try to insert
    std::cout << "  User not found in heap, inserting as new entry" << std::endl;
    insert(username, newScore);
}

PlayerScore* MinHeap::getTopPlayers() const {
    std::cout << "MinHeap::getTopPlayers - Creating array of " << size << " players" << std::endl;

    // Create a copy of the heap
    PlayerScore* sortedPlayers = new PlayerScore[size];
    for (int i = 0; i < size; i++) {
        sortedPlayers[i] = heap[i];
        std::cout << "  Copying player " << i + 1 << ": " << heap[i].username << " - " << heap[i].score << std::endl;
    }

    // Sort in descending order
    std::sort(sortedPlayers, sortedPlayers + size,
        [](const PlayerScore& a, const PlayerScore& b) {
            return a.score > b.score;
        });

    std::cout << "Sorted players by score:" << std::endl;
    for (int i = 0; i < size; i++) {
        std::cout << "  Sorted position " << i + 1 << ": " << sortedPlayers[i].username << " - " << sortedPlayers[i].score << std::endl;
    }

    return sortedPlayers;
}

void MinHeap::clear() {
    size = 0;
}