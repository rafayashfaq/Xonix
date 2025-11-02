/*
    Player.cpp
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - Data Structures and Algorithms Project
*/

#include "Player.h"

// Default constructor
Player::Player() {
    x = y = 0;
    dx = dy = 0;
    playerID = 0;
    score = 0;
    powerups = 0;
    isActive = true;
    username = "";
}

// Constructor with player ID
Player::Player(int id) {
    x = y = 0;
    dx = dy = 0;
    playerID = id;
    score = 0;
    powerups = 0;
    isActive = true;
    username = "";
}

// Move the player
void Player::move() {
    x += dx;
    y += dy;
}

// Set player direction
void Player::setDirection(int dirX, int dirY) {
    dx = dirX;
    dy = dirY;
}

// Reset player position
void Player::reset(int startX, int startY) {
    x = startX;
    y = startY;
    dx = dy = 0;
    isActive = true;
}

// Add points to score
void Player::addScore(int points) {
    score += points;

    // Award powerups based on score thresholds
    if (score >= 50 && (score - points) < 50) {
        addPowerup();
    }
    if (score >= 70 && (score - points) < 70) {
        addPowerup();
    }
    if (score >= 100 && (score - points) < 100) {
        addPowerup();
    }
    if (score >= 130 && (score - points) < 130) {
        addPowerup();
    }
    // For every 30 points after 130
    if (score >= 130 && (score / 30) > ((score - points) / 30)) {
        addPowerup();
    }
}

// Add a powerup
void Player::addPowerup() {
    powerups++;
}

// Use a powerup
void Player::usePowerup() {
    if (powerups > 0) {
        powerups--;
    }
}

// Getters
int Player::getX() const {
    return x;
}

int Player::getY() const {
    return y;
}

int Player::getDX() const {
    return dx;
}

int Player::getDY() const {
    return dy;
}

int Player::getScore() const {
    return score;
}

int Player::getPowerups() const {
    return powerups;
}

int Player::getPlayerID() const {
    return playerID;
}

bool Player::getIsActive() const {
    return isActive;
}

std::string Player::getUsername() const {
    return username;
}

// Setters
void Player::setX(int newX) {
    x = newX;
}

void Player::setY(int newY) {
    y = newY;
}

void Player::setIsActive(bool active) {
    isActive = active;
}

void Player::setPlayerID(int id) {
    playerID = id;
}

void Player::setUsername(const std::string& name) {
    username = name;
}

// Friend system methods

// Check if a user is in our friends list
bool Player::isFriend(const std::string& username) {
    return friends.contains(username);
}

// Check if we have an outgoing request to this username
bool Player::hasOutgoingRequest(const std::string& username) {
    return outgoingRequests.contains(username);
}

// Check if we have an incoming request from this username
bool Player::hasIncomingRequest(const std::string& username) {
    return incomingRequests.contains(username);
}

// Add a friend to our friends list
void Player::addFriend(const std::string& username) {
    if (!isFriend(username)) {
        friends.addLast(username);
    }
}

// Add an outgoing friend request
void Player::addOutgoingRequest(const std::string& username) {
    if (!hasOutgoingRequest(username)) {
        outgoingRequests.addLast(username);
    }
}

// Add an incoming friend request
void Player::addIncomingRequest(const std::string& username) {
    if (!hasIncomingRequest(username)) {
        incomingRequests.addLast(username);
    }
}

// Remove an incoming friend request
void Player::removeIncomingRequest(const std::string& username) {
    incomingRequests.remove(username);
}

// Remove an outgoing friend request
void Player::removeOutgoingRequest(const std::string& username) {
    outgoingRequests.remove(username);
}

// Remove a friend from our friends list
void Player::removeFriend(const std::string& username) {
    friends.remove(username);
}

// Get the friend list as an array with count
void Player::getFriendList(std::string* result, int& count) {
    // Create a temporary array to hold all friends
    std::string* tempArray = new std::string[100]; // Assuming max 100 friends
    int tempCount = 0;

    // Use a helper function to traverse the linked list and fill the array
    Node<std::string>* current = friends.getHead();
    while (current != nullptr) {
        tempArray[tempCount++] = current->data;
        current = current->next;
    }

    // Copy to the result array
    count = tempCount;
    for (int i = 0; i < tempCount; i++) {
        result[i] = tempArray[i];
    }

    delete[] tempArray;
}

// Get the incoming requests as an array with count
void Player::getIncomingRequests(std::string* result, int& count) {
    // Create a temporary array to hold all requests
    std::string* tempArray = new std::string[100]; // Assuming max 100 requests
    int tempCount = 0;

    // Use a helper function to traverse the linked list and fill the array
    Node<std::string>* current = incomingRequests.getHead();
    while (current != nullptr) {
        tempArray[tempCount++] = current->data;
        current = current->next;
    }

    // Copy to the result array
    count = tempCount;
    for (int i = 0; i < tempCount; i++) {
        result[i] = tempArray[i];
    }

    delete[] tempArray;
}

// Get the outgoing requests as an array with count
void Player::getOutgoingRequests(std::string* result, int& count) {
    // Create a temporary array to hold all requests
    std::string* tempArray = new std::string[100]; // Assuming max 100 requests
    int tempCount = 0;

    // Use a helper function to traverse the linked list and fill the array
    Node<std::string>* current = outgoingRequests.getHead();
    while (current != nullptr) {
        tempArray[tempCount++] = current->data;
        current = current->next;
    }

    // Copy to the result array
    count = tempCount;
    for (int i = 0; i < tempCount; i++) {
        result[i] = tempArray[i];
    }

    delete[] tempArray;
}