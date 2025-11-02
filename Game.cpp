/*
    Game.cpp
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - Data Structures and Algorithms Project
*/

#include "Game.h"
#include "Queue.h"
#include "Leaderboard.h"
#include <iostream>
#include <direct.h> // For _mkdir function
using namespace std;

// Constructor
Game::Game() {
    isRunning = false;
    score = 0;
    highScore = 0;
    window = nullptr;
    enemyCount = 4;
    timer = 0;
    delay = 0.07f;
    gameState = GameState::PLAYING;
    isMultiplayer = false;
    returnToMenu = false;
    currentUsername = "";

    // Save game initialization
    gameSave = new GameSave();
    showSaveMenu = false;
    showLoadMenu = false;
    selectedSaveIndex = 0;

    // Notification initialization
    notificationMessage = "";
    notificationTimer = 0.0f;

    // Points system initialization
    bonusCounter = 0;
    bonusThreshold = 10;
    bonusMultiplier = 2;
}


// Destructor
Game::~Game() {
    if (window != nullptr) {
        delete window;
    }

    if (gameSave != nullptr) {
        delete gameSave;
    }
}

void Game::initialize() {
    // Create window if it doesn't exist
    if (window == nullptr) {
        window = new RenderWindow(VideoMode(N * ts, M * ts), "Xonix Game!");
        window->setFramerateLimit(60);
    }

    // Print username info
    std::cout << "Game initialized with username: '" << currentUsername << "'" << std::endl;
    if (currentUsername.empty()) {
        std::cout << "Warning: No username set, save/load functionality may not work properly." << std::endl;
    }

    // Load textures
    if (!tileTexture.loadFromFile("images/tiles.png") ||
        !gameoverTexture.loadFromFile("images/gameover.png") ||
        !enemyTexture.loadFromFile("images/enemy.png")) {
        // Handle error loading textures
        std::cout << "Error: Failed to load game textures" << std::endl;
        return;
    }

    // Load font
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        if (!font.loadFromFile("C:/Windows/Fonts/calibri.ttf")) {
            if (!font.loadFromFile("C:/Windows/Fonts/consolas.ttf")) {
                std::cout << "Error: Failed to load font" << std::endl;
            }
        }
    }

    // Set up sprites
    tileSprite.setTexture(tileTexture);
    gameoverSprite.setTexture(gameoverTexture);
    gameoverSprite.setPosition(100, 100);
    enemySprite.setTexture(enemyTexture);
    enemySprite.setOrigin(20, 20);

    // Load high score for current user from leaderboard
    if (!currentUsername.empty()) {
        // Make sure we have a valid leaderboard to read from
        static Leaderboard leaderboard("leaderboard.dat");

        // Get top scores from leaderboard
        int count;
        PlayerScore* scores = leaderboard.getTopScores(count);

        // Find the highest score for the current user
        for (int i = 0; i < count; i++) {
            if (scores[i].username == currentUsername && scores[i].score > highScore) {
                highScore = scores[i].score;
                std::cout << "Loaded high score for " << currentUsername << ": " << highScore << std::endl;
            }
        }

        // Also check the all-time high score
        if (count > 0) {
            int allTimeHigh = scores[0].score;
            if (allTimeHigh > highScore) {
                highScore = allTimeHigh;
                std::cout << "Using all-time high score: " << highScore << std::endl;
            }
        }

        // Clean up
        delete[] scores;
    }

    // Initialize game state
    initializeGrid();
    resetGame();
    gameState = GameState::PLAYING;
    isRunning = true;
}

void Game::initializeGrid() {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1) {
                grid[i][j] = 1;  // Border
            }
            else {
                grid[i][j] = 0;  // Empty
            }
        }
    }
}

void Game::resetGame() {
    player.reset(10, 0);
    score = 0;
    gameState = GameState::PLAYING;
    returnToMenu = false;

    // Reset points system
    bonusCounter = 0;
    bonusThreshold = 10;
    bonusMultiplier = 2;

    // Reset enemies
    for (int i = 0; i < enemyCount; i++) {
        enemies[i] = Enemy();
    }

    isRunning = true;

    // Note: We deliberately don't reset highScore here to preserve it between games
}

void Game::processInput() {
    // Handle window close and escape key
    Event event;

    // Debug all key presses
    if (Keyboard::isKeyPressed(Keyboard::F5)) {
        std::cout << "F5 key is currently pressed (direct check)" << std::endl;
    }

    if (Keyboard::isKeyPressed(Keyboard::F9)) {
        std::cout << "F9 key is currently pressed (direct check)" << std::endl;
    }

    while (window->pollEvent(event)) {
        // Process save/load menu input if active
        if (showSaveMenu || showLoadMenu) {
            processMenuInput(event);
            continue;
        }

        if (event.type == Event::Closed) {
            window->close();
            isRunning = false;
            return;
        }
        if (event.type == Event::KeyPressed) {
            std::cout << "Key pressed: " << event.key.code << std::endl;

            // Handle ESC key for different game states
            if (event.key.code == Keyboard::Escape) {
                if (gameState == GameState::WIN || gameState == GameState::GAME_OVER) {
                    // Set flag to return to menu instead of closing or resetting
                    returnToMenu = true;
                    isRunning = false;
                    return;
                }
                else if (gameState == GameState::PLAYING) {
                    // During normal gameplay, ESC pauses the game
                    pause();
                }
                else if (gameState == GameState::PAUSED) {
                    // Resume game if paused
                    resume();
                }
            }
            // P key can also be used for pause/resume
            else if (event.key.code == Keyboard::P) {
                if (gameState == GameState::PLAYING) {
                    // Pause the game
                    pause();
                }
                else if (gameState == GameState::PAUSED) {
                    // Resume game if paused
                    resume();
                }
                // Only activate power-up in PLAYING state when powerups are available
                else if (gameState == GameState::PLAYING && player.getPowerups() > 0) {
                    activatePowerup();
                }
            }
            // Add F5 key handling to save
            else if (event.key.code == Keyboard::F5 && gameState == GameState::PLAYING) {
                // Directly save the game instead of showing a menu
                std::cout << "F5 pressed - Trying to save game with username: '" << currentUsername << "'" << std::endl;

                // Check if username is empty
                if (currentUsername.empty()) {
                    std::cout << "WARNING: Username is empty, using 'default_user'" << std::endl;
                    currentUsername = "default_user";
                }

                // Make sure gameSave is initialized
                if (gameSave == nullptr) {
                    std::cout << "Initializing GameSave object" << std::endl;
                    gameSave = new GameSave();
                }

                if (saveGame(currentUsername)) {
                    // Show a temporary saved message
                    std::cout << "Game saved successfully!" << std::endl;
                    notificationMessage = "Game saved successfully!";
                    notificationTimer = 3.0f; // Show for 3 seconds
                }
                else {
                    std::cout << "Game save failed!" << std::endl;
                    notificationMessage = "Failed to save game!";
                    notificationTimer = 3.0f; // Show for 3 seconds
                }
            }
            // Add F9 key handling to load game
            else if (event.key.code == Keyboard::F9) {
                std::cout << "F9 pressed - Opening load game menu for username: '" << currentUsername << "'" << std::endl;

                // Make sure gameSave is initialized
                if (gameSave == nullptr) {
                    std::cout << "Initializing GameSave object" << std::endl;
                    gameSave = new GameSave();
                }

                // First get the list of saves for the current player
                savesList = gameSave->listSaves(currentUsername);
                std::cout << "Found " << savesList.getSize() << " saves for user '" << currentUsername << "'" << std::endl;

                showLoadMenu = true;
                gameState = GameState::PAUSED;
            }
            // Add space key handling to stop the player
            else if (event.key.code == Keyboard::Space && gameState == GameState::PLAYING) {
                // Check if player is on the border or solid ground (value 1)
                if (grid[player.getY()][player.getX()] == 1) {
                    player.setDirection(0, 0);
                }

                // Do the same for player 2 in multiplayer
                if (isMultiplayer && grid[player2.getY()][player2.getX()] == 1) {
                    player2.setDirection(0, 0);
                }
            }
            // Handle pause menu selection 
            else if (gameState == GameState::PAUSED) {
                // Process pause menu input
                processPauseMenuInput(event);
            }
        }
    }

    // Only process movement controls if game is in playing state
    if (gameState == GameState::PLAYING) {
        // Player 1 controls (WASD)
        if (Keyboard::isKeyPressed(Keyboard::A)) { player.setDirection(-1, 0); }
        if (Keyboard::isKeyPressed(Keyboard::D)) { player.setDirection(1, 0); }
        if (Keyboard::isKeyPressed(Keyboard::W)) { player.setDirection(0, -1); }
        if (Keyboard::isKeyPressed(Keyboard::S)) { player.setDirection(0, 1); }

        // Player 2 controls (Arrow keys)
        if (isMultiplayer) {
            if (Keyboard::isKeyPressed(Keyboard::Left)) { player2.setDirection(-1, 0); }
            if (Keyboard::isKeyPressed(Keyboard::Right)) { player2.setDirection(1, 0); }
            if (Keyboard::isKeyPressed(Keyboard::Up)) { player2.setDirection(0, -1); }
            if (Keyboard::isKeyPressed(Keyboard::Down)) { player2.setDirection(0, 1); }
        }
    }
}

