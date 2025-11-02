/*
    Menu.cpp
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - DSA Project

    Description:
    This file contains the implementation of the Menu class methods.
*/

#include "Menu.h"
#include<iostream>

Menu::Menu(RenderWindow* window, Auth* auth) : window(window), auth(auth) {
    currentState = MenuState::MAIN_MENU;
    selectedItem = 0;
    isLoggedIn = false;
    cursorBlinkTime = 0.0f;
    showCursor = true;
    showPassword = false;
    game = nullptr;
    errorTimer = 0.0f;
    passwordDisplay = "";
    confirmPasswordDisplay = "";
    isUsernameSelected = false;
    isPasswordSelected = false;
    isConfirmPasswordSelected = false;

    // Initialize save/load game variables
    selectedSaveIndex = 0;
    showConfirmationDialog = false;
    confirmationMessage = "";
    isSaveOperation = false;

    // Load system font
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        if (!font.loadFromFile("C:/Windows/Fonts/calibri.ttf")) {
            if (!font.loadFromFile("C:/Windows/Fonts/consolas.ttf")) {
                if (!font.loadFromFile("C:/Windows/Fonts/times.ttf")) {
                    font.loadFromFile("C:/Windows/Fonts/msmincho.ttc");
                }
            }
        }
    }

    // Initialize cursor
    cursor.setFont(font);
    cursor.setString("|");
    cursor.setCharacterSize(30);
    cursor.setFillColor(Color::Black);

    // Initialize text objects
    usernameText.setFont(font);
    passwordText.setFont(font);
    confirmPasswordText.setFont(font);
    usernameText.setCharacterSize(30);
    passwordText.setCharacterSize(30);
    confirmPasswordText.setCharacterSize(30);
    usernameText.setFillColor(Color::Black);
    passwordText.setFillColor(Color::Black);
    confirmPasswordText.setFillColor(Color::Black);

    // Initialize the main menu
    initializeMainMenu();
}

Menu::~Menu() {
    // Clean up any resources
}

void Menu::initialize() {
    initializeMainMenu();
}

void Menu::initializeMainMenu() {
    menuItems.clear();
    buttons.clear();

    // Create title
    Text title;
    title.setFont(font);
    title.setString("Xonix Game");
    title.setCharacterSize(50);
    title.setFillColor(Color::White);
    title.setPosition(250, 50);
    menuItems.push_back(title);

    // Create menu items
    std::string items[] = {
        "Single Player",
        "Multi Player",
        "Level Select",
        "Load Game",     // Added Load Game option
        "Leaderboard",
        "Profile",
        "Options",
        "Exit"
    };

    for (int i = 0; i < 8; i++) {  // Updated to 8 items
        Text text;
        text.setFont(font);
        text.setString(items[i]);
        text.setCharacterSize(30);
        text.setFillColor(Color::White);
        text.setPosition(300, 150 + i * 40);
        menuItems.push_back(text);

        RectangleShape button;
        button.setSize(Vector2f(200, 40));
        button.setPosition(290, 150 + i * 40);
        button.setFillColor(Color::Transparent);
        button.setOutlineThickness(2);
        button.setOutlineColor(Color::White);
        buttons.push_back(button);
    }
}

void Menu::initializeLoginMenu() {
    menuItems.clear();
    buttons.clear();
    hints.clear();

    // Create title
    Text title;
    title.setFont(font);
    title.setString("Login");
    title.setCharacterSize(50);
    title.setFillColor(Color::White);
    title.setPosition(300, 50);
    menuItems.push_back(title);

    // Create input fields with more visible labels
    std::string fields[] = { "Username:", "Password:" };
    for (int i = 0; i < 2; i++) {
        // Create label text
        Text labelText;
        labelText.setFont(font);
        labelText.setString(fields[i]);
        labelText.setCharacterSize(30);
        labelText.setFillColor(Color::White);
        labelText.setPosition(200, 150 + i * 60);
        menuItems.push_back(labelText);

        // Create input field
        RectangleShape field;
        field.setSize(Vector2f(300, 40));
        field.setPosition(200, 200 + i * 60);
        field.setFillColor(Color::White);
        field.setOutlineThickness(2);
        field.setOutlineColor(Color::Black);
        buttons.push_back(field);

        // Add hint text below the field
        Text hintText;
        hintText.setFont(font);
        hintText.setString(i == 0 ? "Enter your username" : "Enter your password");
        hintText.setCharacterSize(15);
        hintText.setFillColor(Color(200, 200, 200));  // Light gray color
        hintText.setPosition(200, 245 + i * 60);
        hints.push_back(hintText);
    }

    // Create login button with more visible text
    Text loginText;
    loginText.setFont(font);
    loginText.setString("Click Here to Login");
    loginText.setCharacterSize(30);
    loginText.setFillColor(Color::White);
    loginText.setPosition(250, 350);
    menuItems.push_back(loginText);

    RectangleShape loginButton;
    loginButton.setSize(Vector2f(300, 50));  // Made button larger
    loginButton.setPosition(240, 350);
    loginButton.setFillColor(Color(0, 100, 0));  // Dark green color
    loginButton.setOutlineThickness(2);
    loginButton.setOutlineColor(Color::White);
    buttons.push_back(loginButton);

    // Create register button with more visible text
    Text registerText;
    registerText.setFont(font);
    registerText.setString("New User? Click Here to Register");
    registerText.setCharacterSize(20);
    registerText.setFillColor(Color::White);
    registerText.setPosition(250, 420);
    menuItems.push_back(registerText);

    RectangleShape registerButton;
    registerButton.setSize(Vector2f(300, 40));
    registerButton.setPosition(240, 420);
    registerButton.setFillColor(Color::Transparent);
    registerButton.setOutlineThickness(2);
    registerButton.setOutlineColor(Color::White);
    buttons.push_back(registerButton);

    // Store input boxes for reference
    usernameBox = buttons[0];
    passwordBox = buttons[1];

    // Reset input fields
    username = "";
    password = "";
    passwordDisplay = "";
    usernameText.setString("");
    passwordText.setString("");
    isUsernameSelected = false;
    isPasswordSelected = false;

    // Set up text display positions with black color for visibility
    usernameText.setFont(font);
    usernameText.setCharacterSize(30);
    usernameText.setFillColor(Color::Black);  // Changed to black
    usernameText.setPosition(210, 205);

    passwordText.setFont(font);
    passwordText.setCharacterSize(30);
    passwordText.setFillColor(Color::Black);  // Changed to black
    passwordText.setPosition(210, 265);
}

void Menu::processInput(Event& event) {
    switch (currentState) {
    case MenuState::MAIN_MENU:
        handleMainMenuInput(event);
        break;
    case MenuState::LOGIN:
        handleLoginInput(event);
        break;
    case MenuState::REGISTER:
        handleRegisterInput(event);
        break;
    case MenuState::LEVEL_SELECT:
        handleLevelSelectInput(event);
        break;
    case MenuState::LEADERBOARD:
        handleLeaderboardInput(event);
        break;
    case MenuState::PROFILE:
        handleProfileInput(event);
        break;
    case MenuState::OPTIONS:
        handleOptionsInput(event);
        break;
    case MenuState::SAVE_GAME:
        handleSaveGameInput(event);
        break;
    case MenuState::LOAD_GAME:
        handleLoadGameInput(event);
        break;
    case MenuState::MATCHMAKING:
        handleMatchmakingInput(event);
        break;
    default:
        break;
    }
}

void Menu::handleMainMenuInput(Event& event) {
    if (event.type == Event::KeyPressed) {
        switch (event.key.code) {
        case Keyboard::Up:
            if (selectedItem > 1) {  // Start from 1 since we've defined menu items to start at index 1
                selectedItem--;
                std::cout << "Selected menu item: " << selectedItem << std::endl;
            }
            else if (selectedItem == 0) {
                // Wrap around to the bottom if at the top
                selectedItem = 8;  // Updated to 8 total items
                std::cout << "Selected menu item: " << selectedItem << std::endl;
            }
            break;
        case Keyboard::Down:
            if (selectedItem < 8) {  // Updated to 8 total items
                selectedItem++;
                std::cout << "Selected menu item: " << selectedItem << std::endl;
            }
            else if (selectedItem == 8) {  // Updated to 8 total items
                // Wrap around to the top if at the bottom
                selectedItem = 1;  // First selectable item
                std::cout << "Selected menu item: " << selectedItem << std::endl;
            }
            break;
        case Keyboard::Return:  // Enter key
            std::cout << "Selected menu item: " << selectedItem << std::endl;
            switch (selectedItem) {
            case 1:  // Single Player
                if (!isLoggedIn) {
                    setState(MenuState::LOGIN);
                }
                else {
                    // Initialize and start single player game
                    game = new Game();
                    game->initialize();
                    game->setCurrentUsername(currentUser);
                    game->run();
                    updateLeaderboard(currentUser, game->getFinalScore());
                    delete game;
                    game = nullptr;
                    setState(MenuState::MAIN_MENU);
                }
                break;
            case 2:  // Multi Player
                if (!isLoggedIn) {
                    setState(MenuState::LOGIN);
                }
                else {
                    // Enter matchmaking queue with player's score
                    int playerScore = auth->getPlayerScore(currentUser);
                    enterMatchmakingQueue(currentUser, playerScore);
                }
                break;
            case 3:  // Level Select
                if (!isLoggedIn) {
                    setState(MenuState::LOGIN);
                }
                else {
                    setState(MenuState::LEVEL_SELECT);
                }
                break;
            case 4:  // Load Game
                if (!isLoggedIn) {
                    setState(MenuState::LOGIN);
                }
                else {
                    setState(MenuState::LOAD_GAME);
                }
                break;
            case 5:  // Leaderboard
                setState(MenuState::LEADERBOARD);
                break;
            case 6:  // Profile
                if (!isLoggedIn) {
                    setState(MenuState::LOGIN);
                }
                else {
                    setState(MenuState::PROFILE);
                }
                break;
            case 7:  // Options
                setState(MenuState::OPTIONS);
                break;
            case 8:  // Exit
                window->close();
                break;
            }
            break;
        }
    }
    else if (event.type == Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
            // Scroll up
            if (event.mouseWheelScroll.delta > 0) {
                if (selectedItem > 1) {
                    selectedItem--;
                    std::cout << "Mouse wheel scrolled up, selected item: " << selectedItem << std::endl;
                }
                else if (selectedItem <= 1) {
                    // Wrap around to the bottom if at the top
                    selectedItem = 8;
                    std::cout << "Mouse wheel scrolled up, selected item: " << selectedItem << std::endl;
                }
            }
            // Scroll down
            else if (event.mouseWheelScroll.delta < 0) {
                if (selectedItem < 8) {
                    selectedItem++;
                    std::cout << "Mouse wheel scrolled down, selected item: " << selectedItem << std::endl;
                }
                else if (selectedItem >= 8) {
                    // Wrap around to the top if at the bottom
                    selectedItem = 1;
                    std::cout << "Mouse wheel scrolled down, selected item: " << selectedItem << std::endl;
                }
            }
        }
    }

    // Add mouse click support for menu items
    // Add mouse click support for menu items
    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        Vector2f mousePos = window->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));

        // Calculate the vertical spacing based on available space
        float windowWidth = static_cast<float>(window->getSize().x);
        float windowHeight = static_cast<float>(window->getSize().y);

        int totalItems = 8; // Number of menu items
        float menuItemHeight = 40;
        float availableHeight = windowHeight - 120; // Total height minus header
        float menuSpacing = min(10.0f, max(3.0f, (availableHeight - totalItems * menuItemHeight) / (totalItems - 1)));

        float menuStartY = 120; // Start just below the divider
        float menuWidth = 300;

        for (int i = 0; i < 8; i++) {  // Updated to 8 items
            float itemY = menuStartY + i * (menuItemHeight + menuSpacing);
            FloatRect menuItemBounds(
                (windowWidth - menuWidth) / 2,
                itemY,
                menuWidth,
                menuItemHeight
            );

            if (menuItemBounds.contains(mousePos)) {
                // Update selected item and trigger selection
                selectedItem = i + 1;

                // Simulate Enter key press by executing the same code
                Event enterEvent;
                enterEvent.type = Event::KeyPressed;
                enterEvent.key.code = Keyboard::Return;
                handleMainMenuInput(enterEvent);
                break;
            }
        }
    }
}

void Menu::display() {
    window->clear(Color(50, 50, 50));  // Dark gray background

    switch (currentState) {
    case MenuState::MAIN_MENU:
        displayMainMenu();
        break;
    case MenuState::LOGIN:
        displayLoginMenu();
        break;
    case MenuState::REGISTER:
        displayRegisterMenu();
        break;
    case MenuState::LEVEL_SELECT:
        displayLevelSelect();
        break;
    case MenuState::LEADERBOARD:
        displayLeaderboard();
        break;
    case MenuState::PROFILE:
        displayProfile();
        break;
    case MenuState::OPTIONS:
        displayOptions();
        break;
    case MenuState::SAVE_GAME:
        displaySaveGame();
        break;
    case MenuState::LOAD_GAME:
        displayLoadGame();
        break;
    case MenuState::MATCHMAKING:
        displayMatchmaking();
        break;
    default:
        break;
    }

    // Display confirmation dialog over other elements if active
    if (showConfirmationDialog) {
        displayConfirmationDialog();
    }

    window->display();
}

