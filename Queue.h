/*
    Queue.h
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - Data Structures and Algorithms Project

    Description:
    This file contains the implementation of a simple Queue data structure.
    The Queue is used for the Game Room functionality, where players wait
    in a First-In-First-Out (FIFO) manner for matchmaking.
*/

#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
using namespace std;

// Maximum number of players that can wait in the queue
#define MAX_QUEUE_SIZE 100

// Structure to store player information in the queue
struct QueueNode {
    int playerID;    // Unique identifier for the player

    // Default constructor
    QueueNode() {
        playerID = -1;
    }

    // Parameterized constructor
    QueueNode(int id) {
        playerID = id;
    }
};

class Queue {
private:
    QueueNode items[MAX_QUEUE_SIZE];  // Array to store queue elements
    int front;                        // Index of the front element
    int rear;                         // Index of the rear element
    int size;                         // Current number of elements in the queue

public:
    // Constructor
    Queue() {
        front = 0;
        rear = -1;
        size = 0;
    }

    // Returns true if the queue is empty
    bool isEmpty() {
        return size == 0;
    }

    // Returns true if the queue is full
    bool isFull() {
        return size == MAX_QUEUE_SIZE;
    }

    // Returns the current size of the queue
    int getSize() {
        return size;
    }

    // Enqueue a new player into the queue
    void enqueue(int playerID) {
        if (isFull()) {
            cout << "Queue is full. Cannot add more players." << endl;
            return;
        }

        // Increase rear circularly
        rear = (rear + 1) % MAX_QUEUE_SIZE;
        items[rear] = QueueNode(playerID);
        size++;
    }

    // Dequeue a player from the queue (FIFO)
    QueueNode dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty." << endl;
            return QueueNode();
        }

        QueueNode frontItem = items[front];

        // Increase front circularly
        front = (front + 1) % MAX_QUEUE_SIZE;
        size--;

        return frontItem;
    }

    // Peek at the front player in the queue without removing
    QueueNode peek() {
        if (isEmpty()) {
            cout << "Queue is empty." << endl;
            return QueueNode();
        }
        return items[front];
    }

    // Display the queue (for debugging)
    void display() {
        if (isEmpty()) {
            cout << "Queue is empty." << endl;
            return;
        }

        cout << "Queue (Game Room Waiting List):" << endl;

        int count = 0;
        int i = front;

        while (count < size) {
            cout << "Position " << (count + 1) << ": PlayerID " << items[i].playerID << endl;
            i = (i + 1) % MAX_QUEUE_SIZE;
            count++;
        }
    }
};

#endif // QUEUE_H