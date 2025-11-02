/*
    GameSave.h
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - Data Structures and Algorithms Project

    Description:
    This file contains the GameSave class which handles saving and loading game states
    using a LinkedList to store tile information.
*/

#ifndef GAMESAVE_H
#define GAMESAVE_H

// Prevent COM interface conflicts with BYTE
#ifndef _NO_COM_INTERFACE_BYTE_OVERRIDE
#define _NO_COM_INTERFACE_BYTE_OVERRIDE
#endif

// Prevent conflicts with Windows.h BYTE definition
#ifdef BYTE
#undef BYTE
#endif

// Additional protection for COM interfaces
#ifdef byte
#undef byte
#endif

#include <SFML/Graphics.hpp>
#include <string>
#include <ctime>
#include <fstream>
#include "LinkedList.h"
#include "Player.h"
#include "Enemy.h"

// Structure to store game state information
struct SaveGameState {
    std::string saveID;
    std::string playerUsername;
    int score;
    time_t timestamp;
    int gridWidth;
    int gridHeight;

    // Default constructor
    SaveGameState() :
        saveID(""),
        playerUsername(""),
        score(0),
        timestamp(0),
        gridWidth(0),
        gridHeight(0) {
    }

    // Parameterized constructor
    SaveGameState(const std::string& id, const std::string& username, int s, int width, int height) :
        saveID(id),
        playerUsername(username),
        score(s),
        timestamp(time(nullptr)),
        gridWidth(width),
        gridHeight(height) {
    }
};

class GameSave {
private:
    SaveGameState gameState;
    LinkedList<TileNode> tiles;

    // Helper methods for generating saveID
    std::string generateSaveID();

    // Path to saves directory
    std::string savesDirectory;

public:
    // Constructor
    GameSave();

    // Save the current game state
    bool saveGame(int grid[][40], int width, int height, const std::string& username, int score,
        const Player& player, const Enemy enemies[], int enemyCount);

    // Load a saved game state
    bool loadGame(int grid[][40], int& width, int& height, std::string& username, int& score,
        Player& player, Enemy enemies[], int& enemyCount, const std::string& saveID);

    // List all available saves for a player
    LinkedList<std::string> listSaves(const std::string& username);

    // Delete a saved game
    bool deleteSave(const std::string& saveID);

    // Get information about a save
    SaveGameState getSaveInfo(const std::string& saveID);
};

#endif // GAMESAVE_H 