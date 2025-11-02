/*
    GameSave.cpp
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - Data Structures and Algorithms Project

    Description:
    This file contains the implementation of the GameSave class methods.
*/

#include "GameSave.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <direct.h> // For _mkdir
#include <sys/stat.h> // For stat

// Windows API includes - isolate to avoid conflicts
#define WIN32_LEAN_AND_MEAN
#include <windows.h> // For FindFirstFile, FindNextFile, FindClose
#undef min
#undef max
#undef BYTE

// Constructor
GameSave::GameSave() {
    // Initialize with default saves directory
    savesDirectory = "saves/";

    // Create saves directory if it doesn't exist
    struct stat info;
    if (stat(savesDirectory.c_str(), &info) != 0) {
        _mkdir(savesDirectory.c_str());
    }
}

// Generate a unique save ID based on timestamp and random value
std::string GameSave::generateSaveID() {
    // Get current time
    auto now = std::time(nullptr);
    struct tm timeInfo;
    localtime_s(&timeInfo, &now);
    auto tm = &timeInfo;

    // Format as YYYYMMDD_HHMMSS
    std::stringstream ss;
    ss << std::put_time(tm, "%Y%m%d_%H%M%S_");

    // Add random suffix to avoid collisions
    ss << rand() % 1000;

    return ss.str();
}

// Save the current game state
bool GameSave::saveGame(int grid[][40], int width, int height, const std::string& username, int score,
    const Player& player, const Enemy enemies[], int enemyCount) {
    // Generate a unique save ID
    std::string saveID = generateSaveID();

    // Create a SaveGameState object
    gameState = SaveGameState(saveID, username, score, width, height);

    // Clear and populate tiles list with current grid data
    tiles.clear();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] != 0) { // Only save non-empty tiles
                tiles.addLast(TileNode(j, i, grid[i][j]));
            }
        }
    }

    // Create the full path for the save file
    std::string filePath = savesDirectory + saveID + ".sav";

    // Open file for writing in binary mode
    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Error: Could not open file for writing: " << filePath << std::endl;
        return false;
    }

    // Write game state metadata
    file.write(saveID.c_str(), saveID.length() + 1); // +1 for null terminator
    file.write(username.c_str(), username.length() + 1);
    file.write(reinterpret_cast<const char*>(&score), sizeof(score));
    file.write(reinterpret_cast<const char*>(&gameState.timestamp), sizeof(gameState.timestamp));
    file.write(reinterpret_cast<const char*>(&width), sizeof(width));
    file.write(reinterpret_cast<const char*>(&height), sizeof(height));

    // Write player data
    int playerX = player.getX();
    int playerY = player.getY();
    int playerDX = player.getDX();
    int playerDY = player.getDY();
    int playerScore = player.getScore();
    int playerPowerups = player.getPowerups();
    file.write(reinterpret_cast<const char*>(&playerX), sizeof(playerX));
    file.write(reinterpret_cast<const char*>(&playerY), sizeof(playerY));
    file.write(reinterpret_cast<const char*>(&playerDX), sizeof(playerDX));
    file.write(reinterpret_cast<const char*>(&playerDY), sizeof(playerDY));
    file.write(reinterpret_cast<const char*>(&playerScore), sizeof(playerScore));
    file.write(reinterpret_cast<const char*>(&playerPowerups), sizeof(playerPowerups));

    // Write enemy count and enemy data
    file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
    for (int i = 0; i < enemyCount; i++) {
        int enemyX = enemies[i].getX();
        int enemyY = enemies[i].getY();
        bool isFrozen = enemies[i].isFrozen();
        file.write(reinterpret_cast<const char*>(&enemyX), sizeof(enemyX));
        file.write(reinterpret_cast<const char*>(&enemyY), sizeof(enemyY));
        file.write(reinterpret_cast<const char*>(&isFrozen), sizeof(isFrozen));
    }

    // Write tile data
    int tileCount = tiles.getSize();
    file.write(reinterpret_cast<const char*>(&tileCount), sizeof(tileCount));

    // Traverse the linked list and write each tile
    Node<TileNode>* current = tiles.getHead();
    while (current != nullptr) {
        file.write(reinterpret_cast<const char*>(&current->data.x), sizeof(current->data.x));
        file.write(reinterpret_cast<const char*>(&current->data.y), sizeof(current->data.y));
        file.write(reinterpret_cast<const char*>(&current->data.type), sizeof(current->data.type));
        current = current->next;
    }

    file.close();
    std::cout << "Game saved successfully! Save ID: " << saveID << std::endl;
    return true;
}

