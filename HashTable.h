/*
    HashTable.h
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - DSA Project

    Description:
    This file contains the HashTable class implementation for storing user credentials.
*/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <iostream>

class HashTable {
public:
    static const int TABLE_SIZE = 1000;  // Size of the hash table

    // Make Auth class a friend to access private members
    friend class Auth;

    // Node structure definition
    struct Node {
        std::string key;
        std::string value;
        Node* next;

        Node(const std::string& k, const std::string& v) : key(k), value(v), next(nullptr) {}
    };

private:
    Node** table;  // Array of pointers to Node

    // Hash function
    int hash(const std::string& key) const {
        int hash = 0;
        for (char c : key) {
            hash = (hash * 31 + c) % TABLE_SIZE;
        }
        return hash;
    }

public:
    // Constructor and Destructor
    HashTable() {
        table = new Node * [TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    ~HashTable() {
        clear();
        delete[] table;
    }

    // Core operations
    void insert(const std::string& key, const std::string& value) {
        int index = hash(key);
        std::cout << "Inserting " << key << " at index " << index << std::endl;

        // Check if key already exists
        Node* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }

        // Create new node
        Node* newNode = new Node(key, value);
        newNode->next = table[index];
        table[index] = newNode;
    }

    std::string* get(const std::string& key) const {
        int index = hash(key);
        std::cout << "Getting " << key << " from index " << index << std::endl;

        Node* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;
    }

    void remove(const std::string& key) {
        int index = hash(key);
        std::cout << "Removing " << key << " from index " << index << std::endl;

        Node* current = table[index];
        Node* prev = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[index] = current->next;
                }
                else {
                    prev->next = current->next;
                }
                delete current;
                return;
            }
            prev = current;
            current = current->next;
        }
    }

    bool contains(const std::string& key) const {
        return get(key) != nullptr;
    }

    void clear() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
            table[i] = nullptr;
        }
    }
};

#endif // HASHTABLE_H