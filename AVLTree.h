/*
    AVLTree.h
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - Data Structures and Algorithms Project

    Description:
    This file contains the implementation of an AVL Tree data structure.
    The AVL Tree is used for storing and organizing game themes in the inventory system.
    It provides efficient searching, insertion, and traversal operations.
*/

#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <cstring>
using namespace std;

// Structure to store theme information in the AVL tree
struct ThemeInfo {
    int themeID;            // Unique identifier for the theme
    char themeName[50];     // Name of the theme
    char description[200];  // Description of the theme
    int colorCode;          // Color code representing the theme's appearance

    // Default constructor
    ThemeInfo() {
        themeID = -1;
        themeName[0] = '\0';
        description[0] = '\0';
        colorCode = 0;
    }

    // Parameterized constructor
    ThemeInfo(int id, const char* name, const char* desc, int color) {
        themeID = id;
        strncpy(themeName, name, 49);
        themeName[49] = '\0';  // Ensure null termination
        strncpy(description, desc, 199);
        description[199] = '\0';  // Ensure null termination
        colorCode = color;
    }
};

// AVL Tree node structure
struct AVLNode {
    ThemeInfo theme;      // Theme data
    AVLNode* left;        // Left child
    AVLNode* right;       // Right child
    int height;           // Height of the node for balancing

    // Constructor
    AVLNode(const ThemeInfo& themeData) {
        theme = themeData;
        left = right = nullptr;
        height = 1;  // New node is initially at height 1
    }
};

class AVLTree {
private:
    AVLNode* root;  // Root of the AVL tree

    // Get the height of a node (0 if nullptr)
    int getHeight(AVLNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return node->height;
    }

    // Calculate the balance factor of a node
    int getBalanceFactor(AVLNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return getHeight(node->left) - getHeight(node->right);
    }

    // Update the height of a node based on its children's heights
    void updateHeight(AVLNode* node) {
        if (node == nullptr) {
            return;
        }
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }

    // Right rotate subtree rooted with y
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        updateHeight(y);
        updateHeight(x);