// Load a saved game state
bool GameSave::loadGame(int grid[][40], int& width, int& height, std::string& username, int& score,
    Player& player, Enemy enemies[], int& enemyCount, const std::string& saveID) {
    // Create the full path for the save file
    std::string filePath = savesDirectory + saveID + ".sav";

    // Open file for reading in binary mode
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Error: Could not open save file: " << filePath << std::endl;
        return false;
    }

    // Read save ID (just to verify it matches)
    char buffer[256];
    file.getline(buffer, 256, '\0');
    std::string readSaveID(buffer);
    if (readSaveID != saveID) {
        std::cout << "Error: Save ID mismatch. Expected: " << saveID << ", Got: " << readSaveID << std::endl;
        file.close();
        return false;
    }

    // Read username
    file.getline(buffer, 256, '\0');
    username = std::string(buffer);

    // Read score, timestamp, width, height
    file.read(reinterpret_cast<char*>(&score), sizeof(score));
    file.read(reinterpret_cast<char*>(&gameState.timestamp), sizeof(gameState.timestamp));
    file.read(reinterpret_cast<char*>(&width), sizeof(width));
    file.read(reinterpret_cast<char*>(&height), sizeof(height));

    // Read player data
    int playerX, playerY, playerDX, playerDY, playerScore, playerPowerups;
    file.read(reinterpret_cast<char*>(&playerX), sizeof(playerX));
    file.read(reinterpret_cast<char*>(&playerY), sizeof(playerY));
    file.read(reinterpret_cast<char*>(&playerDX), sizeof(playerDX));
    file.read(reinterpret_cast<char*>(&playerDY), sizeof(playerDY));
    file.read(reinterpret_cast<char*>(&playerScore), sizeof(playerScore));
    file.read(reinterpret_cast<char*>(&playerPowerups), sizeof(playerPowerups));

    // Update player object
    player.setX(playerX);
    player.setY(playerY);
    player.setDirection(playerDX, playerDY);
    // Set score and powerups (you may need to add these methods to the Player class)

    // Read enemy count and enemy data
    file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));

    // Make sure enemy count doesn't exceed maximum
    int maxEnemies = 10; // Should match Game::maxEnemies
    if (enemyCount > maxEnemies) {
        std::cout << "Warning: Saved enemy count exceeds maximum. Truncating." << std::endl;
        enemyCount = maxEnemies;
    }

    // Initialize enemies array with loaded data
    for (int i = 0; i < enemyCount; i++) {
        int enemyX, enemyY;
        bool isFrozen;
        file.read(reinterpret_cast<char*>(&enemyX), sizeof(enemyX));
        file.read(reinterpret_cast<char*>(&enemyY), sizeof(enemyY));
        file.read(reinterpret_cast<char*>(&isFrozen), sizeof(isFrozen));

        // Create a new enemy with the loaded data
        // You may need to adjust this based on your Enemy class implementation
        enemies[i] = Enemy(); // Create a new enemy
        // Set enemy properties if your Enemy class has appropriate setters

        // If Enemy doesn't have setters, you may need to modify Enemy class
        // or use a different approach to restore enemy state
    }

    // Clear the grid first
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i][j] = 0;
        }
    }

    // Read tile data
    int tileCount;
    file.read(reinterpret_cast<char*>(&tileCount), sizeof(tileCount));

    // Read each tile and update grid
    for (int i = 0; i < tileCount; i++) {
        int tileX, tileY, tileType;
        file.read(reinterpret_cast<char*>(&tileX), sizeof(tileX));
        file.read(reinterpret_cast<char*>(&tileY), sizeof(tileY));
        file.read(reinterpret_cast<char*>(&tileType), sizeof(tileType));

        // Update the grid with the tile
        if (tileX >= 0 && tileX < width && tileY >= 0 && tileY < height) {
            grid[tileY][tileX] = tileType;
        }
    }

    file.close();
    std::cout << "Game loaded successfully! Save ID: " << saveID << std::endl;
    return true;
}

// List all available saves for a player
LinkedList<std::string> GameSave::listSaves(const std::string& username) {
    LinkedList<std::string> saveList;

    // Create directories if they don't exist
    struct stat info;
    if (stat(savesDirectory.c_str(), &info) != 0) {
        _mkdir(savesDirectory.c_str());
    }

    // Using Windows API to scan directory
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind = FindFirstFileA((savesDirectory + "*.sav").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // Get the filename without the .sav extension
            std::string filename = findFileData.cFileName;
            std::string saveID = filename.substr(0, filename.length() - 4);

            // Open the file to check if username matches
            std::string filePath = savesDirectory + filename;
            std::ifstream file(filePath, std::ios::binary);

            if (file.is_open()) {
                // Read save ID (just to verify)
                char buffer[256];
                file.getline(buffer, 256, '\0');

                // Read username
                file.getline(buffer, 256, '\0');
                std::string fileUsername(buffer);

                // If username matches, add to list
                if (fileUsername == username) {
                    saveList.addLast(saveID);
                    std::cout << "Found save for " << username << ": " << saveID << std::endl;
                }

                file.close();
            }

        } while (FindNextFileA(hFind, &findFileData) != 0);

        FindClose(hFind);
    }

    std::cout << "Found " << saveList.getSize() << " saves for player: " << username << std::endl;
    return saveList;
}

// Delete a saved game
bool GameSave::deleteSave(const std::string& saveID) {
    std::string filePath = savesDirectory + saveID + ".sav";

    // Try to delete the file
    if (remove(filePath.c_str()) != 0) {
        std::cout << "Error: Could not delete save file: " << filePath << std::endl;
        return false;
    }

    std::cout << "Save deleted successfully: " << saveID << std::endl;
    return true;
}

// Get information about a save
SaveGameState GameSave::getSaveInfo(const std::string& saveID) {
    std::string filePath = savesDirectory + saveID + ".sav";

    // Open file for reading in binary mode
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Error: Could not open save file: " << filePath << std::endl;
        return SaveGameState();
    }

    // Read save ID
    char buffer[256];
    file.getline(buffer, 256, '\0');
    std::string readSaveID(buffer);

    // Read username
    file.getline(buffer, 256, '\0');
    std::string username(buffer);

    // Read score, timestamp, width, height
    int score, width, height;
    time_t timestamp;
    file.read(reinterpret_cast<char*>(&score), sizeof(score));
    file.read(reinterpret_cast<char*>(&timestamp), sizeof(timestamp));
    file.read(reinterpret_cast<char*>(&width), sizeof(width));
    file.read(reinterpret_cast<char*>(&height), sizeof(height));

    file.close();

    // Create and return a SaveGameState object with the read data
    SaveGameState state;
    state.saveID = readSaveID;
    state.playerUsername = username;
    state.score = score;
    state.timestamp = timestamp;
    state.gridWidth = width;
    state.gridHeight = height;

    return state;
}