// New method to process save/load menu input
void Game::processMenuInput(Event& event) {
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Escape) {
            // Cancel save/load operation
            showSaveMenu = false;
            showLoadMenu = false;
            gameState = GameState::PLAYING;
        }
        else if (showSaveMenu && event.key.code == Keyboard::Return) {
            // Confirm save operation
            if (saveGame(currentUsername)) {
                showSaveMenu = false;
                gameState = GameState::PLAYING;
            }
        }
        else if (showLoadMenu) {
            if (event.key.code == Keyboard::Up) {
                // Select previous save
                if (selectedSaveIndex > 0) {
                    selectedSaveIndex--;
                }
            }
            else if (event.key.code == Keyboard::Down) {
                // Select next save
                if (selectedSaveIndex < savesList.getSize() - 1) {
                    selectedSaveIndex++;
                }
            }
            else if (event.key.code == Keyboard::Return) {
                // Confirm load operation
                if (savesList.getSize() > 0) {
                    std::string saveID = savesList.get(selectedSaveIndex);
                    if (loadGame(saveID, currentUsername)) {
                        showLoadMenu = false;
                        gameState = GameState::PLAYING;
                    }
                }
            }
        }
    }
}

// New method to set current username
void Game::setCurrentUsername(const std::string& username) {
    std::cout << "Setting username: '" << username << "'" << std::endl;
    currentUsername = username;

    // Also set username for GameSave if it exists
    if (gameSave != nullptr) {
        std::cout << "Setting username for GameSave object" << std::endl;
    }
}

// New method to save the current game
bool Game::saveGame(const std::string& username) {
    if (gameSave == nullptr) {
        std::cout << "Error: GameSave object not initialized" << std::endl;
        return false;
    }

    std::cout << "Saving game for user: '" << username << "'" << std::endl;
    std::cout << "Grid dimensions: " << N << "x" << M << ", Score: " << score << std::endl;
    std::cout << "Enemy count: " << enemyCount << std::endl;
    std::cout << "Player position: (" << player.getX() << "," << player.getY() << ")" << std::endl;

    // Make sure the saves directory exists
    _mkdir("saves");

    // Call the GameSave method to save
    bool result = gameSave->saveGame(grid, N, M, username, score, player, enemies, enemyCount);

    if (result) {
        std::cout << "Game saved successfully for user: '" << username << "'" << std::endl;
    }
    else {
        std::cout << "Failed to save game for user: '" << username << "'" << std::endl;
    }

    return result;
}

// New method to load a saved game
bool Game::loadGame(const std::string& saveID, const std::string& username) {
    if (gameSave == nullptr) {
        std::cout << "Error: GameSave object not initialized" << std::endl;
        return false;
    }

    std::cout << "Loading game from save ID: '" << saveID << "' for user: '" << username << "'" << std::endl;

    // Call the GameSave method to load
    std::string loadedUsername;
    bool result = gameSave->loadGame(grid, const_cast<int&>(N), const_cast<int&>(M),
        loadedUsername, score, player, enemies, enemyCount, saveID);

    if (result) {
        std::cout << "Game loaded successfully!" << std::endl;
        std::cout << "Loaded username: '" << loadedUsername << "', Score: " << score << std::endl;
        std::cout << "Player position: (" << player.getX() << "," << player.getY() << ")" << std::endl;
        std::cout << "Enemy count: " << enemyCount << std::endl;

        // Update current username if it was empty
        if (currentUsername.empty()) {
            currentUsername = loadedUsername;
            std::cout << "Updated current username to: '" << currentUsername << "'" << std::endl;
        }

        // Reset game state to playing
        gameState = GameState::PLAYING;
    }
    else {
        std::cout << "Failed to load game from save ID: '" << saveID << "'" << std::endl;
    }

    return result;
}

// New method to get list of saves for a player
LinkedList<std::string> Game::getSaves(const std::string& username) {
    if (gameSave == nullptr) {
        std::cout << "Error: GameSave object not initialized" << std::endl;
        return LinkedList<std::string>();
    }

    return gameSave->listSaves(username);
}