void Menu::displayMainMenu() {
    // Get current window dimensions
    float windowWidth = static_cast<float>(window->getSize().x);
    float windowHeight = static_cast<float>(window->getSize().y);

    // Draw a nice background with gradient effect
    RectangleShape background(Vector2f(windowWidth, windowHeight));
    background.setFillColor(Color(30, 30, 40)); // Darker background
    window->draw(background);

    // Add a decorative header bar
    RectangleShape headerBar(Vector2f(windowWidth, 80));
    headerBar.setFillColor(Color(50, 50, 70));
    window->draw(headerBar);

    // Draw title with shadow effect for depth
    Text shadowText;
    shadowText.setFont(font);
    shadowText.setString("Xonix Game");
    shadowText.setCharacterSize(50);
    shadowText.setFillColor(Color(20, 20, 30, 200));
    FloatRect shadowBounds = shadowText.getLocalBounds();
    shadowText.setPosition((windowWidth - shadowBounds.width) / 2 + 3, 37); // Position slightly higher
    window->draw(shadowText);

    // Main title with glow effect
    Text titleText;
    titleText.setFont(font);
    titleText.setString("Xonix Game");
    titleText.setCharacterSize(50);
    titleText.setFillColor(Color(220, 220, 50)); // Bright yellow
    titleText.setStyle(Text::Bold);
    FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition((windowWidth - titleBounds.width) / 2, 34); // Position slightly higher
    window->draw(titleText);

    // Draw decorative line beneath title
    RectangleShape divider(Vector2f(400, 2));
    divider.setFillColor(Color(220, 220, 50, 150)); // Semi-transparent yellow
    divider.setPosition((windowWidth - 400) / 2, 100); // Position slightly higher
    window->draw(divider);

    // Calculate the vertical spacing based on available space
    // Calculate how much space we need for all menu items
    int totalItems = 8; // Number of menu items
    float requiredHeight = totalItems * 40 + (totalItems - 1) * 10; // Menu item height (40) + spacing (10)
    float availableHeight = windowHeight - 120; // Total height minus header

    // Adjust spacing based on available height
    float menuItemHeight = 40;
    float menuSpacing = min(10.0f, max(3.0f, (availableHeight - totalItems * menuItemHeight) / (totalItems - 1)));

    // Start menu items higher
    float menuStartY = 120; // Start just below the divider
    float menuWidth = 300;

    // Draw menu items with better spacing and visual appeal
    for (int i = 0; i < 8; i++) {  // Updated to 8 items
        // Calculate position - compact layout
        float itemY = menuStartY + i * (menuItemHeight + menuSpacing);

        // Create menu box
        RectangleShape menuBox(Vector2f(menuWidth, menuItemHeight));
        menuBox.setPosition((windowWidth - menuWidth) / 2, itemY);

        // Style based on selection
        if (i + 1 == selectedItem) {
            // Selected item styling
            menuBox.setFillColor(Color(80, 80, 100, 200));
            menuBox.setOutlineThickness(2);
            menuBox.setOutlineColor(Color(220, 220, 50)); // Yellow border

            // Add highlight effects
            RectangleShape glow(Vector2f(menuWidth + 10, menuItemHeight + 10));
            glow.setPosition((windowWidth - menuWidth - 10) / 2, itemY - 5);
            glow.setFillColor(Color(220, 220, 50, 30)); // Very light yellow glow
            window->draw(glow);
        }
        else {
            // Unselected item styling
            menuBox.setFillColor(Color(60, 60, 80, 150));
            menuBox.setOutlineThickness(1);
            menuBox.setOutlineColor(Color(150, 150, 150, 100));
        }
        window->draw(menuBox);

        // Create menu text
        Text menuText;
        menuText.setFont(font);

        // Use menu items from the fixed array instead of dynamic menuItems
        std::string items[] = {
            "Single Player",
            "Multi Player",
            "Level Select",
            "Load Game",     // Added Load Game option
            "Leaderboard",
            "Profile",
            "Options",
            "Exit"
        };
        menuText.setString(items[i]);

        menuText.setCharacterSize(i + 1 == selectedItem ? 30 : 28);
        menuText.setFillColor(i + 1 == selectedItem ? Color(255, 255, 100) : Color(220, 220, 220));
        menuText.setStyle(i + 1 == selectedItem ? Text::Bold : Text::Regular);

        // Center text in box
        FloatRect textBounds = menuText.getLocalBounds();
        menuText.setPosition(
            (windowWidth - textBounds.width) / 2,
            itemY + (menuItemHeight - textBounds.height) / 2 - textBounds.top
        );
        window->draw(menuText);
    }

    // Draw login status with a more attractive box
    RectangleShape statusBox(Vector2f(200, 30));
    statusBox.setPosition(windowWidth - 220, 10);
    statusBox.setFillColor(isLoggedIn ? Color(40, 100, 40, 200) : Color(100, 40, 40, 200));
    statusBox.setOutlineThickness(1);
    statusBox.setOutlineColor(Color(200, 200, 200, 100));
    window->draw(statusBox);

    Text loginStatus;
    loginStatus.setFont(font);
    loginStatus.setString(isLoggedIn ? "Logged in as: " + currentUser : "Not logged in");
    loginStatus.setCharacterSize(16);
    loginStatus.setFillColor(Color(240, 240, 240));
    FloatRect statusBounds = loginStatus.getLocalBounds();
    loginStatus.setPosition(
        windowWidth - 220 + (200 - statusBounds.width) / 2,
        10 + (30 - statusBounds.height) / 2 - statusBounds.top
    );
    window->draw(loginStatus);
}

void Menu::setState(MenuState newState) {
    currentState = newState;
    selectedItem = 0;  // Reset selection when changing states

    switch (newState) {
    case MenuState::MAIN_MENU:
        initializeMainMenu();
        break;
    case MenuState::LOGIN:
        initializeLoginMenu();
        break;
    case MenuState::REGISTER:
        initializeRegisterMenu();
        break;
    case MenuState::LEVEL_SELECT:
        initializeLevelSelect();
        break;
    case MenuState::LEADERBOARD:
        initializeLeaderboard();
        break;
    case MenuState::PROFILE:
        initializeProfile();
        break;
    case MenuState::OPTIONS:
        initializeOptions();
        break;
    case MenuState::SAVE_GAME:
        initializeSaveGame();
        break;
    case MenuState::LOAD_GAME:
        initializeLoadGame();
        break;
    case MenuState::MATCHMAKING:
        initializeMatchmaking();
        break;
    }
}

bool Menu::getIsLoggedIn() const {
    return isLoggedIn;
}

std::string Menu::getCurrentUser() const {
    return currentUser;
}

// Initialize other menu states
void Menu::initializeRegisterMenu() {
    menuItems.clear();
    buttons.clear();
    hints.clear();

    // Create back button
    Text backText;
    backText.setFont(font);
    backText.setString("Back to Login");
    backText.setCharacterSize(20);
    backText.setFillColor(Color::White);
    backText.setPosition(20, 20);
    menuItems.push_back(backText);

    RectangleShape backButton;
    backButton.setSize(Vector2f(150, 30));
    backButton.setPosition(20, 20);
    backButton.setFillColor(Color::Transparent);
    backButton.setOutlineThickness(2);
    backButton.setOutlineColor(Color::White);
    buttons.push_back(backButton);

    // Create title
    Text title;
    title.setFont(font);
    title.setString("Register");
    title.setCharacterSize(50);
    title.setFillColor(Color::White);
    title.setPosition(280, 50);
    menuItems.push_back(title);

    // Create input fields
    std::string fields[] = { "Username:", "Password:", "Confirm Password:" };
    for (int i = 0; i < 3; i++) {
        Text text;
        text.setFont(font);
        text.setString(fields[i]);
        text.setCharacterSize(30);
        text.setFillColor(Color::White);
        text.setPosition(200, 150 + i * 60);
        menuItems.push_back(text);

        RectangleShape field;
        field.setSize(Vector2f(300, 40));
        field.setPosition(200, 200 + i * 60);
        field.setFillColor(Color::White);
        field.setOutlineThickness(2);
        field.setOutlineColor(Color::Black);
        buttons.push_back(field);
    }

    // Add field hints
    addFieldHint("Enter your username (3-20 characters)", 200, 245);
    addFieldHint("Password Requirements:", 200, 305);
    addFieldHint("• At least 6 characters long", 220, 325);
    addFieldHint("• At least one uppercase letter (A-Z)", 220, 345);
    addFieldHint("• At least one lowercase letter (a-z)", 220, 365);
    addFieldHint("• At least one number (0-9)", 220, 385);
    addFieldHint("• At least one special character (!@#$%^&* etc.)", 220, 405);
    addFieldHint("Re-enter your password to confirm", 200, 425);

    // Create show password button
    Text showPasswordText;
    showPasswordText.setFont(font);
    showPasswordText.setString("Show Password");
    showPasswordText.setCharacterSize(20);
    showPasswordText.setFillColor(Color::White);
    showPasswordText.setPosition(520, 200);
    menuItems.push_back(showPasswordText);

    RectangleShape showPasswordButton;
    showPasswordButton.setSize(Vector2f(150, 30));
    showPasswordButton.setPosition(520, 200);
    showPasswordButton.setFillColor(Color::Transparent);
    showPasswordButton.setOutlineThickness(2);
    showPasswordButton.setOutlineColor(Color::White);
    buttons.push_back(showPasswordButton);

    // Create register button
    RectangleShape registerButton;
    registerButton.setSize(Vector2f(500, 80));
    registerButton.setPosition(150, 430);
    registerButton.setFillColor(Color(255, 0, 0));
    registerButton.setOutlineThickness(5);
    registerButton.setOutlineColor(Color::Yellow);
    buttons.push_back(registerButton);

    Text registerText;
    registerText.setFont(font);
    registerText.setString("CLICK HERE TO REGISTER");
    registerText.setCharacterSize(40);
    registerText.setFillColor(Color::White);
    registerText.setPosition(180, 445);
    menuItems.push_back(registerText);

    // Initialize text input fields
    username = "";
    password = "";
    confirmPassword = "";
    isUsernameSelected = false;
    isPasswordSelected = false;
    isConfirmPasswordSelected = false;
    showPassword = false;

    // Set up text display with black color for visibility
    usernameText.setFont(font);
    usernameText.setCharacterSize(30);
    usernameText.setFillColor(Color::Black);  // Changed to black
    usernameText.setPosition(210, 205);

    passwordText.setFont(font);
    passwordText.setCharacterSize(30);
    passwordText.setFillColor(Color::Black);  // Changed to black
    passwordText.setPosition(210, 265);

    confirmPasswordText.setFont(font);
    confirmPasswordText.setCharacterSize(30);
    confirmPasswordText.setFillColor(Color::Black);  // Changed to black
    confirmPasswordText.setPosition(210, 325);

    // Store input boxes for reference
    usernameBox = buttons[1];
    passwordBox = buttons[2];
    confirmPasswordBox = buttons[3];
}