        // Return new root
        return x;
    }

    // Left rotate subtree rooted with x
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        updateHeight(x);
        updateHeight(y);

        // Return new root
        return y;
    }

    // Recursive function to insert a theme in the subtree rooted with node
    AVLNode* insertNode(AVLNode* node, const ThemeInfo& theme) {
        // Normal BST insertion
        if (node == nullptr) {
            return new AVLNode(theme);
        }

        if (theme.themeID < node->theme.themeID) {
            node->left = insertNode(node->left, theme);
        }
        else if (theme.themeID > node->theme.themeID) {
            node->right = insertNode(node->right, theme);
        }
        else {
            // Duplicate themeID not allowed, update instead
            node->theme = theme;
            return node;
        }

        // Update height of this node
        updateHeight(node);

        // Get the balance factor to check if this node became unbalanced
        int balance = getBalanceFactor(node);

        // Left Left Case
        if (balance > 1 && theme.themeID < node->left->theme.themeID) {
            return rightRotate(node);
        }

        // Right Right Case
        if (balance < -1 && theme.themeID > node->right->theme.themeID) {
            return leftRotate(node);
        }

        // Left Right Case
        if (balance > 1 && theme.themeID > node->left->theme.themeID) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && theme.themeID < node->right->theme.themeID) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        // Return the unchanged node pointer
        return node;
    }

    // Recursive function to search for a theme by ID
    AVLNode* searchNode(AVLNode* node, int themeID) {
        if (node == nullptr || node->theme.themeID == themeID) {
            return node;
        }

        if (themeID < node->theme.themeID) {
            return searchNode(node->left, themeID);
        }
        else {
            return searchNode(node->right, themeID);
        }
    }

    // Recursive function to search for a theme by name
    AVLNode* searchNodeByName(AVLNode* node, const char* themeName) {
        if (node == nullptr) {
            return nullptr;
        }

        int cmp = strcmp(themeName, node->theme.themeName);
        if (cmp == 0) {
            return node;
        }
        else if (cmp < 0) {
            return searchNodeByName(node->left, themeName);
        }
        else {
            return searchNodeByName(node->right, themeName);
        }
    }

    // Find the node with minimum value in a subtree
    AVLNode* findMinNode(AVLNode* node) {
        AVLNode* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    // Recursive function to delete a node with given themeID
    AVLNode* deleteNode(AVLNode* root, int themeID) {
        // Standard BST delete
        if (root == nullptr) {
            return root;
        }

        if (themeID < root->theme.themeID) {
            root->left = deleteNode(root->left, themeID);
        }
        else if (themeID > root->theme.themeID) {
            root->right = deleteNode(root->right, themeID);
        }
        else {
            // Node to be deleted found

            // Node with only one child or no child
            if (root->left == nullptr) {
                AVLNode* temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr) {
                AVLNode* temp = root->left;
                delete root;
                return temp;
            }

            // Node with two children
            AVLNode* temp = findMinNode(root->right);
            root->theme = temp->theme;
            root->right = deleteNode(root->right, temp->theme.themeID);
        }

        // If the tree had only one node
        if (root == nullptr) {
            return root;
        }

        // Update height
        updateHeight(root);

        // Check balance factor
        int balance = getBalanceFactor(root);

        // Left Left Case
        if (balance > 1 && getBalanceFactor(root->left) >= 0) {
            return rightRotate(root);
        }

        // Left Right Case
        if (balance > 1 && getBalanceFactor(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // Right Right Case
        if (balance < -1 && getBalanceFactor(root->right) <= 0) {
            return leftRotate(root);
        }

        // Right Left Case
        if (balance < -1 && getBalanceFactor(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    // Recursive function for in-order traversal
    void inOrderTraversal(AVLNode* node) {
        if (node == nullptr) {
            return;
        }
        inOrderTraversal(node->left);
        cout << "Theme ID: " << node->theme.themeID << ", Name: " << node->theme.themeName << endl;
        inOrderTraversal(node->right);
    }

    // Recursively clean up nodes
    void deleteTree(AVLNode* node) {
        if (node == nullptr) {
            return;
        }

        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    // Constructor
    AVLTree() {
        root = nullptr;
    }

    // Destructor
    ~AVLTree() {
        deleteTree(root);
    }

    // Insert a new theme
    void insert(const ThemeInfo& theme) {
        root = insertNode(root, theme);
    }

    // Search for a theme by ID
    ThemeInfo search(int themeID) {
        AVLNode* node = searchNode(root, themeID);
        if (node != nullptr) {
            return node->theme;
        }
        else {
            return ThemeInfo();  // Return empty theme if not found
        }
    }

    // Search for a theme by name
    ThemeInfo searchByName(const char* themeName) {
        AVLNode* node = searchNodeByName(root, themeName);
        if (node != nullptr) {
            return node->theme;
        }
        else {
            return ThemeInfo();  // Return empty theme if not found
        }
    }

    // Delete a theme by ID
    void remove(int themeID) {
        root = deleteNode(root, themeID);
    }

    // Display all themes in sorted order (by ID)
    void displayInOrder() {
        cout << "Themes in ID order:" << endl;
        inOrderTraversal(root);
    }

    // Check if a theme exists by ID
    bool exists(int themeID) {
        return searchNode(root, themeID) != nullptr;
    }

    // Check if a theme exists by name
    bool existsByName(const char* themeName) {
        return searchNodeByName(root, themeName) != nullptr;
    }

    // Get the number of themes (count nodes)
    int count() {
        return countNodes(root);
    }

private:
    // Helper function to count nodes in the tree
    int countNodes(AVLNode* node) {
        if (node == nullptr) {
            return 0;
        }
        return 1 + countNodes(node->left) + countNodes(node->right);
    }
};

#endif // AVLTREE_H