// New method to draw save menu
void Game::drawSaveMenu() {
    // Create a semi-transparent overlay
    RectangleShape overlay;
    overlay.setSize(Vector2f(window->getSize()));
    overlay.setFillColor(Color(0, 0, 0, 200));
    window->draw(overlay);

    // Create save menu panel
    RectangleShape menuPanel;
    menuPanel.setSize(Vector2f(400, 200));
    menuPanel.setPosition(window->getSize().x / 2 - 200, window->getSize().y / 2 - 100);
    menuPanel.setFillColor(Color(50, 50, 70));
    menuPanel.setOutlineThickness(2);
    menuPanel.setOutlineColor(Color(100, 100, 150));
    window->draw(menuPanel);

    // Draw title
    if (font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        Text titleText;
        titleText.setFont(font);
        titleText.setString("Save Game");
        titleText.setCharacterSize(24);
        titleText.setFillColor(Color::White);
        titleText.setPosition(window->getSize().x / 2 - titleText.getLocalBounds().width / 2,
            window->getSize().y / 2 - 80);
        window->draw(titleText);

        // Draw message
        Text messageText;
        messageText.setFont(font);
        messageText.setString("Press Enter to save your current game\nor Esc to cancel");
        messageText.setCharacterSize(18);
        messageText.setFillColor(Color::White);
        messageText.setPosition(window->getSize().x / 2 - messageText.getLocalBounds().width / 2,
            window->getSize().y / 2 - 30);
        window->draw(messageText);

        // Draw player info
        Text playerText;
        playerText.setFont(font);
        playerText.setString("Player: " + currentUsername);
        playerText.setCharacterSize(18);
        playerText.setFillColor(Color::Yellow);
        playerText.setPosition(window->getSize().x / 2 - playerText.getLocalBounds().width / 2,
            window->getSize().y / 2 + 20);
        window->draw(playerText);

        // Draw score info
        Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(18);
        scoreText.setFillColor(Color::Yellow);
        scoreText.setPosition(window->getSize().x / 2 - scoreText.getLocalBounds().width / 2,
            window->getSize().y / 2 + 50);
        window->draw(scoreText);
    }
}

// New method to draw load menu
void Game::drawLoadMenu() {
    // Create a semi-transparent overlay
    RectangleShape overlay;
    overlay.setSize(Vector2f(window->getSize()));
    overlay.setFillColor(Color(0, 0, 0, 200));
    window->draw(overlay);

    // Create load menu panel
    RectangleShape menuPanel;
    menuPanel.setSize(Vector2f(400, 300));
    menuPanel.setPosition(window->getSize().x / 2 - 200, window->getSize().y / 2 - 150);
    menuPanel.setFillColor(Color(50, 50, 70));
    menuPanel.setOutlineThickness(2);
    menuPanel.setOutlineColor(Color(100, 100, 150));
    window->draw(menuPanel);

    if (font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        // Draw title
        Text titleText;
        titleText.setFont(font);
        titleText.setString("Load Game");
        titleText.setCharacterSize(24);
        titleText.setFillColor(Color::White);
        titleText.setPosition(window->getSize().x / 2 - titleText.getLocalBounds().width / 2,
            window->getSize().y / 2 - 130);
        window->draw(titleText);

        // Draw player info
        Text playerText;
        playerText.setFont(font);
        playerText.setString("Player: " + currentUsername);
        playerText.setCharacterSize(18);
        playerText.setFillColor(Color::Yellow);
        playerText.setPosition(window->getSize().x / 2 - 180, window->getSize().y / 2 - 90);
        window->draw(playerText);

        // Draw instructions
        Text instructionsText;
        instructionsText.setFont(font);
        instructionsText.setString("Use Up/Down keys to select a save\nPress Enter to load, Esc to cancel");
        instructionsText.setCharacterSize(16);
        instructionsText.setFillColor(Color(200, 200, 200));
        instructionsText.setPosition(window->getSize().x / 2 - 180, window->getSize().y / 2 - 60);
        window->draw(instructionsText);

        // Draw save list or "No Saves" message
        if (savesList.getSize() > 0) {
            // Draw each save in the list
            for (int i = 0; i < savesList.getSize() && i < 5; i++) {
                RectangleShape saveItemBg;
                saveItemBg.setSize(Vector2f(360, 30));
                saveItemBg.setPosition(window->getSize().x / 2 - 180, window->getSize().y / 2 - 10 + i * 35);

                // Highlight selected save
                if (i == selectedSaveIndex) {
                    saveItemBg.setFillColor(Color(100, 100, 150));
                    saveItemBg.setOutlineThickness(1);
                    saveItemBg.setOutlineColor(Color::White);
                }
                else {
                    saveItemBg.setFillColor(Color(70, 70, 90));
                }

                window->draw(saveItemBg);

                // Get save info
                SaveGameState saveInfo = gameSave->getSaveInfo(savesList.get(i));

                // Convert timestamp to readable form
                std::string dateStr;
                if (saveInfo.timestamp != 0) {
                    time_t timestamp = saveInfo.timestamp;
                    char buffer[80];
                    struct tm timeinfo;
                    localtime_s(&timeinfo, &timestamp);
                    strftime(buffer, sizeof(buffer), "%m/%d/%Y %H:%M", &timeinfo);
                    dateStr = buffer;
                }
                else {
                    dateStr = "Unknown";
                }

                // Draw save ID and date
                Text saveText;
                saveText.setFont(font);
                saveText.setString(savesList.get(i) + " - " + dateStr);
                saveText.setCharacterSize(16);
                saveText.setFillColor(Color::White);
                saveText.setPosition(window->getSize().x / 2 - 175, window->getSize().y / 2 - 5 + i * 35);
                window->draw(saveText);

                // Draw score
                Text scoreText;
                scoreText.setFont(font);
                scoreText.setString("Score: " + std::to_string(saveInfo.score));
                scoreText.setCharacterSize(16);
                scoreText.setFillColor(Color::Yellow);
                scoreText.setPosition(window->getSize().x / 2 + 50, window->getSize().y / 2 - 5 + i * 35);
                window->draw(scoreText);
            }
        }
        else {
            // No saves found
            Text noSavesText;
            noSavesText.setFont(font);
            noSavesText.setString("No saved games found for this player");
            noSavesText.setCharacterSize(16);
            noSavesText.setFillColor(Color::White);
            noSavesText.setPosition(window->getSize().x / 2 - noSavesText.getLocalBounds().width / 2,
                window->getSize().y / 2);
            window->draw(noSavesText);
        }
    }
}

