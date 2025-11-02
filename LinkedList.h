/*
    LinkedList.h
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - Data Structures and Algorithms Project

    Description:
    This file contains the implementation of a Linked List data structure.
    The Linked List is used for storing friends lists for each player
    and for tracking game tiles in save game functionality.
*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
using namespace std;

// Structure for a generic linked list node
// We make it template-based so it can be used for both friend IDs and game tiles
template <typename T>
struct Node {
    T data;          // Data stored in the node (can be friendID or tile coordinates)
    Node<T>* next;   // Pointer to the next node

    // Constructor
    Node(T value) {
        data = value;
        next = nullptr;
    }
};

// The main linked list class
template <typename T>
class LinkedList {
private:
    Node<T>* head;  // Pointer to the first node in the list
    int size;       // Number of nodes in the list

public:
    // Constructor
    LinkedList() {
        head = nullptr;
        size = 0;
    }

    // Destructor to free memory
    ~LinkedList() {
        clear();
    }

    // Add a new node at the beginning of the list
    void addFirst(T value) {
        Node<T>* newNode = new Node<T>(value);
        newNode->next = head;
        head = newNode;
        size++;
    }

    // Add a new node at the end of the list
    void addLast(T value) {
        Node<T>* newNode = new Node<T>(value);

        // If the list is empty, the new node becomes the head
        if (head == nullptr) {
            head = newNode;
        }
        else {
            // Find the last node and add the new node after it
            Node<T>* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }

        size++;
    }

    // Remove the first occurrence of a value from the list
    bool remove(T value) {
        // If the list is empty, nothing to remove
        if (head == nullptr) {
            return false;
        }

        // If the head is the node to remove
        if (head->data == value) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            size--;
            return true;
        }

        // Search for the node to remove
        Node<T>* current = head;
        while (current->next != nullptr && current->next->data != value) {
            current = current->next;
        }

        // If the value was found
        if (current->next != nullptr) {
            Node<T>* temp = current->next;
            current->next = current->next->next;
            delete temp;
            size--;
            return true;
        }

        return false;
    }

    // Check if the list contains a specific value
    bool contains(T value) {
        Node<T>* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Get the number of elements in the list
    int getSize() {
        return size;
    }

    // Check if the list is empty
    bool isEmpty() {
        return head == nullptr;
    }

    bool contains(T value) const {
        // Start from the head node
        Node<T>* current = head;

        // Traverse through the list
        while (current != nullptr) {
            // If the current node's data matches the value, return true
            if (current->data == value) {
                return true;
            }
            // Move to the next node
            current = current->next;
        }

        // If we got here, the value was not found
        return false;
    }

    // Clear the entire list (delete all nodes)
    void clear() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }

    // Display the list contents (for debugging)
    void display() {
        Node<T>* current = head;
        cout << "List contents: ";
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    // Get the head node of the list for traversal
    Node<T>* getHead() const {
        return head;
    }

    // Get the data at a specific index
    T get(int index) {
        if (index < 0 || index >= size) {
            cout << "Index out of bounds" << endl;
            // Return a default value (may need adjusting based on T)
            return T();
        }

        Node<T>* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }

        return current->data;
    }
};

// Define some specific structs for different use cases

// For friends list
struct FriendNode {
    int friendID;

    FriendNode() {
        friendID = -1;
    }

    FriendNode(int id) {
        friendID = id;
    }

    // For comparison in contains() method
    bool operator==(const FriendNode& other) const {
        return friendID == other.friendID;
    }
};

// For game tiles
struct TileNode {
    int x, y;
    int type; // 0: empty, 1: border, 2: player trail

    TileNode() {
        x = y = type = 0;
    }

    TileNode(int xPos, int yPos, int tileType) {
        x = xPos;
        y = yPos;
        type = tileType;
    }

    // For comparison in contains() method
    bool operator==(const TileNode& other) const {
        return x == other.x && y == other.y && type == other.type;
    }
};

#endif // LINKEDLIST_H