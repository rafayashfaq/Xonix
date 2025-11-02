/*
    Game.h
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - DSA Project

    Description:
    This file contains the Game class which encapsulates the core game logic.
*/

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Player.h"
#include "Enemy.h"
#include "GameSave.h"

using namespace sf;

// Game states
enum class GameState {
    PLAYING,
    GAME_OVER,
    WIN,
    PAUSED
};

class Game {
private:
    // Game dimensions
    static const int M = 25;
    static const int N = 40;
    static const int ts = 18; // tile size

    // Game grid
    int grid[M][N];

    // Game state
    bool isRunning;
    int score;
    int highScore;
    bool isMultiplayer;
    bool returnToMenu;
    GameState gameState;
    std::string winReason;  // Store the reason for winning
    std::string currentUsername;  // Current player's username

    // SFML objects
    RenderWindow* window;
    Texture tileTexture;
    Texture gameoverTexture;
    Texture enemyTexture;
    Sprite tileSprite;
    Sprite gameoverSprite;
    Sprite enemySprite;
    Font font;

    // Game entities
    static const int maxEnemies = 10;
    int enemyCount;
    Enemy enemies[maxEnemies];
    Player player;
    Player player2;  // Second player for multiplayer

    // Game clock
    Clock gameClock;
    float timer;
    float delay;

    int bonusCounter;       // Tracks how many times bonus points were earned
    int bonusThreshold;     // Current threshold for bonus points (starts at 10)
    int bonusMultiplier;    // Current multiplier for bonus points (starts at 2)

    // Save game functionality
    GameSave* gameSave;
    bool showSaveMenu;
    bool showLoadMenu;
    LinkedList<std::string> savesList;
    int selectedSaveIndex;

    // Notification message
    std::string notificationMessage;
    float notificationTimer;

    // Helper functions
    void initializeGrid();
    void resetGame();
    void processInput();
    void update(float deltaTime);
    void draw();
    void fillArea(int y, int x, int tempGrid[][N], bool& containsEnemy);
    void drop(int y, int x);
    void updateScore(int points);
    void checkCollisions();
    void updatePlayer1();
    void updatePlayer2();
    void updateEnemy(Enemy& enemy);
    bool isTrailConnectedToBorder();
    void activatePowerup();

    // Save/Load menu functions
    void drawSaveMenu();
    void drawLoadMenu();
    void processMenuInput(Event& event);
    void processPauseMenuInput(Event& event);

public:
    // Constructor
    Game();

    // Destructor
    ~Game();

    // Main game functions
    void initialize();
    void initializeMultiplayer(const std::string& player1Name, const std::string& player2Name);
    void run();
    void pause();
    void resume();
    void end();
    int getFinalScore() const;  // Add this method

    // Save/Load methods
    bool saveGame(const std::string& username);
    bool loadGame(const std::string& saveID, const std::string& username);
    LinkedList<std::string> getSaves(const std::string& username);
    void setCurrentUsername(const std::string& username);

    // Getters
    int getScore() const;
    int getHighScore() const;
    bool getIsRunning() const;
    RenderWindow* getWindow();
    bool shouldReturnToMenu() const;

    // Add a new getter for gameSave
    GameSave* getGameSave() const { return gameSave; }
};

#endif // GAME_H