void Game::updatePlayer1() {
    // Get player's current position and grid value before moving
    int currentY = player.getY();
    int currentX = player.getX();
    int currentGridValue = grid[currentY][currentX];

    // Update player 1 position
    player.setX(player.getX() + player.getDX());
    player.setY(player.getY() + player.getDY());

    // Boundary checks for player 1
    if (player.getX() < 0) player.setX(0);
    if (player.getX() > N - 1) player.setX(N - 1);
    if (player.getY() < 0) player.setY(0);
    if (player.getY() > M - 1) player.setY(M - 1);

    // Check for enemy collision
    for (int i = 0; i < enemyCount; i++) {
        int enemyX = enemies[i].getX() / ts;
        int enemyY = enemies[i].getY() / ts;
        if (player.getX() == enemyX && player.getY() == enemyY) {
            gameState = GameState::GAME_OVER;
            return;
        }
    }

    // Check if player hit their own trail or border
    if (grid[player.getY()][player.getX()] == 2 ||
        (grid[player.getY()][player.getX()] == 1 && currentGridValue == 2)) {
        // Player hit their own trail or returned to border after making a trail
        checkCollisions();
        player.setDirection(0, 0);
    }
    else if (grid[player.getY()][player.getX()] == 0 ||
        (currentGridValue == 1 && grid[player.getY()][player.getX()] == 0)) {
        // Player is in empty space or just left solid ground
        grid[player.getY()][player.getX()] = 2;  // Player 1 trail
    }
}

void Game::updatePlayer2() {
    if (!isMultiplayer) return;

    // Get player's current position and grid value before moving
    int currentY = player2.getY();
    int currentX = player2.getX();
    int currentGridValue = grid[currentY][currentX];

    // Update player 2 position
    player2.setX(player2.getX() + player2.getDX());
    player2.setY(player2.getY() + player2.getDY());

    // Boundary checks for player 2
    if (player2.getX() < 0) player2.setX(0);
    if (player2.getX() > N - 1) player2.setX(N - 1);
    if (player2.getY() < 0) player2.setY(0);
    if (player2.getY() > M - 1) player2.setY(M - 1);

    // Check for enemy collision
    for (int i = 0; i < enemyCount; i++) {
        int enemyX = enemies[i].getX() / ts;
        int enemyY = enemies[i].getY() / ts;
        if (player2.getX() == enemyX && player2.getY() == enemyY) {
            gameState = GameState::GAME_OVER;
            return;
        }
    }

    // Check if player 2 hit their own trail or border
    if (grid[player2.getY()][player2.getX()] == 3 ||
        (grid[player2.getY()][player2.getX()] == 1 && currentGridValue == 3)) {
        // Player 2 hit their own trail or returned to border after making a trail
        checkCollisions();
        player2.setDirection(0, 0);
    }
    else if (grid[player2.getY()][player2.getX()] == 0 ||
        (currentGridValue == 1 && grid[player2.getY()][player2.getX()] == 0)) {
        // Player 2 is in empty space or just left solid ground
        grid[player2.getY()][player2.getX()] = 3;  // Player 2 trail
    }
}

void Game::updateEnemy(Enemy& enemy) {
    bool hitTrail = enemy.move(grid, ts);
    if (hitTrail) {
        gameState = GameState::GAME_OVER;
    }
}

bool Game::shouldReturnToMenu() const {
    return returnToMenu;
}

