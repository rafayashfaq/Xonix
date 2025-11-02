/*
    PlayerManager.h
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - Data Structures and Algorithms Project

    Description:
    This file contains the PlayerManager class which handles player registration,
    lookup, and friend request functionality using custom data structures.
*/

#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <string>
#include "Player.h"

// Define the size of our hash table
#define HASH_TABLE_SIZE 100

// Define a key-value pair for the hash table
struct HashNode {
    std::string key;      // Username
    int value;            // Index in players array
    HashNode* next;       // For handling collisions with chaining

    HashNode(const std::string& k, int v) : key(k), value(v), next(nullptr) {}
};

// Define our simple hash table implementation
class HashMap {
private:
    HashNode* table[HASH_TABLE_SIZE];

    // Simple hash function
    int hashFunction(const std::string& key) {
        int hash = 0;
        for (char c : key) {
            hash = (hash * 31 + c) % HASH_TABLE_SIZE;
        }
        return hash;
    }

public:
    // Constructor
    HashMap() {
        // Initialize all buckets to nullptr
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    // Destructor
    ~HashMap() {
        // Clean up all nodes
        for (int i = 0; i < HASH_TABLE_SIZE; i++) {
            HashNode* current = table[i];
            while (current != nullptr) {
                HashNode* next = current->next;
                delete current;
                current = next;
            }
        }
    }

    // Insert a key-value pair
    void put(const std::string& key, int value) {
        int hash = hashFunction(key);

        // Check if key already exists
        HashNode* current = table[hash];
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value; // Update value
                return;
            }
            current = current->next;
        }

        // Key doesn't exist, create new node
        HashNode* newNode = new HashNode(key, value);
        newNode->next = table[hash];
        table[hash] = newNode;
    }

    // Get value by key
    int get(const std::string& key) {
        int hash = hashFunction(key);

        // Search for key
        HashNode* current = table[hash];
        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }

        // Key not found
        return -1;
    }

    // Check if key exists
    bool containsKey(const std::string& key) {
        return get(key) != -1;
    }

    // Remove a key
    void remove(const std::string& key) {
        int hash = hashFunction(key);

        // Handle case where key is at head
        if (table[hash] != nullptr && table[hash]->key == key) {
            HashNode* temp = table[hash];
            table[hash] = table[hash]->next;
            delete temp;
            return;
        }

        // Search for key
        HashNode* current = table[hash];
        while (current != nullptr && current->next != nullptr) {
            if (current->next->key == key) {
                HashNode* temp = current->next;
                current->next = current->next->next;
                delete temp;
                return;
            }
            current = current->next;
        }
    }
};

// Player Manager class
class PlayerManager {
private:
    // Array to store players
    Player** players;
    int maxPlayers;
    int playerCount;

    // Hash map for fast lookup by username
    HashMap playerMap;

public:
    // Constructor and destructor
    PlayerManager(int maxSize = 100);
    ~PlayerManager();

    // Player registration and lookup
    bool registerPlayer(const std::string& username);
    Player* getPlayerByUsername(const std::string& username);

    // Friend request functionality
    bool sendFriendRequest(const std::string& senderUsername, const std::string& receiverUsername);
    bool acceptFriendRequest(const std::string& receiverUsername, const std::string& senderUsername);
    bool rejectFriendRequest(const std::string& receiverUsername, const std::string& senderUsername);

    // Friend list operations
    void getFriendList(const std::string& username, std::string* result, int& count);
    void getIncomingRequests(const std::string& username, std::string* result, int& count);
    void getOutgoingRequests(const std::string& username, std::string* result, int& count);
};

#endif // PLAYERMANAGER_H#pragma once
