/*
    Menu.h
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - DSA Project

    Description:
    This file contains the Menu class which handles all menu states and UI elements.
*/

#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <string>
#include "MinHeap.h"
#include "Leaderboard.h"
#include "Auth.h"
#include "Game.h"
#include "LinkedList.h"
#include "PriorityQueue.h"

using namespace sf;

// Menu states
enum class MenuState {
    MAIN_MENU,
    LOGIN,
    LOGIN_PLAYER2,  // New state for player 2 login
    REGISTER,
    SINGLE_PLAYER,
    MULTIPLAYER,
    MATCHMAKING,   // New state for matchmaking queue
    LEVEL_SELECT,
    LEADERBOARD,
    PROFILE,
    FRIEND_LIST,       // View friends and manage requests
    FRIEND_SEARCH,     // Search for players to add as friends
    OPTIONS,
    SAVE_GAME,     // New state for save game menu
    LOAD_GAME,     // New state for load game menu
    EXIT
};

class Menu {
private:
    RenderWindow* window;
    MenuState currentState;
    Auth* auth;
    Game* game;
    Leaderboard leaderboard;
    PriorityQueue matchmakingQueue;

    // Add this struct to Menu.h
    struct MatchHistory {
        char date[32];  // Date or description of the match
        bool isWin;     // Whether the player won
    };

    // UI Elements
    Font font;
    std::vector<Text> menuItems;
    std::vector<RectangleShape> buttons;
    std::vector<Text> hints;

    // Menu-specific data
    int selectedItem;
    bool isLoggedIn;
    std::string currentUser;
    std::string player2Username;  // Store second player's username
    std::string errorMessage;
    float errorTimer;

    // Save/Load game data
    LinkedList<std::string> savesList;
    int selectedSaveIndex;
    bool showConfirmationDialog;
    std::string confirmationMessage;
    bool isSaveOperation;

    // Login/Register form data
    std::string username;
    std::string password;
    std::string confirmPassword;
    std::string passwordDisplay;
    std::string confirmPasswordDisplay;
    bool isUsernameSelected;
    bool isPasswordSelected;
    bool isConfirmPasswordSelected;
    bool showPassword;
    Text usernameText;
    Text passwordText;
    Text confirmPasswordText;
    RectangleShape usernameBox;
    RectangleShape passwordBox;
    RectangleShape confirmPasswordBox;
    Text cursor;
    float cursorBlinkTime;
    bool showCursor;

    // Constants
    static const int MIN_PASSWORD_LENGTH = 8;
    static const int MAX_PASSWORD_LENGTH = 20;
    static const int MAX_USERNAME_LENGTH = 20;

    // Helper functions
    void initializeMainMenu();
    void initializeLoginMenu();
    void initializeRegisterMenu();
    void initializeMultiplayerLogin1();  // First player login
    void initializeMultiplayerLogin2();  // Second player login
    void initializeLevelSelect();
    void initializeLeaderboard();
    void initializeProfile();
    void initializeOptions();
    void initializeSaveGame();  // Initialize save game menu
    void initializeLoadGame();  // Initialize load game menu
    void initializeMatchmaking();  // Initialize matchmaking UI

    void handleMainMenuInput(Event& event);
    void handleLoginInput(Event& event);
    void handleRegisterInput(Event& event);
    void handleMultiplayerLogin1Input(Event& event);  // First player login
    void handleMultiplayerLogin2Input(Event& event);  // Second player login
    void handleLevelSelectInput(Event& event);
    void handleLeaderboardInput(Event& event);
    void handleProfileInput(Event& event);
    void handleOptionsInput(Event& event);
    void handleSaveGameInput(Event& event);  // Handle save game menu input
    void handleLoadGameInput(Event& event);  // Handle load game menu input
    void handleMatchmakingInput(Event& event);  // Handle matchmaking input

    void updateLoginForm();
    void updateRegisterForm();
    void updateCursor();
    void addFieldHint(const std::string& text, float x, float y);
    bool validatePassword(const std::string& pass);
    void togglePasswordVisibility();
    void showErrorMessage(const std::string& message);
    void showSuccessMessage(const std::string& message);
    void showConfirmation(const std::string& message, bool isSave);

    // Display functions
    void displayMainMenu();
    void displayLoginMenu();
    void displayRegisterMenu();
    void displayMultiplayerLogin1();  // First player login
    void displayMultiplayerLogin2();  // Second player login
    void displayLevelSelect();
    void displayLeaderboard();
    void displayProfile();
    void displayOptions();
    void displaySaveGame();  // Display save game menu
    void displayLoadGame();  // Display load game menu
    void displayConfirmationDialog();  // Display confirmation dialog
    void displayMatchmaking();  // Display matchmaking UI

    // Matchmaking methods
    void enterMatchmakingQueue(const std::string& username, int score);
    void removeFromMatchmakingQueue(const std::string& username);
    bool processMatchmaking();  // Process the matchmaking queue

public:
    Menu(RenderWindow* window, Auth* auth);
    ~Menu();

    // Core menu functions
    void initialize();
    void processInput(Event& event);
    void display();
    void update(float deltaTime);

    // State management
    MenuState getState() const;
    void setState(MenuState newState);

    // Save/Load game functions
    void refreshSavesList();
    bool saveGame();
    bool loadGame(const std::string& saveID);
    bool deleteSave(const std::string& saveID);

    // Getters
    bool getIsLoggedIn() const;
    std::string getCurrentUser() const;
    std::string getPlayer2Username() const { return player2Username; }

    void updateLeaderboard(const std::string& username, int score);
};

#endif // MENU_H