void Game::checkCollisions() {
    // ===== LOSE CONDITIONS =====
    // 1. Player 1 collisions
    int player1X = player.getX();
    int player1Y = player.getY();

    // Player 1 hits their own trail
    if (grid[player1Y][player1X] == 2) {
        gameState = GameState::GAME_OVER;
        return;
    }

    // Player 1 hits an enemy
    for (int i = 0; i < enemyCount; i++) {
        int enemyX = enemies[i].getX() / ts;
        int enemyY = enemies[i].getY() / ts;
        if (player1X == enemyX && player1Y == enemyY) {
            gameState = GameState::GAME_OVER;
            return;
        }
    }

    // 2. Player 2 collisions (in multiplayer)
    if (isMultiplayer) {
        int player2X = player2.getX();
        int player2Y = player2.getY();

        // Player 2 hits their own trail
        if (grid[player2Y][player2X] == 3) {
            gameState = GameState::GAME_OVER;
            return;
        }

        // Player 2 hits an enemy
        for (int i = 0; i < enemyCount; i++) {
            int enemyX = enemies[i].getX() / ts;
            int enemyY = enemies[i].getY() / ts;
            if (player2X == enemyX && player2Y == enemyY) {
                gameState = GameState::GAME_OVER;
                return;
            }
        }
    }

    // 3. Enemy hits a trail
    for (int i = 0; i < enemyCount; i++) {
        int enemyX = enemies[i].getX() / ts;
        int enemyY = enemies[i].getY() / ts;
        if (grid[enemyY][enemyX] == 2 || grid[enemyY][enemyX] == 3) {
            gameState = GameState::GAME_OVER;
            return;
        }
    }

    // ===== GAME PROGRESSION =====
    // 1. Convert all TRAIL to WALL
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 2 || grid[i][j] == 3) {
                grid[i][j] = 1;
            }
        }
    }

    // ===== WIN CONDITIONS =====
    // 1. Check if all enemies are captured
    bool anyEnemyInEmpty = false;
    for (int e = 0; e < enemyCount; e++) {
        int ex = enemies[e].getX() / ts;
        int ey = enemies[e].getY() / ts;
        if (grid[ey][ex] == 0) {
            anyEnemyInEmpty = true;
            break;
        }
    }

    // Win by capturing all enemies
    if (!anyEnemyInEmpty) {
        gameState = GameState::WIN;
        winReason = "All Enemies Captured!";
        return;
    }

    // 2. Check area coverage
    // First fill unreachable areas
    bool reachable[M][N];
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            reachable[i][j] = false;
        }
    }

    // Mark all areas reachable by enemies using our own Queue class
    // We'll use the playerID field to encode coordinates (y*N + x)
    Queue floodQueue;

    // First add enemy positions to the queue
    for (int e = 0; e < enemyCount; e++) {
        int ex = enemies[e].getX() / ts;
        int ey = enemies[e].getY() / ts;
        if (grid[ey][ex] == 0 && !reachable[ey][ex]) {
            reachable[ey][ex] = true;
            floodQueue.enqueue(ey * N + ex);  // Encode coordinates
        }
    }

    // Process the queue for flood fill
    while (!floodQueue.isEmpty()) {
        QueueNode node = floodQueue.dequeue();
        int encodedPos = node.playerID;

        // Decode coordinates
        int y = encodedPos / N;
        int x = encodedPos % N;

        // Try all four directions
        int dy[] = { -1, 1, 0, 0 };
        int dx[] = { 0, 0, -1, 1 };

        for (int d = 0; d < 4; d++) {
            int ny = y + dy[d];
            int nx = x + dx[d];

            // Check if valid and not visited
            if (ny >= 0 && ny < M && nx >= 0 && nx < N &&
                grid[ny][nx] == 0 && !reachable[ny][nx]) {
                reachable[ny][nx] = true;
                floodQueue.enqueue(ny * N + nx);  // Encode new coordinates
            }
        }
    }

    // Fill unreachable areas
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 0 && !reachable[i][j]) {
                grid[i][j] = 1;
            }
        }
    }

    // Calculate coverage and check win condition
    int filledCells = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 1) {
                filledCells++;
            }
        }
    }
    updateScore(filledCells);

    // Win by area coverage
    int totalArea = M * N;
    float coveragePercentage = (float)filledCells / totalArea * 100.0f;
    if (coveragePercentage >= 60.0f) {
        gameState = GameState::WIN;
        winReason = "Area Coverage Achieved!";
        return;
    }
}
void Game::update(float deltaTime) {
    if (!isRunning) return;

    timer += deltaTime;

    // Update notification timer
    if (notificationTimer > 0) {
        notificationTimer -= deltaTime;
        if (notificationTimer <= 0) {
            notificationMessage = "";
        }
    }

    // Check for F5 and F9 keys directly during gameplay
    static bool f5KeyWasPressed = false;
    static bool f9KeyWasPressed = false;

    if (gameState == GameState::PLAYING) {
        // F5 key handling for save
        bool f5KeyIsPressed = Keyboard::isKeyPressed(Keyboard::F5);
        if (f5KeyIsPressed && !f5KeyWasPressed) {
            // Only trigger once when key is pressed
            std::cout << "F5 save key detected (direct) - Saving game with username: '" << currentUsername << "'" << std::endl;

            // Check if username is empty
            if (currentUsername.empty()) {
                std::cout << "WARNING: Username is empty, using 'default_user'" << std::endl;
                currentUsername = "default_user";
            }

            // Make sure gameSave is initialized
            if (gameSave == nullptr) {
                std::cout << "Initializing GameSave object" << std::endl;
                gameSave = new GameSave();
            }

            if (saveGame(currentUsername)) {
                // Show a temporary saved message
                std::cout << "Game saved successfully!" << std::endl;
                notificationMessage = "Game saved successfully!";
                notificationTimer = 3.0f; // Show for 3 seconds
            }
            else {
                std::cout << "Game save failed!" << std::endl;
                notificationMessage = "Failed to save game!";
                notificationTimer = 3.0f; // Show for 3 seconds
            }
        }
        f5KeyWasPressed = f5KeyIsPressed;

        // F9 key handling for load
        bool f9KeyIsPressed = Keyboard::isKeyPressed(Keyboard::F9);
        if (f9KeyIsPressed && !f9KeyWasPressed) {
            // Only trigger once when key is pressed
            std::cout << "F9 load key detected (direct) - Opening load game menu" << std::endl;

            // Make sure gameSave is initialized
            if (gameSave == nullptr) {
                std::cout << "Initializing GameSave object" << std::endl;
                gameSave = new GameSave();
            }

            // First get the list of saves for the current player
            savesList = gameSave->listSaves(currentUsername);
            std::cout << "Found " << savesList.getSize() << " saves for user '" << currentUsername << "'" << std::endl;

            showLoadMenu = true;
            gameState = GameState::PAUSED;
        }
        f9KeyWasPressed = f9KeyIsPressed;
    }
    else {
        // Reset key states when not in playing mode
        f5KeyWasPressed = Keyboard::isKeyPressed(Keyboard::F5);
        f9KeyWasPressed = Keyboard::isKeyPressed(Keyboard::F9);
    }

    if (timer > delay && gameState == GameState::PLAYING) {
        // Update player positions
        updatePlayer1();
        if (isMultiplayer) {
            updatePlayer2();
        }

        // Update enemies
        for (int i = 0; i < enemyCount; i++) {
            // Save previous position
            int prevEnemyX = enemies[i].getX() / ts;
            int prevEnemyY = enemies[i].getY() / ts;

            // Move the enemy
            updateEnemy(enemies[i]);

            // Get new position
            int newEnemyX = enemies[i].getX() / ts;
            int newEnemyY = enemies[i].getY() / ts;

            // Check if enemy hit a trail
            if (grid[newEnemyY][newEnemyX] == 2 || grid[newEnemyY][newEnemyX] == 3) {
                gameState = GameState::GAME_OVER;
                return;
            }

            // Also check if enemy crossed a trail (moved through it)
            if (prevEnemyX != newEnemyX || prevEnemyY != newEnemyY) {
                // Check if enemy crossed between two trail cells
                if (grid[prevEnemyY][newEnemyX] == 2 || grid[prevEnemyY][newEnemyX] == 3) {
                    if (grid[newEnemyY][prevEnemyX] == 2 || grid[newEnemyY][prevEnemyX] == 3) {
                        gameState = GameState::GAME_OVER;
                        return;
                    }
                }
            }
        }

        timer = 0;
    }
}

void Game::fillArea(int y, int x, int tempGrid[][N], bool& containsEnemy) {
    if (tempGrid[y][x] == 0) {
        tempGrid[y][x] = -1;  // Mark as checked

        // Check if there's an enemy in this cell
        for (int i = 0; i < enemyCount; i++) {
            int enemyX = enemies[i].getX() / ts;
            int enemyY = enemies[i].getY() / ts;
            if (enemyX == x && enemyY == y) {
                containsEnemy = true;
                return;
            }
        }

        // Recursively check adjacent cells
        if (y - 1 >= 0) fillArea(y - 1, x, tempGrid, containsEnemy);
        if (y + 1 < M) fillArea(y + 1, x, tempGrid, containsEnemy);
        if (x - 1 >= 0) fillArea(y, x - 1, tempGrid, containsEnemy);
        if (x + 1 < N) fillArea(y, x + 1, tempGrid, containsEnemy);
    }
}

void Game::drop(int y, int x) {
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (y - 1 >= 0 && grid[y - 1][x] == 0) drop(y - 1, x);
    if (y + 1 < M && grid[y + 1][x] == 0) drop(y + 1, x);
    if (x - 1 >= 0 && grid[y][x - 1] == 0) drop(y, x - 1);
    if (x + 1 < N && grid[y][x + 1] == 0) drop(y, x + 1);
}

void Game::updateScore(int points) {
    // Apply bonus multiplier if enough tiles were captured in one move
    if (points >= bonusThreshold) {
        int originalPoints = points;
        points = points * bonusMultiplier;

        // Update bonus counter and adjust threshold/multiplier if needed
        bonusCounter++;

        if (bonusCounter == 3) {
            // After 3 bonus occurrences, reduce threshold to 5
            bonusThreshold = 5;
        }
        else if (bonusCounter == 5) {
            // After 5 total bonus occurrences, increase multiplier to 4
            bonusMultiplier = 4;
        }

        // Display bonus message
        std::cout << "BONUS! " << originalPoints << " tiles x" << bonusMultiplier
            << " = " << points << " points!" << std::endl;
    }

    // Add points to score
    int oldScore = score;
    score += points;

    // Check for power-up milestones
    if ((oldScore < 50 && score >= 50) ||
        (oldScore < 70 && score >= 70) ||
        (oldScore < 100 && score >= 100) ||
        (oldScore < 130 && score >= 130) ||
        (oldScore / 30 < score / 30 && score >= 130)) {

        // Award a power-up
        player.addPowerup();
        std::cout << "Power-up earned! Total power-ups: " << player.getPowerups() << std::endl;
    }

    // Update high score if needed
    if (score > highScore) {
        highScore = score;
        std::cout << "New high score: " << highScore << std::endl;

        // Also update the leaderboard immediately to save the high score
        if (!currentUsername.empty()) {
            static Leaderboard leaderboard("leaderboard.dat");
            leaderboard.updateScore(currentUsername, score);
        }
    }
}