void Menu::handleRegisterInput(Event& event) {
    if (event.type == Event::MouseButtonPressed) {
        Vector2f mousePos = window->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));

        // Check username box
        if (usernameBox.getGlobalBounds().contains(mousePos)) {
            std::cout << "Username field selected" << std::endl;
            isUsernameSelected = true;
            isPasswordSelected = false;
            isConfirmPasswordSelected = false;
        }
        // Check password box
        else if (passwordBox.getGlobalBounds().contains(mousePos)) {
            std::cout << "Password field selected" << std::endl;
            isUsernameSelected = false;
            isPasswordSelected = true;
            isConfirmPasswordSelected = false;
        }
        // Check confirm password box
        else if (confirmPasswordBox.getGlobalBounds().contains(mousePos)) {
            std::cout << "Confirm password field selected" << std::endl;
            isUsernameSelected = false;
            isPasswordSelected = false;
            isConfirmPasswordSelected = true;
        }
        // Check register button (index 5)
        else if (buttons[5].getGlobalBounds().contains(mousePos)) {
            std::cout << "\n=== Registration Attempt ===" << std::endl;
            std::cout << "Username: " << username << std::endl;
            std::cout << "Password length: " << password.length() << std::endl;
            std::cout << "Confirm password length: " << confirmPassword.length() << std::endl;

            if (username.empty() || password.empty() || confirmPassword.empty()) {
                std::cout << "Registration failed: Please fill all fields" << std::endl;
                errorMessage = "Please fill all fields";
                errorTimer = 3.0f;
            }
            else if (password != confirmPassword) {
                std::cout << "Registration failed: Passwords do not match" << std::endl;
                errorMessage = "Passwords do not match";
                errorTimer = 3.0f;
            }
            else if (!validatePassword(password)) {
                std::cout << "Registration failed: Password does not meet requirements" << std::endl;
                errorMessage = "Password does not meet requirements";
                errorTimer = 3.0f;
            }
            else if (auth->registerUser(username, password)) {
                std::cout << "Registration successful!" << std::endl;
                isLoggedIn = true;
                currentUser = username;
                // Initialize and start single player game
                game = new Game();
                game->initialize();
                game->setCurrentUsername(currentUser); // Set username before running game
                game->run();
                updateLeaderboard(currentUser, game->getFinalScore());
                delete game;
                game = nullptr;
                setState(MenuState::MAIN_MENU);
            }
            else {
                std::cout << "Registration failed: Username already exists" << std::endl;
                errorMessage = "Username already exists";
                errorTimer = 3.0f;
            }
        }
        // Check back button (index 0)
        else if (buttons[0].getGlobalBounds().contains(mousePos)) {
            std::cout << "Returning to login screen" << std::endl;
            setState(MenuState::LOGIN);
        }
        // Check show password button (index 4)
        else if (buttons[4].getGlobalBounds().contains(mousePos)) {
            std::cout << "Toggling password visibility" << std::endl;
            togglePasswordVisibility();
        }
        else {
            isUsernameSelected = false;
            isPasswordSelected = false;
            isConfirmPasswordSelected = false;
        }
    }
    else if (event.type == Event::TextEntered) {
        if (isUsernameSelected) {
            if (event.text.unicode == '\b' && !username.empty()) {
                username.pop_back();
                std::cout << "Username backspace: " << username << std::endl;
            }
            else if (event.text.unicode < 128 && username.length() < MAX_USERNAME_LENGTH) {
                username += static_cast<char>(event.text.unicode);
                std::cout << "Username updated: " << username << std::endl;
            }
            usernameText.setString(username);
            usernameText.setFillColor(Color::Black);
            usernameText.setPosition(210, 205);
        }
        else if (isPasswordSelected) {
            if (event.text.unicode == '\b' && !password.empty()) {
                password.pop_back();
                passwordDisplay.pop_back();
                std::cout << "Password length: " << password.length() << std::endl;
            }
            else if (event.text.unicode < 128 && password.length() < MAX_PASSWORD_LENGTH) {
                password += static_cast<char>(event.text.unicode);
                passwordDisplay += '*';
                std::cout << "Password length: " << password.length() << std::endl;
            }
            passwordText.setString(showPassword ? password : passwordDisplay);
            passwordText.setFillColor(Color::Black);
            passwordText.setPosition(210, 265);
        }
        else if (isConfirmPasswordSelected) {
            if (event.text.unicode == '\b' && !confirmPassword.empty()) {
                confirmPassword.pop_back();
                confirmPasswordDisplay.pop_back();
                std::cout << "Confirm password length: " << confirmPassword.length() << std::endl;
            }
            else if (event.text.unicode < 128 && confirmPassword.length() < MAX_PASSWORD_LENGTH) {
                confirmPassword += static_cast<char>(event.text.unicode);
                confirmPasswordDisplay += '*';
                std::cout << "Confirm password length: " << confirmPassword.length() << std::endl;
            }
            confirmPasswordText.setString(showPassword ? confirmPassword : confirmPasswordDisplay);
            confirmPasswordText.setFillColor(Color::Black);
            confirmPasswordText.setPosition(210, 325);
        }
    }
}

void Menu::togglePasswordVisibility() {
    showPassword = !showPassword;
    if (showPassword) {
        passwordText.setString(password);
        confirmPasswordText.setString(confirmPassword);
    }
    else {
        passwordText.setString(std::string(password.length(), '*'));
        confirmPasswordText.setString(std::string(confirmPassword.length(), '*'));
    }
}

void Menu::showErrorMessage(const std::string& message) {
    Text errorText;
    errorText.setFont(font);
    errorText.setString(message);
    errorText.setCharacterSize(20);
    errorText.setFillColor(Color::Red);
    errorText.setPosition(200, 450);
    menuItems.push_back(errorText);
}

void Menu::showSuccessMessage(const std::string& message) {
    // Create success message text
    Text successText;
    successText.setFont(font);
    successText.setString(message);
    successText.setCharacterSize(20);
    successText.setFillColor(Color::Green);
    successText.setPosition(250, 450);
    menuItems.push_back(successText);

    // Display the message
    display();
    window->display();
}

void Menu::initializeLevelSelect() {
    // Create level selection menu
}

void Menu::initializeLeaderboard() {
    menuItems.clear();
    buttons.clear();

    // Create back button
    Text backText;
    backText.setFont(font);
    backText.setString("Back to Main Menu");
    backText.setCharacterSize(20);
    backText.setFillColor(Color::White);
    backText.setPosition(20, 20);
    menuItems.push_back(backText);

    RectangleShape backButton;
    backButton.setSize(Vector2f(200, 40));
    backButton.setPosition(20, 20);
    backButton.setFillColor(Color::Transparent);
    backButton.setOutlineThickness(2);
    backButton.setOutlineColor(Color::White);
    buttons.push_back(backButton);

    // Create title
    Text title;
    title.setFont(font);
    title.setString("Leaderboard");
    title.setCharacterSize(50);
    title.setFillColor(Color::White);
    title.setPosition(280, 50);
    menuItems.push_back(title);

    // Fetch scores from leaderboard
    int count;
    PlayerScore* scores = leaderboard.getTopScores(count);
    std::cout << "Displaying leaderboard with " << count << " scores" << std::endl;

    // Add all scores to the debug output
    for (int i = 0; i < count; i++) {
        std::cout << "  Leaderboard entry " << i + 1 << ": " << scores[i].username << " - " << scores[i].score << std::endl;
    }

    // Display column headers
    Text headerRank;
    headerRank.setFont(font);
    headerRank.setString("Rank");
    headerRank.setCharacterSize(24);
    headerRank.setFillColor(Color::Yellow);
    headerRank.setPosition(150, 120);
    menuItems.push_back(headerRank);

    Text headerName;
    headerName.setFont(font);
    headerName.setString("Player");
    headerName.setCharacterSize(24);
    headerName.setFillColor(Color::Yellow);
    headerName.setPosition(250, 120);
    menuItems.push_back(headerName);

    Text headerScore;
    headerScore.setFont(font);
    headerScore.setString("Score");
    headerScore.setCharacterSize(24);
    headerScore.setFillColor(Color::Yellow);
    headerScore.setPosition(450, 120);
    menuItems.push_back(headerScore);

    // Add a separator line
    RectangleShape separator;
    separator.setSize(Vector2f(500, 2));
    separator.setPosition(150, 155);
    separator.setFillColor(Color::Yellow);
    buttons.push_back(separator);

    // Display each score
    for (int i = 0; i < count; i++) {
        // Rank
        Text rank;
        rank.setFont(font);
        rank.setString(std::to_string(i + 1) + ".");
        rank.setCharacterSize(20);
        rank.setFillColor(Color::White);
        rank.setPosition(150, 170 + i * 35);
        menuItems.push_back(rank);

        // Player name
        Text name;
        name.setFont(font);
        name.setString(scores[i].username);
        name.setCharacterSize(20);
        name.setFillColor(Color::White);
        name.setPosition(250, 170 + i * 35);
        menuItems.push_back(name);

        // Score
        Text score;
        score.setFont(font);
        std::string scoreStr = std::to_string(scores[i].score);
        std::cout << "  Setting score text for " << scores[i].username << " to: " << scoreStr << std::endl;
        score.setString(scoreStr);
        score.setCharacterSize(20);
        score.setFillColor(Color::White);
        score.setPosition(450, 170 + i * 35);
        menuItems.push_back(score);
    }

    // If no scores yet
    if (count == 0) {
        Text noScores;
        noScores.setFont(font);
        noScores.setString("No scores yet! Play a game to set a record.");
        noScores.setCharacterSize(24);
        noScores.setFillColor(Color::White);
        noScores.setPosition(200, 250);
        menuItems.push_back(noScores);
    }

    // Clean up
    delete[] scores;
}

void Menu::updateLeaderboard(const std::string& username, int score) {
    leaderboard.updateScore(username, score);
}

void Menu::initializeProfile() {
    menuItems.clear();
    buttons.clear();

    // Create title
    Text title;
    title.setFont(font);
    title.setString("Profile");
    title.setCharacterSize(50);
    title.setFillColor(Color::White);
    title.setPosition(280, 50);
    menuItems.push_back(title);

    // Create back button
    Text backText;
    backText.setFont(font);
    backText.setString("Back to Main Menu");
    backText.setCharacterSize(20);
    backText.setFillColor(Color::White);
    backText.setPosition(20, 20);
    menuItems.push_back(backText);

    RectangleShape backButton;
    backButton.setSize(Vector2f(200, 40));
    backButton.setPosition(20, 20);
    backButton.setFillColor(Color::Transparent);
    backButton.setOutlineThickness(2);
    backButton.setOutlineColor(Color::White);
    buttons.push_back(backButton);

    // Hardcoded stats for demonstration
    int wins = 3;
    int losses = 1;
    int totalPoints = 1250;

    // Create profile card section
    float cardWidth = 700;
    float cardHeight = 450;
    float cardX = (window->getSize().x - cardWidth) / 2;
    float cardY = 120;

    RectangleShape profileCard;
    profileCard.setSize(Vector2f(cardWidth, cardHeight));
    profileCard.setPosition(cardX, cardY);
    profileCard.setFillColor(Color(40, 40, 60, 180));
    profileCard.setOutlineThickness(2);
    profileCard.setOutlineColor(Color(80, 80, 100));
    buttons.push_back(profileCard);

    // Username section
    Text usernameHeader;
    usernameHeader.setFont(font);
    usernameHeader.setString("Username:");
    usernameHeader.setCharacterSize(24);
    usernameHeader.setFillColor(Color(220, 220, 50));
    usernameHeader.setPosition(cardX + 30, cardY + 30);
    menuItems.push_back(usernameHeader);

    Text usernameValue;
    usernameValue.setFont(font);
    usernameValue.setString(currentUser);
    usernameValue.setCharacterSize(30);
    usernameValue.setStyle(Text::Bold);
    usernameValue.setFillColor(Color::White);
    usernameValue.setPosition(cardX + 150, cardY + 30);
    menuItems.push_back(usernameValue);

    // Stats section
    Text statsHeader;
    statsHeader.setFont(font);
    statsHeader.setString("Stats:");
    statsHeader.setCharacterSize(24);
    statsHeader.setFillColor(Color(220, 220, 50));
    statsHeader.setPosition(cardX + 30, cardY + 80);
    menuItems.push_back(statsHeader);

    // Wins
    Text winsText;
    winsText.setFont(font);
    winsText.setString("Wins:");
    winsText.setCharacterSize(22);
    winsText.setFillColor(Color::White);
    winsText.setPosition(cardX + 50, cardY + 120);
    menuItems.push_back(winsText);

    Text winsValue;
    winsValue.setFont(font);
    winsValue.setString(std::to_string(wins));
    winsValue.setCharacterSize(22);
    winsValue.setFillColor(Color::Green);
    winsValue.setPosition(cardX + 150, cardY + 120);
    menuItems.push_back(winsValue);

    // Losses
    Text lossesText;
    lossesText.setFont(font);
    lossesText.setString("Losses:");
    lossesText.setCharacterSize(22);
    lossesText.setFillColor(Color::White);
    lossesText.setPosition(cardX + 50, cardY + 150);
    menuItems.push_back(lossesText);

    Text lossesValue;
    lossesValue.setFont(font);
    lossesValue.setString(std::to_string(losses));
    lossesValue.setCharacterSize(22);
    lossesValue.setFillColor(Color::Red);
    lossesValue.setPosition(cardX + 150, cardY + 150);
    menuItems.push_back(lossesValue);

    // Win ratio
    float winRatio = (wins + losses > 0) ? (float)wins / (wins + losses) : 0.0f;
    char ratioBuffer[16];
    sprintf_s(ratioBuffer, "%.1f%%", winRatio * 100);

    Text ratioText;
    ratioText.setFont(font);
    ratioText.setString("Win Ratio:");
    ratioText.setCharacterSize(22);
    ratioText.setFillColor(Color::White);
    ratioText.setPosition(cardX + 50, cardY + 180);
    menuItems.push_back(ratioText);

    Text ratioValue;
    ratioValue.setFont(font);
    ratioValue.setString(ratioBuffer);
    ratioValue.setCharacterSize(22);
    ratioValue.setFillColor(Color(220, 220, 50));
    ratioValue.setPosition(cardX + 150, cardY + 180);
    menuItems.push_back(ratioValue);

    // Total points
    Text pointsText;
    pointsText.setFont(font);
    pointsText.setString("Total Points:");
    pointsText.setCharacterSize(22);
    pointsText.setFillColor(Color::White);
    pointsText.setPosition(cardX + 50, cardY + 210);
    menuItems.push_back(pointsText);

    Text pointsValue;
    pointsValue.setFont(font);
    pointsValue.setString(std::to_string(totalPoints));
    pointsValue.setCharacterSize(22);
    pointsValue.setFillColor(Color(220, 220, 50));
    pointsValue.setPosition(cardX + 190, cardY + 210);
    menuItems.push_back(pointsValue);

    // Match history section with hardcoded matches
    Text historyHeader;
    historyHeader.setFont(font);
    historyHeader.setString("Recent Matches:");
    historyHeader.setCharacterSize(24);
    historyHeader.setFillColor(Color(220, 220, 50));
    historyHeader.setPosition(cardX + 30, cardY + 250);
    menuItems.push_back(historyHeader);

    // Hardcoded match history
    const char* matchResults[3][2] = {
        {"05/15/2025", "Win"},
        {"05/12/2025", "Win"},
        {"05/10/2025", "Loss"}
    };

    for (int i = 0; i < 3; i++) {
        Text matchText;
        matchText.setFont(font);
        matchText.setString(std::string(matchResults[i][0]) + " - " + std::string(matchResults[i][1]));
        matchText.setCharacterSize(18);
        matchText.setFillColor(std::string(matchResults[i][1]) == "Win" ? Color::Green : Color::Red);
        matchText.setPosition(cardX + 50, cardY + 290 + i * 30);
        menuItems.push_back(matchText);
    }

    // Friends list section on the right side
    Text friendsHeader;
    friendsHeader.setFont(font);
    friendsHeader.setString("Friends:");
    friendsHeader.setCharacterSize(24);
    friendsHeader.setFillColor(Color(220, 220, 50));
    friendsHeader.setPosition(cardX + 400, cardY + 80);
    menuItems.push_back(friendsHeader);

    // Hardcoded friends list
    const char* friends[] = {
        "mahnoor",
        "abdulrehman",
        "rafay"
    };
    int friendCount = 3;

    // Display friends
    for (int i = 0; i < friendCount; i++) {
        Text friendText;
        friendText.setFont(font);
        friendText.setString(friends[i]);
        friendText.setCharacterSize(18);
        friendText.setFillColor(Color::White);
        friendText.setPosition(cardX + 420, cardY + 120 + i * 30);
        menuItems.push_back(friendText);
    }

    // Add friend text field
    RectangleShape addFriendField;
    addFriendField.setSize(Vector2f(200, 40));
    addFriendField.setPosition(cardX + 400, cardY + 230);
    addFriendField.setFillColor(Color(60, 60, 80));
    addFriendField.setOutlineThickness(2);
    addFriendField.setOutlineColor(Color(100, 100, 150));
    buttons.push_back(addFriendField);

    Text addFriendLabel;
    addFriendLabel.setFont(font);
    addFriendLabel.setString("Add Friend:");
    addFriendLabel.setCharacterSize(18);
    addFriendLabel.setFillColor(Color::White);
    addFriendLabel.setPosition(cardX + 400, cardY + 210);
    menuItems.push_back(addFriendLabel);

    // Add friend button
    RectangleShape addButton;
    addButton.setSize(Vector2f(80, 40));
    addButton.setPosition(cardX + 610, cardY + 230);
    addButton.setFillColor(Color(60, 100, 60));
    addButton.setOutlineThickness(2);
    addButton.setOutlineColor(Color::White);
    buttons.push_back(addButton);

    Text addButtonText;
    addButtonText.setFont(font);
    addButtonText.setString("Add");
    addButtonText.setCharacterSize(18);
    addButtonText.setFillColor(Color::White);
    addButtonText.setPosition(cardX + 630, cardY + 240);
    menuItems.push_back(addButtonText);
}

