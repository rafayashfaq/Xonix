/*
    ArrayList.h
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - Data Structures and Algorithms Project

    Description:
    This file contains the implementation of a dynamic Array List data structure.
    The Array List is used for storing all player information in the game system.
    It allows for efficient indexed access and dynamic resizing.
*/

#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <iostream>
using namespace std;

// Initial capacity of the array list
#define INITIAL_CAPACITY 10

// Structure to store basic player information in the array list
struct PlayerInfo {
    int playerID;       // Unique identifier for the player
    char username[50];  // Player's username
    int totalScore;     // Player's total score
    bool isLoggedIn;    // Whether the player is currently logged in

    // Default constructor
    PlayerInfo() {
        playerID = -1;
        username[0] = '\0';
        totalScore = 0;
        isLoggedIn = false;
    }

    // Parameterized constructor
    PlayerInfo(int id, const char* name, int score = 0, bool logged = false) {
        playerID = id;
        strncpy_s(username, name, 49);
        username[49] = '\0'; // Ensure null termination
        totalScore = score;
        isLoggedIn = logged;
    }
};

class ArrayList {
private:
    PlayerInfo* array;  // Dynamic array to store player info
    int capacity;       // Current capacity of the array
    int size;           // Current number of elements in the array

    // Helper function to resize the array when needed
    void resize(int newCapacity) {
        // Create a new array with the new capacity
        PlayerInfo* newArray = new PlayerInfo[newCapacity];

        // Copy elements from the old array to the new array
        for (int i = 0; i < size; i++) {
            newArray[i] = array[i];
        }

        // Delete the old array
        delete[] array;

        // Update array pointer and capacity
        array = newArray;
        capacity = newCapacity;
    }

public:
    // Constructor
    ArrayList() {
        capacity = INITIAL_CAPACITY;
        size = 0;
        array = new PlayerInfo[capacity];
    }

    // Destructor
    ~ArrayList() {
        delete[] array;
    }

    // Returns the current size of the array list
    int getSize() {
        return size;
    }

    // Returns true if the array list is empty
    bool isEmpty() {
        return size == 0;
    }

    // Add a player to the end of the array list
    void add(const PlayerInfo& player) {
        // If the array is full, resize it
        if (size == capacity) {
            resize(capacity * 2);
        }

        // Add the new player at the end of the array
        array[size] = player;
        size++;
    }

    // Add a player at a specific index in the array list
    void add(int index, const PlayerInfo& player) {
        // Check if the index is valid
        if (index < 0 || index > size) {
            cout << "Invalid index for insertion." << endl;
            return;
        }

        // If the array is full, resize it
        if (size == capacity) {
            resize(capacity * 2);
        }

        // Shift elements to make space for the new player
        for (int i = size; i > index; i--) {
            array[i] = array[i - 1];
        }

        // Add the new player at the specified index
        array[index] = player;
        size++;
    }

    // Get a player at a specific index in the array list
    PlayerInfo get(int index) {
        // Check if the index is valid
        if (index < 0 || index >= size) {
            cout << "Invalid index for retrieval." << endl;
            return PlayerInfo();
        }

        return array[index];
    }

    // Update a player at a specific index in the array list
    void set(int index, const PlayerInfo& player) {
        // Check if the index is valid
        if (index < 0 || index >= size) {
            cout << "Invalid index for update." << endl;
            return;
        }

        array[index] = player;
    }

    // Remove a player at a specific index from the array list
    void remove(int index) {
        // Check if the index is valid
        if (index < 0 || index >= size) {
            cout << "Invalid index for removal." << endl;
            return;
        }

        // Shift elements to fill the gap
        for (int i = index; i < size - 1; i++) {
            array[i] = array[i + 1];
        }

        size--;

        // If the array is too empty, resize it to save space
        if (size > 0 && size == capacity / 4) {
            resize(capacity / 2);
        }
    }

    // Find a player by username
    int findByUsername(const char* username) {
        for (int i = 0; i < size; i++) {
            // Compare usernames using string comparison
            if (strcmp(array[i].username, username) == 0) {
                return i; // Return the index if found
            }
        }
        return -1; // Return -1 if not found
    }

    // Display all players (for debugging)
    void display() {
        cout << "Player List:" << endl;
        for (int i = 0; i < size; i++) {
            cout << "ID: " << array[i].playerID
                << ", Username: " << array[i].username
                << ", Score: " << array[i].totalScore
                << ", Logged In: " << (array[i].isLoggedIn ? "Yes" : "No") << endl;
        }
    }
};

#endif // ARRAYLIST_H