void Game::draw() {
    if (window == nullptr) return;

    window->clear(Color::Black);

    // Draw grid
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 0) continue;
            if (grid[i][j] == 1) tileSprite.setTextureRect(IntRect(0, 0, ts, ts));
            if (grid[i][j] == 2) tileSprite.setTextureRect(IntRect(54, 0, ts, ts));  // Player 1 trail
            if (grid[i][j] == 3) tileSprite.setTextureRect(IntRect(72, 0, ts, ts));  // Player 2 trail
            tileSprite.setPosition(j * ts, i * ts);
            window->draw(tileSprite);
        }
    }

    // Draw player 1
    tileSprite.setTextureRect(IntRect(36, 0, ts, ts));
    tileSprite.setPosition(player.getX() * ts, player.getY() * ts);
    window->draw(tileSprite);

    // Draw player 2 in multiplayer
    if (isMultiplayer) {
        tileSprite.setTextureRect(IntRect(36, 18, ts, ts));  // Different color for player 2
        tileSprite.setPosition(player2.getX() * ts, player2.getY() * ts);
        window->draw(tileSprite);
    }

    // Draw enemies
    enemySprite.rotate(10);
    for (int i = 0; i < enemyCount; i++) {
        enemySprite.setPosition(enemies[i].getX(), enemies[i].getY());
        window->draw(enemySprite);
    }

    // Draw score and coverage
    if (font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(Color::White);
        scoreText.setPosition(10, 10);
        window->draw(scoreText);

        Text highScoreText;
        highScoreText.setFont(font);
        highScoreText.setString("High Score: " + std::to_string(highScore));
        highScoreText.setCharacterSize(20);
        highScoreText.setFillColor(Color::White);
        highScoreText.setPosition(10, 40);
        window->draw(highScoreText);

        // Calculate and display coverage percentage
        int totalArea = M * N;
        int filledArea = 0;
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (grid[i][j] == 1) {
                    filledArea++;
                }
            }
        }
        float coveragePercentage = (float)filledArea / totalArea * 100.0f;

        Text coverageText;
        coverageText.setFont(font);
        coverageText.setString("Coverage: " + std::to_string((int)coveragePercentage) + "%");
        coverageText.setCharacterSize(20);
        coverageText.setFillColor(Color::White);
        coverageText.setPosition(10, 70);
        window->draw(coverageText);

        // Draw power-ups
        Text powerupsText;
        powerupsText.setFont(font);
        powerupsText.setString("Power-ups: " + std::to_string(player.getPowerups()) + " (Press P to use)");
        powerupsText.setCharacterSize(20);
        powerupsText.setFillColor(Color::Yellow);
        powerupsText.setPosition(10, 100);
        window->draw(powerupsText);

        // Draw bonus info
        Text bonusText;
        bonusText.setFont(font);
        bonusText.setString("Bonus: x" + std::to_string(bonusMultiplier) +
            " for " + std::to_string(bonusThreshold) + "+ tiles");
        bonusText.setCharacterSize(20);
        bonusText.setFillColor(Color::Cyan);
        bonusText.setPosition(10, 130);
        window->draw(bonusText);

        // Draw save/load game hint
        Text saveLoadText;
        saveLoadText.setFont(font);
        saveLoadText.setString("F5: Save Game | F9: Load Game");
        saveLoadText.setCharacterSize(20);
        saveLoadText.setFillColor(Color(150, 150, 255));
        saveLoadText.setPosition(10, 160);
        window->draw(saveLoadText);
    }

    // Draw game over or win screen
    if (gameState == GameState::GAME_OVER) {
        // Create a semi-transparent overlay
        RectangleShape overlay;
        overlay.setSize(Vector2f(window->getSize()));
        overlay.setFillColor(Color(0, 0, 0, 200));
        window->draw(overlay);

        // Draw game over message
        Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setString("GAME OVER");
        gameOverText.setCharacterSize(60);
        gameOverText.setFillColor(Color::Red);
        gameOverText.setStyle(Text::Bold);

        // Center the game over text
        FloatRect textBounds = gameOverText.getLocalBounds();
        gameOverText.setPosition(
            window->getSize().x / 2 - textBounds.width / 2,
            window->getSize().y / 2 - textBounds.height / 2 - 30
        );
        window->draw(gameOverText);

        // Draw final score
        Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Final Score: " + std::to_string(score));
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(Color::White);

        // Center the score text below the game over text
        FloatRect scoreBounds = scoreText.getLocalBounds();
        scoreText.setPosition(
            window->getSize().x / 2 - scoreBounds.width / 2,
            window->getSize().y / 2 + 30
        );
        window->draw(scoreText);

        // Draw exit instruction
        Text exitText;
        exitText.setFont(font);
        exitText.setString("Press ESC to exit");
        exitText.setCharacterSize(20);
        exitText.setFillColor(Color::White);

        // Center the exit text at the bottom
        FloatRect exitBounds = exitText.getLocalBounds();
        exitText.setPosition(
            window->getSize().x / 2 - exitBounds.width / 2,
            window->getSize().y - 50
        );
        window->draw(exitText);
    }
    else if (gameState == GameState::WIN) {
        // Create a semi-transparent overlay
        RectangleShape overlay;
        overlay.setSize(Vector2f(window->getSize()));
        overlay.setFillColor(Color(0, 0, 0, 200));
        window->draw(overlay);

        // Draw win message
        Text winText;
        winText.setFont(font);
        winText.setString("YOU WON!");
        winText.setCharacterSize(60);
        winText.setFillColor(Color::Green);
        winText.setStyle(Text::Bold);

        // Center the main win text
        FloatRect textBounds = winText.getLocalBounds();
        winText.setPosition(
            window->getSize().x / 2 - textBounds.width / 2,
            window->getSize().y / 2 - textBounds.height / 2 - 30
        );
        window->draw(winText);

        // Draw reason for winning
        Text reasonText;
        reasonText.setFont(font);
        reasonText.setString(winReason);
        reasonText.setCharacterSize(30);
        reasonText.setFillColor(Color::Yellow);

        // Center the reason text below the main win text
        FloatRect reasonBounds = reasonText.getLocalBounds();
        reasonText.setPosition(
            window->getSize().x / 2 - reasonBounds.width / 2,
            window->getSize().y / 2 + 30
        );
        window->draw(reasonText);

        // Draw exit instruction
        Text exitText;
        exitText.setFont(font);
        exitText.setString("Press ESC to exit");
        exitText.setCharacterSize(20);
        exitText.setFillColor(Color::White);

        // Center the exit text at the bottom
        FloatRect exitBounds = exitText.getLocalBounds();
        exitText.setPosition(
            window->getSize().x / 2 - exitBounds.width / 2,
            window->getSize().y - 50
        );
        window->draw(exitText);
    }
    else if (gameState == GameState::PAUSED) {
        // Create a semi-transparent overlay
        RectangleShape overlay;
        overlay.setSize(Vector2f(window->getSize()));
        overlay.setFillColor(Color(0, 0, 0, 150));
        window->draw(overlay);

        // Create a pause menu panel
        RectangleShape pausePanel;
        pausePanel.setSize(Vector2f(300, 350));
        pausePanel.setPosition(window->getSize().x / 2 - 150, window->getSize().y / 2 - 175);
        pausePanel.setFillColor(Color(50, 50, 70, 230));
        pausePanel.setOutlineThickness(3);
        pausePanel.setOutlineColor(Color(100, 100, 150));
        window->draw(pausePanel);

        // Draw pause message
        Text pauseText;
        pauseText.setFont(font);
        pauseText.setString("GAME PAUSED");
        pauseText.setCharacterSize(40);
        pauseText.setFillColor(Color::White);
        pauseText.setStyle(Text::Bold);

        // Center the pause text
        FloatRect textBounds = pauseText.getLocalBounds();
        pauseText.setPosition(
            window->getSize().x / 2 - textBounds.width / 2,
            window->getSize().y / 2 - 150
        );
        window->draw(pauseText);

        // Get current mouse position for hover effects
        Vector2i mousePos = Mouse::getPosition(*window);
        Vector2f mousePosF = window->mapPixelToCoords(mousePos);

        // Draw menu options
        std::string options[] = {
            "R - Resume Game",
            "S - Save Game",
            "L - Load Game",
            "M - Main Menu",
            "Q - Quit Game"
        };

        Color buttonColors[] = {
            Color(0, 150, 0, 200),   // Green for Resume
            Color(0, 100, 150, 200), // Blue for Save
            Color(0, 100, 150, 200), // Blue for Load
            Color(150, 100, 0, 200), // Orange for Main Menu
            Color(150, 0, 0, 200)    // Red for Quit
        };

        // Draw each option with a button
        for (int i = 0; i < 5; i++) {
            // Create button bounds
            FloatRect buttonBounds(window->getSize().x / 2 - 125, window->getSize().y / 2 - 70 + (i * 50), 250, 40);

            // Check for mouse hover
            bool isHovered = buttonBounds.contains(mousePosF);

            // Draw button background with hover effect
            RectangleShape buttonBg;
            buttonBg.setSize(Vector2f(250, 40));
            buttonBg.setPosition(buttonBounds.left, buttonBounds.top);

            // Brighten the color if hovered
            Color btnColor = buttonColors[i];
            if (isHovered) {
                btnColor.r = std::min(255, btnColor.r + 40);
                btnColor.g = std::min(255, btnColor.g + 40);
                btnColor.b = std::min(255, btnColor.b + 40);

                // Add glow effect when hovered
                RectangleShape glow;
                glow.setSize(Vector2f(260, 50));
                glow.setPosition(buttonBounds.left - 5, buttonBounds.top - 5);
                glow.setFillColor(Color(200, 200, 255, 60));
                window->draw(glow);
            }

            buttonBg.setFillColor(btnColor);
            buttonBg.setOutlineThickness(isHovered ? 3 : 2);
            buttonBg.setOutlineColor(isHovered ? Color(255, 255, 255, 220) : Color(200, 200, 200, 150));
            window->draw(buttonBg);

            // Draw option text
            Text optionText;
            optionText.setFont(font);
            optionText.setString(options[i]);
            optionText.setCharacterSize(isHovered ? 22 : 20); // Larger text when hovered
            optionText.setFillColor(isHovered ? Color::White : Color(230, 230, 230));
            if (isHovered) optionText.setStyle(Text::Bold);

            // Center the text on button
            FloatRect optionBounds = optionText.getLocalBounds();
            optionText.setPosition(
                window->getSize().x / 2 - optionBounds.width / 2,
                window->getSize().y / 2 - 65 + (i * 50)
            );
            window->draw(optionText);
        }

        // Add current score display
        Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Current Score: " + std::to_string(score));
        scoreText.setCharacterSize(18);
        scoreText.setFillColor(Color(220, 220, 100));
        FloatRect scoreBounds = scoreText.getLocalBounds();
        scoreText.setPosition(
            window->getSize().x / 2 - scoreBounds.width / 2,
            window->getSize().y / 2 + 190
        );
        window->draw(scoreText);
    }

    // Draw save menu if active
    if (showSaveMenu) {
        drawSaveMenu();
    }

    // Draw load menu if active
    if (showLoadMenu) {
        drawLoadMenu();
    }

    // Draw notification message if active
    if (notificationTimer > 0 && !notificationMessage.empty()) {
        // Create a notification box at the bottom of the screen
        RectangleShape notificationBox;
        notificationBox.setSize(Vector2f(400, 40));
        notificationBox.setPosition(window->getSize().x / 2 - 200, window->getSize().y - 60);
        notificationBox.setFillColor(Color(0, 100, 0, 200)); // Semi-transparent green
        notificationBox.setOutlineThickness(2);
        notificationBox.setOutlineColor(Color::White);
        window->draw(notificationBox);

        // Draw notification text
        Text notificationText;
        notificationText.setFont(font);
        notificationText.setString(notificationMessage);
        notificationText.setCharacterSize(20);
        notificationText.setFillColor(Color::White);

        // Center the text in the notification box
        FloatRect textBounds = notificationText.getLocalBounds();
        notificationText.setPosition(
            window->getSize().x / 2 - textBounds.width / 2,
            window->getSize().y - 50 - textBounds.height / 2
        );
        window->draw(notificationText);
    }

    // Display key state debug info when in playing mode
    if (gameState == GameState::PLAYING) {
        Text debugText;
        debugText.setFont(font);
        std::string debugInfo = "F5: Save Game | F9: Load Game | Username: '" + currentUsername + "'";
        debugText.setString(debugInfo);
        debugText.setCharacterSize(12);
        debugText.setFillColor(Color(200, 200, 200, 150));
        debugText.setPosition(10, 5);
        window->draw(debugText);
    }

    window->display();
}