void Menu::handleProfileInput(Event& event) {
    static bool isFriendInputSelected = false;
    static std::string friendUsername = "";

    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Escape) {
            setState(MenuState::MAIN_MENU);
        }
    }
    else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        Vector2f mousePos = window->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));

        // Check if back button clicked
        if (buttons[0].getGlobalBounds().contains(mousePos)) {
            setState(MenuState::MAIN_MENU);
        }

        // Check if friend name input field clicked
        if (buttons.size() >= 3 && buttons[2].getGlobalBounds().contains(mousePos)) {
            isFriendInputSelected = true;
            std::cout << "Friend input field selected" << std::endl;
        }
        // Check if add friend button clicked
        else if (buttons.size() >= 4 && buttons[3].getGlobalBounds().contains(mousePos)) {
            // Only add friend if username isn't empty
            if (!friendUsername.empty()) {
                // Here we would normally add the friend
                showSuccessMessage("Friend request sent to: " + friendUsername);
                // Reset the input field after adding
                friendUsername = "";
                isFriendInputSelected = false;
            }
        }
        else {
            isFriendInputSelected = false;
        }
    }
    // Handle text input for the friend username field
    else if (event.type == Event::TextEntered && isFriendInputSelected) {
        if (event.text.unicode == '\b' && !friendUsername.empty()) {
            // Handle backspace
            friendUsername.pop_back();
        }
        else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
            // Handle enter key - submit friend request
            if (!friendUsername.empty()) {
                showSuccessMessage("Friend request sent to: " + friendUsername);
                friendUsername = "";
                isFriendInputSelected = false;
            }
        }
        else if (event.text.unicode < 128 && friendUsername.length() < 20) {
            // Add character to friend username (limit to 20 chars)
            friendUsername += static_cast<char>(event.text.unicode);
        }

        // Update the text display for the friend input field
        if (buttons.size() >= 3) {
            // Find and update the friend input text in menuItems
            // This assumes the input text is at a specific position in menuItems
            // Alternatively, you could add a dedicated Text object for this
            for (auto& item : menuItems) {
                if (item.getPosition() == buttons[2].getPosition()) {
                    item.setString(friendUsername);
                    break;
                }
            }

            // If no text found, add new text element
            Text inputText;
            inputText.setFont(font);
            inputText.setString(friendUsername);
            inputText.setCharacterSize(18);
            inputText.setFillColor(Color::White);

            // Position input text inside the input box
            inputText.setPosition(buttons[2].getPosition().x + 10,
                buttons[2].getPosition().y + 10);

            // Add or update text in menuItems
            bool found = false;
            for (auto& item : menuItems) {
                if (item.getPosition().x == buttons[2].getPosition().x + 10 &&
                    item.getPosition().y == buttons[2].getPosition().y + 10) {
                    item.setString(friendUsername);
                    found = true;
                    break;
                }
            }
            if (!found) {
                menuItems.push_back(inputText);
            }
        }
    }
}

void Menu::initializeOptions() {
    // Create options menu
}

// Handle input for other menu states
void Menu::handleLoginInput(Event& event) {
    if (event.type == Event::MouseButtonPressed) {
        Vector2f mousePos = window->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));

        // Check username box
        if (usernameBox.getGlobalBounds().contains(mousePos)) {
            std::cout << "Username field selected" << std::endl;
            isUsernameSelected = true;
            isPasswordSelected = false;
        }
        // Check password box
        else if (passwordBox.getGlobalBounds().contains(mousePos)) {
            std::cout << "Password field selected" << std::endl;
            isUsernameSelected = false;
            isPasswordSelected = true;
        }
        // Check login button (index 2)
        else if (buttons[2].getGlobalBounds().contains(mousePos)) {
            std::cout << "\n=== Login Attempt ===" << std::endl;
            std::cout << "Username: " << username << std::endl;
            std::cout << "Password length: " << password.length() << std::endl;

            if (auth->login(username, password)) {
                std::cout << "Login successful!" << std::endl;
                std::cout << "Setting current user to: '" << username << "'" << std::endl;
                isLoggedIn = true;
                currentUser = username;
                setState(MenuState::MAIN_MENU);
            }
            else {
                std::cout << "Login failed: Invalid username or password" << std::endl;
                errorMessage = "Invalid username or password";
                errorTimer = 3.0f;
            }
        }
        // Check register button (index 3)
        else if (buttons[3].getGlobalBounds().contains(mousePos)) {
            std::cout << "Switching to registration screen" << std::endl;
            setState(MenuState::REGISTER);
        }
        else {
            isUsernameSelected = false;
            isPasswordSelected = false;
        }
    }
    else if (event.type == Event::TextEntered) {
        if (isUsernameSelected) {
            if (event.text.unicode == '\b' && !username.empty()) {
                username.pop_back();
                std::cout << "Username backspace: " << username << std::endl;
            }
            else if (event.text.unicode < 128 && username.length() < MAX_USERNAME_LENGTH) {
                username += static_cast<char>(event.text.unicode);
                std::cout << "Username updated: " << username << std::endl;
            }
            usernameText.setString(username);
            usernameText.setFillColor(Color::Black);
            usernameText.setPosition(210, 205);
        }
        else if (isPasswordSelected) {
            if (event.text.unicode == '\b' && !password.empty()) {
                password.pop_back();
                passwordDisplay.pop_back();
                std::cout << "Password length: " << password.length() << std::endl;
            }
            else if (event.text.unicode < 128 && password.length() < MAX_PASSWORD_LENGTH) {
                password += static_cast<char>(event.text.unicode);
                passwordDisplay += '*';
                std::cout << "Password length: " << password.length() << std::endl;
            }
            passwordText.setString(showPassword ? password : passwordDisplay);
            passwordText.setFillColor(Color::Black);
            passwordText.setPosition(210, 265);
        }
    }
}

void Menu::handleLevelSelectInput(Event& event) {
    // Handle level selection input
}

void Menu::handleLeaderboardInput(Event& event) {
    // Handle leaderboard navigation
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Escape || event.key.code == Keyboard::BackSpace) {
            setState(MenuState::MAIN_MENU);
        }
    }
    else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        Vector2f mousePos = window->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));

        // Check if back button clicked
        if (buttons[0].getGlobalBounds().contains(mousePos)) {
            setState(MenuState::MAIN_MENU);
        }
    }
    // Add mouse wheel scroll support for leaderboard scrolling if needed in the future
    else if (event.type == Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
            // Here you could implement scrolling through leaderboard entries
            // if there are more than can fit on the screen at once
            std::cout << "Mouse wheel scrolled on leaderboard" << std::endl;
        }
    }
}

void Menu::handleOptionsInput(Event& event) {
    // Handle options navigation
}

void Menu::update(float deltaTime) {
    // Update cursor blink
    cursorBlinkTime += deltaTime;
    if (cursorBlinkTime >= 0.5f) {
        showCursor = !showCursor;
        cursorBlinkTime = 0.0f;
    }

    // Update error timer
    if (errorTimer > 0) {
        errorTimer -= deltaTime;
    }

    // Process matchmaking if in that state
    if (currentState == MenuState::MATCHMAKING) {
        // Try to find a match every update cycle
        processMatchmaking();
    }
}

void Menu::updateCursor() {
    if (!showCursor) {
        cursor.setString("");
        return;
    }

    if (currentState == MenuState::LOGIN || currentState == MenuState::REGISTER) {
        if (isUsernameSelected) {
            cursor.setPosition(usernameText.getPosition().x + usernameText.getLocalBounds().width + 5, usernameText.getPosition().y);
            cursor.setString("|");
        }
        else if (isPasswordSelected) {
            cursor.setPosition(passwordText.getPosition().x + passwordText.getLocalBounds().width + 5, passwordText.getPosition().y);
            cursor.setString("|");
        }
        else if (isConfirmPasswordSelected && currentState == MenuState::REGISTER) {
            cursor.setPosition(confirmPasswordText.getPosition().x + confirmPasswordText.getLocalBounds().width + 5, confirmPasswordText.getPosition().y);
            cursor.setString("|");
        }
        else {
            cursor.setString("");
        }
    }
}

void Menu::addFieldHint(const std::string& text, float x, float y) {
    Text hint;
    hint.setFont(font);
    hint.setString(text);
    hint.setCharacterSize(15);
    hint.setFillColor(Color(200, 200, 200));  // Light gray color
    hint.setPosition(x, y);
    hints.push_back(hint);
}

bool Menu::validatePassword(const std::string& pass) {
    if (pass.length() < MIN_PASSWORD_LENGTH || pass.length() > MAX_PASSWORD_LENGTH) {
        return false;
    }

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (char c : pass) {
        if (isupper(c)) hasUpper = true;
        else if (islower(c)) hasLower = true;
        else if (isdigit(c)) hasDigit = true;
        else hasSpecial = true;
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}

void Menu::displayLoginMenu() {
    // Get current window dimensions
    float windowWidth = static_cast<float>(window->getSize().x);
    float windowHeight = static_cast<float>(window->getSize().y);

    // Draw background
    RectangleShape background(Vector2f(windowWidth, windowHeight));
    background.setFillColor(Color(30, 30, 40)); // Darker background
    window->draw(background);

    // Add header bar
    RectangleShape headerBar(Vector2f(windowWidth, 80));
    headerBar.setFillColor(Color(50, 50, 70));
    window->draw(headerBar);

    // Draw title with shadow effect
    Text shadowText;
    shadowText.setFont(font);
    shadowText.setString("Login");
    shadowText.setCharacterSize(50);
    shadowText.setFillColor(Color(20, 20, 30, 200));
    FloatRect shadowBounds = shadowText.getLocalBounds();
    shadowText.setPosition((windowWidth - shadowBounds.width) / 2 + 3, 47);
    window->draw(shadowText);

    // Main title with glow effect
    Text titleText;
    titleText.setFont(font);
    titleText.setString("Login");
    titleText.setCharacterSize(50);
    titleText.setFillColor(Color(220, 220, 50)); // Bright yellow
    titleText.setStyle(Text::Bold);
    FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition((windowWidth - titleBounds.width) / 2, 44);
    window->draw(titleText);

    // Draw decorative line beneath title
    RectangleShape divider(Vector2f(400, 2));
    divider.setFillColor(Color(220, 220, 50, 150)); // Semi-transparent yellow
    divider.setPosition((windowWidth - 400) / 2, 120);
    window->draw(divider);

    // Center everything in the window
    float centerX = windowWidth / 2;
    float startY = 150;
    float inputWidth = 300;
    float inputHeight = 40;
    float spacing = 30;

    // Username label
    Text usernameLabel;
    usernameLabel.setFont(font);
    usernameLabel.setString("Username:");
    usernameLabel.setCharacterSize(24);
    usernameLabel.setFillColor(Color(220, 220, 220));
    FloatRect usernameLabelBounds = usernameLabel.getLocalBounds();
    usernameLabel.setPosition(centerX - inputWidth / 2, startY);
    window->draw(usernameLabel);

    // Username input field
    RectangleShape usernameField(Vector2f(inputWidth, inputHeight));
    usernameField.setPosition(centerX - inputWidth / 2, startY + usernameLabelBounds.height + 10);
    usernameField.setFillColor(Color(60, 60, 80));
    usernameField.setOutlineThickness(2);
    usernameField.setOutlineColor(isUsernameSelected ? Color(220, 220, 50) : Color(100, 100, 120));
    window->draw(usernameField);

    // Store updated username box position for input handling
    usernameBox = usernameField;

    // Password label
    Text passwordLabel;
    passwordLabel.setFont(font);
    passwordLabel.setString("Password:");
    passwordLabel.setCharacterSize(24);
    passwordLabel.setFillColor(Color(220, 220, 220));
    FloatRect passwordLabelBounds = passwordLabel.getLocalBounds();
    passwordLabel.setPosition(centerX - inputWidth / 2, startY + usernameLabelBounds.height + inputHeight + spacing);
    window->draw(passwordLabel);

    // Password input field
    RectangleShape passwordField(Vector2f(inputWidth, inputHeight));
    passwordField.setPosition(centerX - inputWidth / 2, startY + usernameLabelBounds.height + inputHeight + spacing + passwordLabelBounds.height + 10);
    passwordField.setFillColor(Color(60, 60, 80));
    passwordField.setOutlineThickness(2);
    passwordField.setOutlineColor(isPasswordSelected ? Color(220, 220, 50) : Color(100, 100, 120));
    window->draw(passwordField);

    // Store updated password box position for input handling
    passwordBox = passwordField;

    // Draw username text
    usernameText.setString(username);
    usernameText.setFont(font);
    usernameText.setCharacterSize(22);
    usernameText.setFillColor(Color(220, 220, 220));
    usernameText.setPosition(centerX - inputWidth / 2 + 10, startY + usernameLabelBounds.height + 15);
    window->draw(usernameText);

    // Draw password text (masked or shown)
    passwordText.setString(showPassword ? password : std::string(password.length(), '*'));
    passwordText.setFont(font);
    passwordText.setCharacterSize(22);
    passwordText.setFillColor(Color(220, 220, 220));
    passwordText.setPosition(centerX - inputWidth / 2 + 10, startY + usernameLabelBounds.height + inputHeight + spacing + passwordLabelBounds.height + 15);
    window->draw(passwordText);

    // Login button
    float buttonY = startY + usernameLabelBounds.height + inputHeight + spacing + passwordLabelBounds.height + inputHeight + spacing + 20;
    RectangleShape loginButton(Vector2f(inputWidth, 50));
    loginButton.setPosition(centerX - inputWidth / 2, buttonY);
    loginButton.setFillColor(Color(0, 100, 0, 200));
    loginButton.setOutlineThickness(2);
    loginButton.setOutlineColor(Color(0, 150, 0));
    window->draw(loginButton);

    // Update login button position
    buttons[2] = loginButton;

    // Login button text
    Text loginButtonText;
    loginButtonText.setFont(font);
    loginButtonText.setString("Login");
    loginButtonText.setCharacterSize(26);
    loginButtonText.setFillColor(Color(220, 220, 220));
    FloatRect loginTextBounds = loginButtonText.getLocalBounds();
    loginButtonText.setPosition(
        centerX - loginTextBounds.width / 2,
        buttonY + (50 - loginTextBounds.height) / 2 - loginTextBounds.top
    );
    window->draw(loginButtonText);

    // Register link
    float registerY = buttonY + 70;
    Text registerText;
    registerText.setFont(font);
    registerText.setString("New User? Click Here to Register");
    registerText.setCharacterSize(20);
    registerText.setFillColor(Color(220, 220, 50));
    FloatRect registerBounds = registerText.getLocalBounds();
    registerText.setPosition(centerX - registerBounds.width / 2, registerY);
    window->draw(registerText);

    // Update register button position
    RectangleShape registerButton(Vector2f(registerBounds.width + 20, registerBounds.height + 10));
    registerButton.setPosition(centerX - (registerBounds.width + 20) / 2, registerY - 5);
    registerButton.setFillColor(Color::Transparent);
    buttons[3] = registerButton;

    // Draw error message if any
    if (errorTimer > 0) {
        Text errorText;
        errorText.setFont(font);
        errorText.setString(errorMessage);
        errorText.setCharacterSize(20);
        errorText.setFillColor(Color::Red);
        FloatRect errorBounds = errorText.getLocalBounds();
        errorText.setPosition(centerX - errorBounds.width / 2, registerY + 40);
        window->draw(errorText);
    }

    // Draw cursor for text input
    updateCursor();
    if (showCursor) {
        window->draw(cursor);
    }
}

void Menu::displayRegisterMenu() {
    // Get current window dimensions
    float windowWidth = static_cast<float>(window->getSize().x);
    float windowHeight = static_cast<float>(window->getSize().y);

    // Draw background
    RectangleShape background(Vector2f(windowWidth, windowHeight));
    background.setFillColor(Color(30, 30, 40)); // Darker background
    window->draw(background);

    // Add header bar
    RectangleShape headerBar(Vector2f(windowWidth, 80));
    headerBar.setFillColor(Color(50, 50, 70));
    window->draw(headerBar);

    // Draw title with shadow effect
    Text shadowText;
    shadowText.setFont(font);
    shadowText.setString("Register");
    shadowText.setCharacterSize(50);
    shadowText.setFillColor(Color(20, 20, 30, 200));
    FloatRect shadowBounds = shadowText.getLocalBounds();
    shadowText.setPosition((windowWidth - shadowBounds.width) / 2 + 3, 47);
    window->draw(shadowText);

    // Main title with glow effect
    Text titleText;
    titleText.setFont(font);
    titleText.setString("Register");
    titleText.setCharacterSize(50);
    titleText.setFillColor(Color(220, 220, 50)); // Bright yellow
    titleText.setStyle(Text::Bold);
    FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition((windowWidth - titleBounds.width) / 2, 44);
    window->draw(titleText);

    // Draw decorative line beneath title
    RectangleShape divider(Vector2f(400, 2));
    divider.setFillColor(Color(220, 220, 50, 150)); // Semi-transparent yellow
    divider.setPosition((windowWidth - 400) / 2, 120);
    window->draw(divider);

    // Back button
    RectangleShape backButton(Vector2f(150, 30));
    backButton.setPosition(20, 20);
    backButton.setFillColor(Color(60, 60, 80, 200));
    backButton.setOutlineThickness(1);
    backButton.setOutlineColor(Color(150, 150, 150));
    window->draw(backButton);

    Text backText;
    backText.setFont(font);
    backText.setString("Back to Login");
    backText.setCharacterSize(16);
    backText.setFillColor(Color(220, 220, 220));
    FloatRect backBounds = backText.getLocalBounds();
    backText.setPosition(
        20 + (150 - backBounds.width) / 2,
        20 + (30 - backBounds.height) / 2 - backBounds.top
    );
    window->draw(backText);

    // Update back button position
    buttons[0] = backButton;

    // Center form elements
    float centerX = windowWidth / 2;
    float startY = 140;
    float inputWidth = 300;
    float inputHeight = 40;
    float spacing = 20;
    float labelSpacing = 8;

    // Username section
    Text usernameLabel;
    usernameLabel.setFont(font);
    usernameLabel.setString("Username:");
    usernameLabel.setCharacterSize(22);
    usernameLabel.setFillColor(Color(220, 220, 220));
    usernameLabel.setPosition(centerX - inputWidth / 2, startY);
    window->draw(usernameLabel);

    RectangleShape usernameField(Vector2f(inputWidth, inputHeight));
    usernameField.setPosition(centerX - inputWidth / 2, startY + usernameLabel.getLocalBounds().height + labelSpacing);
    usernameField.setFillColor(Color(60, 60, 80));
    usernameField.setOutlineThickness(2);
    usernameField.setOutlineColor(isUsernameSelected ? Color(220, 220, 50) : Color(100, 100, 120));
    window->draw(usernameField);

    // Update username box position
    usernameBox = usernameField;

    // Draw username text
    usernameText.setString(username);
    usernameText.setFont(font);
    usernameText.setCharacterSize(22);
    usernameText.setFillColor(Color(220, 220, 220));
    usernameText.setPosition(centerX - inputWidth / 2 + 10, startY + usernameLabel.getLocalBounds().height + labelSpacing + 8);
    window->draw(usernameText);

    // Username hint
    Text usernameHint;
    usernameHint.setFont(font);
    usernameHint.setString("Enter your username (3-20 characters)");
    usernameHint.setCharacterSize(14);
    usernameHint.setFillColor(Color(180, 180, 180, 200));
    usernameHint.setPosition(centerX - inputWidth / 2, startY + usernameLabel.getLocalBounds().height + labelSpacing + inputHeight + 2);
    window->draw(usernameHint);

    // Password section
    float passwordY = startY + usernameLabel.getLocalBounds().height + labelSpacing + inputHeight + 30;

    Text passwordLabel;
    passwordLabel.setFont(font);
    passwordLabel.setString("Password:");
    passwordLabel.setCharacterSize(22);
    passwordLabel.setFillColor(Color(220, 220, 220));
    passwordLabel.setPosition(centerX - inputWidth / 2, passwordY);
    window->draw(passwordLabel);

    RectangleShape passwordField(Vector2f(inputWidth, inputHeight));
    passwordField.setPosition(centerX - inputWidth / 2, passwordY + passwordLabel.getLocalBounds().height + labelSpacing);
    passwordField.setFillColor(Color(60, 60, 80));
    passwordField.setOutlineThickness(2);
    passwordField.setOutlineColor(isPasswordSelected ? Color(220, 220, 50) : Color(100, 100, 120));
    window->draw(passwordField);

    // Update password box position
    passwordBox = passwordField;

    // Draw password text
    passwordText.setString(showPassword ? password : std::string(password.length(), '*'));
    passwordText.setFont(font);
    passwordText.setCharacterSize(22);
    passwordText.setFillColor(Color(220, 220, 220));
    passwordText.setPosition(centerX - inputWidth / 2 + 10, passwordY + passwordLabel.getLocalBounds().height + labelSpacing + 8);
    window->draw(passwordText);

    // Show password button
    RectangleShape showPasswordButton(Vector2f(150, 30));
    showPasswordButton.setPosition(centerX + inputWidth / 2 + 20, passwordY + passwordLabel.getLocalBounds().height + labelSpacing + 5);
    showPasswordButton.setFillColor(Color(60, 60, 100, 200));
    showPasswordButton.setOutlineThickness(1);
    showPasswordButton.setOutlineColor(Color(100, 100, 150));
    window->draw(showPasswordButton);

    Text showPasswordText;
    showPasswordText.setFont(font);
    showPasswordText.setString(showPassword ? "Hide Password" : "Show Password");
    showPasswordText.setCharacterSize(16);
    showPasswordText.setFillColor(Color(220, 220, 220));
    FloatRect showPassBounds = showPasswordText.getLocalBounds();
    showPasswordText.setPosition(
        centerX + inputWidth / 2 + 20 + (150 - showPassBounds.width) / 2,
        passwordY + passwordLabel.getLocalBounds().height + labelSpacing + 5 + (30 - showPassBounds.height) / 2 - showPassBounds.top
    );
    window->draw(showPasswordText);

    // Update show password button position
    buttons[4] = showPasswordButton;

    // Password requirements
    float reqY = passwordY + passwordLabel.getLocalBounds().height + labelSpacing + inputHeight + 5;

    Text reqTitle;
    reqTitle.setFont(font);
    reqTitle.setString("Password Requirements:");
    reqTitle.setCharacterSize(14);
    reqTitle.setFillColor(Color(180, 180, 180, 200));
    reqTitle.setPosition(centerX - inputWidth / 2, reqY);
    window->draw(reqTitle);

    std::string requirements[] = {
        "• At least 6 characters long",
        "• At least one uppercase letter (A-Z)",
        "• At least one lowercase letter (a-z)",
        "• At least one number (0-9)",
        "• At least one special character (!@#$%^&* etc.)"
    };

    for (int i = 0; i < 5; i++) {
        Text req;
        req.setFont(font);
        req.setString(requirements[i]);
        req.setCharacterSize(14);
        req.setFillColor(Color(180, 180, 180, 200));
        req.setPosition(centerX - inputWidth / 2 + 10, reqY + 20 + i * 18);
        window->draw(req);
    }

    // Confirm Password section
    float confirmY = reqY + 120;

    Text confirmLabel;
    confirmLabel.setFont(font);
    confirmLabel.setString("Confirm Password:");
    confirmLabel.setCharacterSize(22);
    confirmLabel.setFillColor(Color(220, 220, 220));
    confirmLabel.setPosition(centerX - inputWidth / 2, confirmY);
    window->draw(confirmLabel);

    RectangleShape confirmField(Vector2f(inputWidth, inputHeight));
    confirmField.setPosition(centerX - inputWidth / 2, confirmY + confirmLabel.getLocalBounds().height + labelSpacing);
    confirmField.setFillColor(Color(60, 60, 80));
    confirmField.setOutlineThickness(2);
    confirmField.setOutlineColor(isConfirmPasswordSelected ? Color(220, 220, 50) : Color(100, 100, 120));
    window->draw(confirmField);

    // Update confirm password box position
    confirmPasswordBox = confirmField;

    // Draw confirm password text
    confirmPasswordText.setString(showPassword ? confirmPassword : std::string(confirmPassword.length(), '*'));
    confirmPasswordText.setFont(font);
    confirmPasswordText.setCharacterSize(22);
    confirmPasswordText.setFillColor(Color(220, 220, 220));
    confirmPasswordText.setPosition(centerX - inputWidth / 2 + 10, confirmY + confirmLabel.getLocalBounds().height + labelSpacing + 8);
    window->draw(confirmPasswordText);

    // Confirm password hint
    Text confirmHint;
    confirmHint.setFont(font);
    confirmHint.setString("Re-enter your password to confirm");
    confirmHint.setCharacterSize(14);
    confirmHint.setFillColor(Color(180, 180, 180, 200));
    confirmHint.setPosition(centerX - inputWidth / 2, confirmY + confirmLabel.getLocalBounds().height + labelSpacing + inputHeight + 2);
    window->draw(confirmHint);

    // Register button
    float registerButtonY = confirmY + confirmLabel.getLocalBounds().height + labelSpacing + inputHeight + 35;
    float registerButtonWidth = 250;

    RectangleShape registerButton(Vector2f(registerButtonWidth, 50));
    registerButton.setPosition(centerX - registerButtonWidth / 2, registerButtonY);
    registerButton.setFillColor(Color(150, 30, 30, 200));
    registerButton.setOutlineThickness(2);
    registerButton.setOutlineColor(Color(200, 50, 50));
    window->draw(registerButton);

    // Update register button position
    buttons[5] = registerButton;

    Text registerButtonText;
    registerButtonText.setFont(font);
    registerButtonText.setString("REGISTER");
    registerButtonText.setCharacterSize(26);
    registerButtonText.setStyle(Text::Bold);
    registerButtonText.setFillColor(Color(220, 220, 220));
    FloatRect registerTextBounds = registerButtonText.getLocalBounds();
    registerButtonText.setPosition(
        centerX - registerTextBounds.width / 2,
        registerButtonY + (50 - registerTextBounds.height) / 2 - registerTextBounds.top
    );
    window->draw(registerButtonText);

    // Draw error message if any
    if (errorTimer > 0) {
        Text errorText;
        errorText.setFont(font);
        errorText.setString(errorMessage);
        errorText.setCharacterSize(18);
        errorText.setFillColor(Color(255, 100, 100));
        FloatRect errorBounds = errorText.getLocalBounds();
        errorText.setPosition(centerX - errorBounds.width / 2, registerButtonY + 60);
        window->draw(errorText);
    }

    // Draw cursor for text input
    updateCursor();
    if (showCursor) {
        window->draw(cursor);
    }
}

void Menu::displayLevelSelect() {
    // Draw title
    Text titleText;
    titleText.setFont(font);
    titleText.setString("Level Select");
    titleText.setCharacterSize(50);
    titleText.setFillColor(Color::White);
    titleText.setPosition(window->getSize().x / 2 - titleText.getLocalBounds().width / 2, 50);
    window->draw(titleText);

    // Draw back button
    Text backText;
    backText.setFont(font);
    backText.setString("Back to Main Menu");
    backText.setCharacterSize(20);
    backText.setFillColor(Color::White);
    backText.setPosition(20, 20);
    window->draw(backText);

    // Draw level buttons
    for (size_t i = 0; i < buttons.size(); i++) {
        window->draw(buttons[i]);
    }

    // Draw level text
    for (const auto& item : menuItems) {
        window->draw(item);
    }
}

void Menu::displayLeaderboard() {
    // Clear the window first
    window->clear(Color(50, 50, 50));  // Dark gray background

    // Get current window dimensions
    float windowWidth = static_cast<float>(window->getSize().x);
    float windowHeight = static_cast<float>(window->getSize().y);

    // Draw a nice background with gradient effect
    RectangleShape background(Vector2f(windowWidth, windowHeight));
    background.setFillColor(Color(30, 30, 40)); // Darker background
    window->draw(background);

    // Add a decorative header bar
    RectangleShape headerBar(Vector2f(windowWidth, 80));
    headerBar.setFillColor(Color(50, 50, 70));
    window->draw(headerBar);

    // Draw title with shadow effect for depth
    Text shadowText;
    shadowText.setFont(font);
    shadowText.setString("Leaderboard");
    shadowText.setCharacterSize(50);
    shadowText.setFillColor(Color(20, 20, 30, 200));
    FloatRect shadowBounds = shadowText.getLocalBounds();
    shadowText.setPosition((windowWidth - shadowBounds.width) / 2 + 3, 47);
    window->draw(shadowText);

    // Main title with glow effect
    Text titleText;
    titleText.setFont(font);
    titleText.setString("Leaderboard");
    titleText.setCharacterSize(50);
    titleText.setFillColor(Color(220, 220, 50)); // Bright yellow
    titleText.setStyle(Text::Bold);
    FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition((windowWidth - titleBounds.width) / 2, 44);
    window->draw(titleText);

    // Draw decorative line beneath title
    RectangleShape divider(Vector2f(400, 2));
    divider.setFillColor(Color(220, 220, 50, 150)); // Semi-transparent yellow
    divider.setPosition((windowWidth - 400) / 2, 120);
    window->draw(divider);

    // Back button with styling
    RectangleShape backButton(Vector2f(180, 40));
    backButton.setPosition(20, 20);
    backButton.setFillColor(Color(60, 60, 80, 200));
    backButton.setOutlineThickness(1);
    backButton.setOutlineColor(Color(150, 150, 150));
    window->draw(backButton);

    Text backText;
    backText.setFont(font);
    backText.setString("Back to Main Menu");
    backText.setCharacterSize(16);
    backText.setFillColor(Color(220, 220, 220));
    FloatRect backBounds = backText.getLocalBounds();
    backText.setPosition(
        20 + (180 - backBounds.width) / 2,
        20 + (40 - backBounds.height) / 2 - backBounds.top
    );
    window->draw(backText);

    // Store the back button for event handling
    if (!buttons.empty()) {
        buttons[0] = backButton;  // Update existing button instead of clearing
    }
    else {
        buttons.push_back(backButton);
    }

    // Create a leaderboard panel
    float panelWidth = 600;
    float panelHeight = 450;
    RectangleShape leaderboardPanel(Vector2f(panelWidth, panelHeight));
    leaderboardPanel.setPosition((windowWidth - panelWidth) / 2, 140);
    leaderboardPanel.setFillColor(Color(40, 40, 60, 180));
    leaderboardPanel.setOutlineThickness(2);
    leaderboardPanel.setOutlineColor(Color(80, 80, 100));
    window->draw(leaderboardPanel);

    // Fetch scores from leaderboard
    int count;
    PlayerScore* scores = leaderboard.getTopScores(count);
    std::cout << "Displaying leaderboard with " << count << " scores" << std::endl;

    // Add all scores to the debug output
    for (int i = 0; i < count; i++) {
        std::cout << "  Leaderboard entry " << i + 1 << ": " << scores[i].username << " - " << scores[i].score << std::endl;
    }

    // Display column headers with style
    float headerY = 150;
    float rankX = (windowWidth - panelWidth) / 2 + 40;
    float nameX = (windowWidth - panelWidth) / 2 + 140;
    float scoreX = (windowWidth - panelWidth) / 2 + 420;

    // Header background
    RectangleShape headerBg(Vector2f(panelWidth - 20, 40));
    headerBg.setPosition((windowWidth - panelWidth) / 2 + 10, headerY);
    headerBg.setFillColor(Color(60, 60, 80));
    headerBg.setOutlineThickness(1);
    headerBg.setOutlineColor(Color(100, 100, 120));
    window->draw(headerBg);

    // Add headers to menuItems for consistency
    menuItems.clear();  // Clear existing items

    // Add back button text to menuItems
    menuItems.push_back(backText);

    // Add title to menuItems
    menuItems.push_back(titleText);

    // Header titles
    Text rankHeader;
    rankHeader.setFont(font);
    rankHeader.setString("Rank");
    rankHeader.setCharacterSize(24);
    rankHeader.setFillColor(Color(220, 220, 50));
    rankHeader.setStyle(Text::Bold);
    rankHeader.setPosition(rankX, headerY + 5);
    window->draw(rankHeader);
    menuItems.push_back(rankHeader);

    Text nameHeader;
    nameHeader.setFont(font);
    nameHeader.setString("Player");
    nameHeader.setCharacterSize(24);
    nameHeader.setFillColor(Color(220, 220, 50));
    nameHeader.setStyle(Text::Bold);
    nameHeader.setPosition(nameX, headerY + 5);
    window->draw(nameHeader);
    menuItems.push_back(nameHeader);

    Text scoreHeader;
    scoreHeader.setFont(font);
    scoreHeader.setString("Score");
    scoreHeader.setCharacterSize(24);
    scoreHeader.setFillColor(Color(220, 220, 50));
    scoreHeader.setStyle(Text::Bold);
    scoreHeader.setPosition(scoreX, headerY + 5);
    window->draw(scoreHeader);
    menuItems.push_back(scoreHeader);

    // Display each score with alternating row colors
    float rowHeight = 38;
    float startY = headerY + 45;

    if (count > 0) {
        for (int i = 0; i < count; i++) {
            // Row background with alternating colors
            RectangleShape rowBg(Vector2f(panelWidth - 20, rowHeight));
            rowBg.setPosition((windowWidth - panelWidth) / 2 + 10, startY + i * rowHeight);
            rowBg.setFillColor(i % 2 == 0 ? Color(50, 50, 70, 180) : Color(60, 60, 80, 180));
            rowBg.setOutlineThickness(1);
            rowBg.setOutlineColor(Color(70, 70, 90, 100));
            window->draw(rowBg);

            // Rank with medal icons for top 3
            std::string rankText = std::to_string(i + 1) + ".";
            Text rank;
            rank.setFont(font);
            rank.setString(rankText);
            rank.setCharacterSize(22);

            // Special styling for top 3 ranks
            if (i == 0) {
                rank.setFillColor(Color(255, 215, 0)); // Gold
                rank.setStyle(Text::Bold);
            }
            else if (i == 1) {
                rank.setFillColor(Color(192, 192, 192)); // Silver
                rank.setStyle(Text::Bold);
            }
            else if (i == 2) {
                rank.setFillColor(Color(205, 127, 50)); // Bronze
                rank.setStyle(Text::Bold);
            }
            else {
                rank.setFillColor(Color(200, 200, 200));
            }

            rank.setPosition(rankX, startY + i * rowHeight + 6);
            window->draw(rank);
            menuItems.push_back(rank);

            // Player name
            Text name;
            name.setFont(font);
            name.setString(scores[i].username);
            name.setCharacterSize(22);
            name.setFillColor(Color(220, 220, 220));
            name.setPosition(nameX, startY + i * rowHeight + 6);
            window->draw(name);
            menuItems.push_back(name);

            // Score with glow effect for top scores
            Text score;
            score.setFont(font);
            score.setString(std::to_string(scores[i].score));
            score.setCharacterSize(22);

            // Special styling for scores
            if (i < 3) {
                score.setFillColor(Color(220, 220, 50));
                score.setStyle(Text::Bold);
            }
            else {
                score.setFillColor(Color(220, 220, 220));
            }

            score.setPosition(scoreX, startY + i * rowHeight + 6);
            window->draw(score);
            menuItems.push_back(score);
        }
    }
    else {
        // If no scores yet
        RectangleShape emptyBg(Vector2f(panelWidth - 40, 100));
        emptyBg.setPosition((windowWidth - panelWidth) / 2 + 20, startY + 50);
        emptyBg.setFillColor(Color(50, 50, 70, 180));
        emptyBg.setOutlineThickness(1);
        emptyBg.setOutlineColor(Color(70, 70, 90));
        window->draw(emptyBg);

        Text noScores;
        noScores.setFont(font);
        noScores.setString("No scores yet! Play a game to set a record.");
        noScores.setCharacterSize(24);
        noScores.setFillColor(Color(220, 220, 220));
        FloatRect noScoresBounds = noScores.getLocalBounds();
        noScores.setPosition(
            (windowWidth - noScoresBounds.width) / 2,
            startY + 85
        );
        window->draw(noScores);
        menuItems.push_back(noScores);
    }

    // Footer with info text
    Text footerText;
    footerText.setFont(font);
    footerText.setString("Top 10 players are displayed. Can you make it to the leaderboard?");
    footerText.setCharacterSize(16);
    footerText.setFillColor(Color(150, 150, 150));
    FloatRect footerBounds = footerText.getLocalBounds();
    footerText.setPosition(
        (windowWidth - footerBounds.width) / 2,
        startY + 10 * rowHeight + 20
    );
    window->draw(footerText);
    menuItems.push_back(footerText);

    // Clean up
    delete[] scores;
}

void Menu::displayProfile() {
    // Draw background
    float windowWidth = static_cast<float>(window->getSize().x);
    float windowHeight = static_cast<float>(window->getSize().y);

    // Draw a background
    RectangleShape background(Vector2f(windowWidth, windowHeight));
    background.setFillColor(Color(30, 30, 40));
    window->draw(background);

    // Add header bar
    RectangleShape headerBar(Vector2f(windowWidth, 80));
    headerBar.setFillColor(Color(50, 50, 70));
    window->draw(headerBar);

    // Draw all buttons first (so they're behind text)
    for (const auto& button : buttons) {
        window->draw(button);
    }

    // Draw all text elements
    for (const auto& item : menuItems) {
        window->draw(item);
    }
}
void Menu::displayOptions() {
    // Get window dimensions
    float windowWidth = static_cast<float>(window->getSize().x);
    float windowHeight = static_cast<float>(window->getSize().y);

    // Draw background
    RectangleShape background(Vector2f(windowWidth, windowHeight));
    background.setFillColor(Color(30, 30, 40));
    window->draw(background);

    // Add header bar
    RectangleShape headerBar(Vector2f(windowWidth, 80));
    headerBar.setFillColor(Color(50, 50, 70));
    window->draw(headerBar);

    // Draw title
    Text titleText;
    titleText.setFont(font);
    titleText.setString("Options");
    titleText.setCharacterSize(50);
    titleText.setFillColor(Color(220, 220, 50));
    titleText.setStyle(Text::Bold);
    FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition((windowWidth - titleBounds.width) / 2, 44);
    window->draw(titleText);

    // Draw back button
    RectangleShape backButton(Vector2f(180, 40));
    backButton.setPosition(20, 20);
    backButton.setFillColor(Color(60, 60, 80, 200));
    backButton.setOutlineThickness(1);
    backButton.setOutlineColor(Color(150, 150, 150));
    window->draw(backButton);

    Text backText;
    backText.setFont(font);
    backText.setString("Back to Main Menu");
    backText.setCharacterSize(16);
    backText.setFillColor(Color(220, 220, 220));
    FloatRect backBounds = backText.getLocalBounds();
    backText.setPosition(
        20 + (180 - backBounds.width) / 2,
        20 + (40 - backBounds.height) / 2 - backBounds.top
    );
    window->draw(backText);

    // Display "Coming Soon" message
    Text comingSoonText;
    comingSoonText.setFont(font);
    comingSoonText.setString("Options feature coming soon!");
    comingSoonText.setCharacterSize(30);
    comingSoonText.setFillColor(Color(220, 220, 220));
    FloatRect textBounds = comingSoonText.getLocalBounds();
    comingSoonText.setPosition(
        (windowWidth - textBounds.width) / 2,
        (windowHeight - textBounds.height) / 2
    );
    window->draw(comingSoonText);
}

// Add these implementations at the end of the file

// Refresh the list of saves for the current user
void Menu::refreshSavesList() {
    if (game != nullptr) {
        savesList = game->getSaves(currentUser);
    }
}

// Save the current game
bool Menu::saveGame() {
    if (game != nullptr) {
        return game->saveGame(currentUser);
    }
    return false;
}

// Load a saved game
bool Menu::loadGame(const std::string& saveID) {
    if (game != nullptr) {
        return game->loadGame(saveID, currentUser);
    }
    return false;
}

// Delete a saved game
bool Menu::deleteSave(const std::string& saveID) {
    if (game != nullptr && game->getGameSave() != nullptr) {
        return game->getGameSave()->deleteSave(saveID);
    }
    return false;
}

// Initialize save game menu
void Menu::initializeSaveGame() {
    menuItems.clear();
    buttons.clear();

    // Create title
    Text title;
    title.setFont(font);
    title.setString("Save Game");
    title.setCharacterSize(50);
    title.setFillColor(Color::White);
    title.setPosition(window->getSize().x / 2 - 100, 50);
    menuItems.push_back(title);

    // Create back button
    Text backText;
    backText.setFont(font);
    backText.setString("Back to Game");
    backText.setCharacterSize(20);
    backText.setFillColor(Color::White);
    backText.setPosition(20, 20);
    menuItems.push_back(backText);

    RectangleShape backButton;
    backButton.setSize(Vector2f(150, 40));
    backButton.setPosition(20, 20);
    backButton.setFillColor(Color::Transparent);
    backButton.setOutlineThickness(2);
    backButton.setOutlineColor(Color::White);
    buttons.push_back(backButton);

    // Create save button
    Text saveText;
    saveText.setFont(font);
    saveText.setString("Save Current Game");
    saveText.setCharacterSize(30);
    saveText.setFillColor(Color::White);
    saveText.setPosition(300, 200);
    menuItems.push_back(saveText);

    RectangleShape saveButton;
    saveButton.setSize(Vector2f(250, 50));
    saveButton.setPosition(290, 200);
    saveButton.setFillColor(Color(0, 100, 0));
    saveButton.setOutlineThickness(2);
    saveButton.setOutlineColor(Color::White);
    buttons.push_back(saveButton);

    // Player info
    Text playerText;
    playerText.setFont(font);
    playerText.setString("Player: " + currentUser);
    playerText.setCharacterSize(25);
    playerText.setFillColor(Color::Yellow);
    playerText.setPosition(300, 150);
    menuItems.push_back(playerText);
}

// Initialize load game menu
void Menu::initializeLoadGame() {
    menuItems.clear();
    buttons.clear();

    // Get saves list for current user
    refreshSavesList();
    selectedSaveIndex = 0;

    // Create title
    Text title;
    title.setFont(font);
    title.setString("Load Game");
    title.setCharacterSize(50);
    title.setFillColor(Color::White);
    title.setPosition(window->getSize().x / 2 - 100, 50);
    menuItems.push_back(title);

    // Create back button
    Text backText;
    backText.setFont(font);
    backText.setString("Back to Main Menu");
    backText.setCharacterSize(20);
    backText.setFillColor(Color::White);
    backText.setPosition(20, 20);
    menuItems.push_back(backText);

    RectangleShape backButton;
    backButton.setSize(Vector2f(150, 40));
    backButton.setPosition(20, 20);
    backButton.setFillColor(Color::Transparent);
    backButton.setOutlineThickness(2);
    backButton.setOutlineColor(Color::White);
    buttons.push_back(backButton);

    // Player info
    Text playerText;
    playerText.setFont(font);
    playerText.setString("Player: " + currentUser);
    playerText.setCharacterSize(25);
    playerText.setFillColor(Color::Yellow);
    playerText.setPosition(300, 120);
    menuItems.push_back(playerText);

    // Create buttons for each save
    int yPos = 170;
    for (int i = 0; i < savesList.getSize() && i < 5; i++) {
        // Get save info
        SaveGameState saveInfo;
        if (game != nullptr && game->getGameSave() != nullptr) {
            saveInfo = game->getGameSave()->getSaveInfo(savesList.get(i));
        }

        // Create button
        RectangleShape saveButton;
        saveButton.setSize(Vector2f(500, 60));
        saveButton.setPosition(150, yPos);
        saveButton.setFillColor(Color(50, 50, 80));
        saveButton.setOutlineThickness(2);
        saveButton.setOutlineColor(Color::White);
        buttons.push_back(saveButton);

        // Save ID and date
        std::string dateStr = "Unknown";
        if (saveInfo.timestamp != 0) {
            time_t timestamp = saveInfo.timestamp;
            char buffer[80];
            struct tm timeinfo;
            localtime_s(&timeinfo, &timestamp);
            strftime(buffer, sizeof(buffer), "%m/%d/%Y %H:%M", &timeinfo);
            dateStr = buffer;
        }

        Text saveInfoText;
        saveInfoText.setFont(font);
        saveInfoText.setString(savesList.get(i) + " - " + dateStr);
        saveInfoText.setCharacterSize(18);
        saveInfoText.setFillColor(Color::White);
        saveInfoText.setPosition(160, yPos + 10);
        menuItems.push_back(saveInfoText);

        // Score
        Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: " + std::to_string(saveInfo.score));
        scoreText.setCharacterSize(18);
        scoreText.setFillColor(Color::Yellow);
        scoreText.setPosition(500, yPos + 10);
        menuItems.push_back(scoreText);

        // Load button
        RectangleShape loadButton;
        loadButton.setSize(Vector2f(80, 40));
        loadButton.setPosition(550, yPos + 10);
        loadButton.setFillColor(Color(0, 100, 0));
        loadButton.setOutlineThickness(1);
        loadButton.setOutlineColor(Color::White);
        buttons.push_back(loadButton);

        Text loadText;
        loadText.setFont(font);
        loadText.setString("Load");
        loadText.setCharacterSize(18);
        loadText.setFillColor(Color::White);
        loadText.setPosition(565, yPos + 20);
        menuItems.push_back(loadText);

        // Delete button
        RectangleShape deleteButton;
        deleteButton.setSize(Vector2f(80, 40));
        deleteButton.setPosition(640, yPos + 10);
        deleteButton.setFillColor(Color(150, 0, 0));
        deleteButton.setOutlineThickness(1);
        deleteButton.setOutlineColor(Color::White);
        buttons.push_back(deleteButton);

        Text deleteText;
        deleteText.setFont(font);
        deleteText.setString("Delete");
        deleteText.setCharacterSize(18);
        deleteText.setFillColor(Color::White);
        deleteText.setPosition(650, yPos + 20);
        menuItems.push_back(deleteText);

        yPos += 70;
    }

    // If no saves found
    if (savesList.getSize() == 0) {
        Text noSavesText;
        noSavesText.setFont(font);
        noSavesText.setString("No saved games found for this player");
        noSavesText.setCharacterSize(25);
        noSavesText.setFillColor(Color::White);
        noSavesText.setPosition(window->getSize().x / 2 - 200, 250);
        menuItems.push_back(noSavesText);
    }
}

// Handle save game menu input
void Menu::handleSaveGameInput(Event& event) {
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Escape) {
            setState(MenuState::MAIN_MENU);
        }
    }
    else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        Vector2f mousePos = window->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));

        // Check if back button clicked
        if (buttons[0].getGlobalBounds().contains(mousePos)) {
            setState(MenuState::MAIN_MENU);
        }
        // Check if save button clicked
        else if (buttons[1].getGlobalBounds().contains(mousePos)) {
            showConfirmation("Are you sure you want to save the current game?", true);
        }
    }
}