void Game::run() {
    // Make sure the window is created
    if (window == nullptr) {
        initialize();
    }

    // Make sure the gameSave object is initialized
    if (gameSave == nullptr) {
        std::cout << "Initializing GameSave object in run()" << std::endl;
        gameSave = new GameSave();
    }

    // Confirm username is set properly
    std::cout << "Running game with username: '" << currentUsername << "'" << std::endl;

    // Check if we have a username, if not set a default one
    if (currentUsername.empty()) {
        std::cout << "No username set, using 'default_player'" << std::endl;
        currentUsername = "default_player";
    }

    // Load high score for current user from leaderboard
    if (highScore == 0) { // Only load if not already set
        static Leaderboard leaderboard("leaderboard.dat");
        int count;
        PlayerScore* scores = leaderboard.getTopScores(count);

        // Find current user's high score
        for (int i = 0; i < count; i++) {
            if (scores[i].username == currentUsername && scores[i].score > highScore) {
                highScore = scores[i].score;
                std::cout << "Loaded high score for " << currentUsername << ": " << highScore << std::endl;
            }
        }

        delete[] scores;
    }

    // Display key hints at the start
    notificationMessage = "Press ESC or P to pause game | F5: Save | F9: Load";
    notificationTimer = 5.0f;

    Clock clock;
    float deltaTime;

    // Make sure the game state is set to playing
    gameState = GameState::PLAYING;
    isRunning = true;

    while (isRunning && window->isOpen()) {
        // Calculate delta time first
        deltaTime = clock.restart().asSeconds();

        // Process input, update, and draw
        processInput();
        if (!isRunning) break;  // Break if ESC was pressed and we're returning to menu

        update(deltaTime);
        draw();

        // Small sleep to prevent CPU hogging
        sf::sleep(sf::milliseconds(1));
    }
}