// Handle load game menu input
void Menu::handleLoadGameInput(Event& event) {
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Escape) {
            setState(MenuState::MAIN_MENU);
        }
        else if (event.key.code == Keyboard::Up) {
            if (selectedSaveIndex > 0) {
                selectedSaveIndex--;
            }
        }
        else if (event.key.code == Keyboard::Down) {
            if (selectedSaveIndex < savesList.getSize() - 1) {
                selectedSaveIndex++;
            }
        }
        else if (event.key.code == Keyboard::Return) {
            if (savesList.getSize() > 0) {
                std::string saveID = savesList.get(selectedSaveIndex);
                showConfirmation("Are you sure you want to load this saved game?", false);
            }
        }
    }
    else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        Vector2f mousePos = window->mapPixelToCoords(Vector2i(event.mouseButton.x, event.mouseButton.y));

        // Check if back button clicked
        if (buttons[0].getGlobalBounds().contains(mousePos)) {
            setState(MenuState::MAIN_MENU);
        }

        // Check if load or delete buttons clicked for each save
        for (int i = 0; i < savesList.getSize() && i < 5; i++) {
            // First save starts at index 1 (back button is index 0)
            // Each save has a main button, load button, and delete button
            int saveButtonIndex = i * 3 + 1;
            int loadButtonIndex = saveButtonIndex + 1;
            int deleteButtonIndex = loadButtonIndex + 1;

            // Check if save row clicked (select it)
            if (buttons[saveButtonIndex].getGlobalBounds().contains(mousePos)) {
                selectedSaveIndex = i;
            }
            // Check if load button clicked
            else if (buttons[loadButtonIndex].getGlobalBounds().contains(mousePos)) {
                selectedSaveIndex = i;
                showConfirmation("Are you sure you want to load this saved game?", false);
            }
            // Check if delete button clicked
            else if (buttons[deleteButtonIndex].getGlobalBounds().contains(mousePos)) {
                selectedSaveIndex = i;
                showConfirmation("Are you sure you want to delete this saved game?", false);
            }
        }
    }
}

// Display save game menu
void Menu::displaySaveGame() {
    // Get window dimensions
    float windowWidth = static_cast<float>(window->getSize().x);
    float windowHeight = static_cast<float>(window->getSize().y);

    // Draw background
    RectangleShape background(Vector2f(windowWidth, windowHeight));
    background.setFillColor(Color(30, 30, 40));
    window->draw(background);

    // Draw header bar
    RectangleShape headerBar(Vector2f(windowWidth, 80));
    headerBar.setFillColor(Color(50, 50, 70));
    window->draw(headerBar);

    // Draw back button
    RectangleShape backButton = buttons[0];
    backButton.setFillColor(Color(60, 60, 80, 200));
    window->draw(backButton);

    // Draw save button
    RectangleShape saveButton = buttons[1];
    window->draw(saveButton);

    // Draw all text elements
    for (const auto& item : menuItems) {
        window->draw(item);
    }

    // Add additional information
    Text infoText;
    infoText.setFont(font);
    infoText.setString("Your game will be saved with the current state and score.\nYou can load it later from the main menu.");
    infoText.setCharacterSize(20);
    infoText.setFillColor(Color(200, 200, 200));
    infoText.setPosition(250, 300);
    window->draw(infoText);
}

// Display load game menu
void Menu::displayLoadGame() {
    // Get window dimensions
    float windowWidth = static_cast<float>(window->getSize().x);
    float windowHeight = static_cast<float>(window->getSize().y);

    // Draw background
    RectangleShape background(Vector2f(windowWidth, windowHeight));
    background.setFillColor(Color(30, 30, 40));
    window->draw(background);

    // Draw header bar
    RectangleShape headerBar(Vector2f(windowWidth, 80));
    headerBar.setFillColor(Color(50, 50, 70));
    window->draw(headerBar);

    // Draw back button with proper styling
    if (!buttons.empty()) {
        RectangleShape backButton = buttons[0];
        backButton.setFillColor(Color(60, 60, 80, 200));
        window->draw(backButton);
    }

    // Highlight selected save
    if (savesList.getSize() > 0 && selectedSaveIndex >= 0 && selectedSaveIndex < savesList.getSize()) {
        int saveButtonIndex = selectedSaveIndex * 3 + 1;
        if (saveButtonIndex < buttons.size()) {
            RectangleShape selectedButton = buttons[saveButtonIndex];
            selectedButton.setFillColor(Color(70, 70, 120));
            selectedButton.setOutlineThickness(2);
            selectedButton.setOutlineColor(Color(200, 200, 50));
            window->draw(selectedButton);
        }
    }

    // Draw all buttons (except the highlighted one we just drew)
    for (size_t i = 0; i < buttons.size(); i++) {
        if (savesList.getSize() > 0 &&
            i == (selectedSaveIndex * 3 + 1) &&
            selectedSaveIndex >= 0 &&
            selectedSaveIndex < savesList.getSize()) {
            continue; // Skip the highlighted button
        }
        window->draw(buttons[i]);
    }

    // Draw all text elements
    for (const auto& item : menuItems) {
        window->draw(item);
    }

    // Draw instructions
    if (savesList.getSize() > 0) {
        Text instructionsText;
        instructionsText.setFont(font);
        instructionsText.setString("Use Up/Down keys to navigate, Enter to load selected save");
        instructionsText.setCharacterSize(16);
        instructionsText.setFillColor(Color(200, 200, 200));
        instructionsText.setPosition(150, 400);
        window->draw(instructionsText);
    }
}