void Game::pause() {
    // Only pause if currently in playing state
    if (gameState == GameState::PLAYING) {
        std::cout << "Game paused" << std::endl;
        gameState = GameState::PAUSED;

        // Show notification
        notificationMessage = "Game Paused - Press ESC to resume";
        notificationTimer = 3.0f;
    }
}

void Game::resume() {
    // Only resume if currently in paused state
    if (gameState == GameState::PAUSED) {
        std::cout << "Game resumed" << std::endl;
        gameState = GameState::PLAYING;

        // Show notification
        notificationMessage = "Game Resumed";
        notificationTimer = 1.5f;
    }
}

void Game::end() {
    // Update leaderboard before ending the game
    if (!currentUsername.empty() && score > 0) {
        static Leaderboard leaderboard("leaderboard.dat");
        leaderboard.updateScore(currentUsername, score);
        std::cout << "Final score for " << currentUsername << ": " << score << " added to leaderboard" << std::endl;

        // Update highScore if needed
        if (score > highScore) {
            highScore = score;
            std::cout << "New high score: " << highScore << std::endl;
        }
    }

    isRunning = false;
}

int Game::getScore() const {
    return score;
}

bool Game::getIsRunning() const {
    return isRunning;
}

RenderWindow* Game::getWindow() {
    return window;
}

// In Game.cpp
int Game::getFinalScore() const {
    return score; // Return the game's score variable instead of player.getScore()
}

void Game::initializeMultiplayer(const std::string& player1Name, const std::string& player2Name) {
    // Create window if it doesn't exist
    if (window == nullptr) {
        window = new RenderWindow(VideoMode(N * ts, M * ts), "Xonix Game - Multiplayer!");
        window->setFramerateLimit(60);
    }

    // Load textures
    if (!tileTexture.loadFromFile("images/tiles.png") ||
        !gameoverTexture.loadFromFile("images/gameover.png") ||
        !enemyTexture.loadFromFile("images/enemy.png")) {
        // Handle error loading textures
        return;
    }

    // Set up sprites
    tileSprite.setTexture(tileTexture);
    gameoverSprite.setTexture(gameoverTexture);
    gameoverSprite.setPosition(100, 100);
    enemySprite.setTexture(enemyTexture);
    enemySprite.setOrigin(20, 20);

    // Initialize game state
    initializeGrid();
    resetGame();

    // Set up multiplayer
    isMultiplayer = true;
    player = Player(1);  // Player 1
    player2 = Player(2); // Player 2
    player.reset(10, 0);  // Start position for player 1
    player2.reset(N - 11, M - 1);  // Start position for player 2

    // Increase enemy count for multiplayer
    enemyCount = 6;  // More enemies for multiplayer challenge

    isRunning = true;
}

void Game::activatePowerup() {
    // Check if player has power-ups
    if (player.getPowerups() > 0) {
        // Use a power-up
        player.usePowerup();

        // Freeze all enemies for 3 seconds
        for (int i = 0; i < enemyCount; i++) {
            enemies[i].freeze(3.0f);  // 3 second freeze
        }

        std::cout << "Power-up activated! Enemies frozen for 3 seconds." << std::endl;
    }
}

// Process pause menu input
void Game::processPauseMenuInput(Event& event) {
    if (event.type == Event::KeyPressed) {
        switch (event.key.code) {
        case Keyboard::R:   // Resume
            resume();
            break;
        case Keyboard::S:   // Save game
            // Show save menu
            showSaveMenu = true;
            break;
        case Keyboard::L:   // Load game
            // Show load menu
            if (gameSave != nullptr) {
                savesList = gameSave->listSaves(currentUsername);
                showLoadMenu = true;
            }
            break;
        case Keyboard::M:   // Return to main menu
            // Set flag to return to menu
            returnToMenu = true;
            isRunning = false;
            break;
        case Keyboard::Q:   // Quit game entirely
            // Close the window
            window->close();
            isRunning = false;
            break;
        }
    }
    // Handle mouse clicks on menu items
    else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        Vector2f mousePos = window->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));

        // Check which option was clicked based on position
        // Resume button (at the top)
        FloatRect resumeRect(window->getSize().x / 2 - 125, window->getSize().y / 2 - 70, 250, 40);
        if (resumeRect.contains(mousePos)) {
            resume();
        }

        // Save button
        FloatRect saveRect(window->getSize().x / 2 - 125, window->getSize().y / 2 - 20, 250, 40);
        if (saveRect.contains(mousePos)) {
            showSaveMenu = true;
        }

        // Load button
        FloatRect loadRect(window->getSize().x / 2 - 125, window->getSize().y / 2 + 30, 250, 40);
        if (loadRect.contains(mousePos)) {
            if (gameSave != nullptr) {
                savesList = gameSave->listSaves(currentUsername);
                showLoadMenu = true;
            }
        }

        // Main Menu button
        FloatRect menuRect(window->getSize().x / 2 - 125, window->getSize().y / 2 + 80, 250, 40);
        if (menuRect.contains(mousePos)) {
            returnToMenu = true;
            isRunning = false;
        }

        // Quit button
        FloatRect quitRect(window->getSize().x / 2 - 125, window->getSize().y / 2 + 130, 250, 40);
        if (quitRect.contains(mousePos)) {
            window->close();
            isRunning = false;
        }
    }
}