// Display confirmation dialog
void Menu::displayConfirmationDialog() {
    // Create a semi-transparent overlay
    RectangleShape overlay;
    overlay.setSize(Vector2f(window->getSize()));
    overlay.setFillColor(Color(0, 0, 0, 150));
    window->draw(overlay);

    // Create dialog panel
    RectangleShape dialogPanel;
    dialogPanel.setSize(Vector2f(400, 200));
    dialogPanel.setPosition(window->getSize().x / 2 - 200, window->getSize().y / 2 - 100);
    dialogPanel.setFillColor(Color(50, 50, 70));
    dialogPanel.setOutlineThickness(2);
    dialogPanel.setOutlineColor(Color(100, 100, 150));
    window->draw(dialogPanel);

    // Draw message
    Text messageText;
    messageText.setFont(font);
    messageText.setString(confirmationMessage);
    messageText.setCharacterSize(20);
    messageText.setFillColor(Color::White);
    messageText.setPosition(
        window->getSize().x / 2 - messageText.getLocalBounds().width / 2,
        window->getSize().y / 2 - 60
    );
    window->draw(messageText);

    // Draw Yes button
    RectangleShape yesButton;
    yesButton.setSize(Vector2f(100, 40));
    yesButton.setPosition(window->getSize().x / 2 - 120, window->getSize().y / 2 + 20);
    yesButton.setFillColor(Color(0, 100, 0));
    yesButton.setOutlineThickness(2);
    yesButton.setOutlineColor(Color::White);
    window->draw(yesButton);

    Text yesText;
    yesText.setFont(font);
    yesText.setString("Yes");
    yesText.setCharacterSize(20);
    yesText.setFillColor(Color::White);
    yesText.setPosition(
        window->getSize().x / 2 - 90,
        window->getSize().y / 2 + 30
    );
    window->draw(yesText);

    // Draw No button
    RectangleShape noButton;
    noButton.setSize(Vector2f(100, 40));
    noButton.setPosition(window->getSize().x / 2 + 20, window->getSize().y / 2 + 20);
    noButton.setFillColor(Color(100, 0, 0));
    noButton.setOutlineThickness(2);
    noButton.setOutlineColor(Color::White);
    window->draw(noButton);

    Text noText;
    noText.setFont(font);
    noText.setString("No");
    noText.setCharacterSize(20);
    noText.setFillColor(Color::White);
    noText.setPosition(
        window->getSize().x / 2 + 55,
        window->getSize().y / 2 + 30
    );
    window->draw(noText);
}

// Show confirmation dialog
void Menu::showConfirmation(const std::string& message, bool isSave) {
    showConfirmationDialog = true;
    confirmationMessage = message;
    isSaveOperation = isSave;
}

// Initialize the matchmaking UI
void Menu::initializeMatchmaking() {
    menuItems.clear();
    buttons.clear();
    hints.clear();

    // Create title
    Text title;
    title.setFont(font);
    title.setString("Matchmaking Queue");
    title.setCharacterSize(50);
    title.setFillColor(Color::White);
    title.setPosition(200, 50);
    menuItems.push_back(title);

    // Status text
    Text statusText;
    statusText.setFont(font);
    statusText.setString("Waiting for players...");
    statusText.setCharacterSize(30);
    statusText.setFillColor(Color::White);
    statusText.setPosition(250, 150);
    menuItems.push_back(statusText);

    // Players in queue text
    Text queueText;
    queueText.setFont(font);
    queueText.setString("Players in queue: " + std::to_string(matchmakingQueue.getSize()));
    queueText.setCharacterSize(24);
    queueText.setFillColor(Color::White);
    queueText.setPosition(250, 200);
    menuItems.push_back(queueText);

    // Your score text
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Your score: " + std::to_string(auth->getPlayerScore(currentUser)));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(250, 250);
    menuItems.push_back(scoreText);

    // Cancel button
    Text cancelText;
    cancelText.setFont(font);
    cancelText.setString("Cancel Matchmaking");
    cancelText.setCharacterSize(30);
    cancelText.setFillColor(Color::White);
    cancelText.setPosition(250, 350);
    menuItems.push_back(cancelText);

    RectangleShape cancelButton;
    cancelButton.setSize(Vector2f(300, 50));
    cancelButton.setPosition(240, 350);
    cancelButton.setFillColor(Color(150, 0, 0));
    cancelButton.setOutlineThickness(2);
    cancelButton.setOutlineColor(Color::White);
    buttons.push_back(cancelButton);
}

// Handle matchmaking input
void Menu::handleMatchmakingInput(Event& event) {
    if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Left) {
            Vector2i mousePos = Mouse::getPosition(*window);

            // Check if cancel button was clicked
            if (buttons[0].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                // Remove player from queue
                removeFromMatchmakingQueue(currentUser);
                // Return to main menu
                setState(MenuState::MAIN_MENU);
            }
        }
    }
}

// Display matchmaking UI
void Menu::displayMatchmaking() {
    const float windowWidth = static_cast<float>(window->getSize().x);
    const float windowHeight = static_cast<float>(window->getSize().y);

    // Draw background
    RectangleShape background(Vector2f(windowWidth, windowHeight));
    background.setFillColor(Color(50, 50, 150)); // Dark blue background
    window->draw(background);

    // Update players in queue text (index 2)
    menuItems[2].setString("Players in queue: " + std::to_string(matchmakingQueue.getSize()));

    // Draw all menu items
    for (const auto& item : menuItems) {
        window->draw(item);
    }

    // Draw all buttons
    for (const auto& button : buttons) {
        window->draw(button);
    }

    // Draw animated "searching" dots
    static float animTimer = 0.0f;
    static int dotCount = 0;

    animTimer += 0.01f;
    if (animTimer >= 0.5f) {
        animTimer = 0.0f;
        dotCount = (dotCount + 1) % 4;
    }

    std::string dots;
    for (int i = 0; i < dotCount; i++) {
        dots += ".";
    }

    Text searchingText;
    searchingText.setFont(font);
    searchingText.setString("Searching for match" + dots);
    searchingText.setCharacterSize(24);
    searchingText.setFillColor(Color::Yellow);
    searchingText.setPosition(250, 300);
    window->draw(searchingText);
}

// Enter matchmaking queue
void Menu::enterMatchmakingQueue(const std::string& username, int score) {
    // Add player to the queue
    matchmakingQueue.enqueue(username, score);
    // Switch to matchmaking state
    setState(MenuState::MATCHMAKING);
}

// Remove from matchmaking queue
void Menu::removeFromMatchmakingQueue(const std::string& username) {
    matchmakingQueue.removePlayer(username);
}

// Process matchmaking
bool Menu::processMatchmaking() {
    // Check if there are at least 2 players in the queue
    if (matchmakingQueue.getSize() < 2) {
        return false;
    }

    // Try to find a match
    MatchmakingPlayer player1, player2;
    bool matchFound = matchmakingQueue.findMatch(player1, player2);

    // If match found, start a new multiplayer game
    if (matchFound) {
        // Set player usernames and start multiplayer game
        currentUser = player1.username;
        player2Username = player2.username;

        // Create and initialize game if needed
        if (game == nullptr) {
            game = new Game();
        }

        // Initialize multiplayer game with the two matched players
        game->initializeMultiplayer(player1.username, player2.username);

        // Switch to multiplayer state
        setState(MenuState::MULTIPLAYER);
        return true;
    }

    return false;
}
