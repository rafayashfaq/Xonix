/*
    PlayerManager.cpp
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - Data Structures and Algorithms Project
*/

#include "PlayerManager.h"
#include <iostream>

// Constructor
PlayerManager::PlayerManager(int maxSize) {
    // Initialize player array
    maxPlayers = maxSize;
    playerCount = 0;
    players = new Player * [maxPlayers];

    // Initialize all slots to nullptr
    for (int i = 0; i < maxPlayers; i++) {
        players[i] = nullptr;
    }
}

// Destructor
PlayerManager::~PlayerManager() {
    // Clean up player objects
    for (int i = 0; i < playerCount; i++) {
        if (players[i] != nullptr) {
            delete players[i];
        }
    }
    delete[] players;
}

// Register a new player
bool PlayerManager::registerPlayer(const std::string& username) {
    // Check if username already exists
    if (playerMap.containsKey(username)) {
        return false; // Player already exists
    }

    // Check if we have space
    if (playerCount >= maxPlayers) {
        // Could implement array resizing here if needed
        return false;
    }

    // Create new player
    Player* newPlayer = new Player();
    newPlayer->setUsername(username);
    newPlayer->setPlayerID(playerCount);

    // Add to array
    players[playerCount] = newPlayer;

    // Add to hash map for quick lookup
    playerMap.put(username, playerCount);

    // Increment player count
    playerCount++;

    return true;
}

// Get player by username
Player* PlayerManager::getPlayerByUsername(const std::string& username) {
    // Look up player in hash map
    int index = playerMap.get(username);
    if (index != -1 && index < playerCount) {
        return players[index];
    }
    return nullptr; // Player not found
}

// Send friend request
bool PlayerManager::sendFriendRequest(const std::string& senderUsername, const std::string& receiverUsername) {
    // Get both players
    Player* sender = getPlayerByUsername(senderUsername);
    Player* receiver = getPlayerByUsername(receiverUsername);

    // Check if both players exist
    if (sender == nullptr || receiver == nullptr) {
        return false;
    }

    // Check if they are already friends
    if (sender->isFriend(receiverUsername)) {
        return false; // Already friends
    }

    // Check if request already sent
    if (sender->hasOutgoingRequest(receiverUsername)) {
        return false; // Request already sent
    }

    // Add request to sender's outgoing requests and receiver's incoming requests
    sender->addOutgoingRequest(receiverUsername);
    receiver->addIncomingRequest(senderUsername);

    return true;
}

// Accept friend request
bool PlayerManager::acceptFriendRequest(const std::string& receiverUsername, const std::string& senderUsername) {
    // Get both players
    Player* receiver = getPlayerByUsername(receiverUsername);
    Player* sender = getPlayerByUsername(senderUsername);

    // Check if both players exist
    if (sender == nullptr || receiver == nullptr) {
        return false;
    }

    // Check if there is a pending request
    if (!receiver->hasIncomingRequest(senderUsername)) {
        return false; // No request to accept
    }

    // Add each other as friends
    receiver->addFriend(senderUsername);
    sender->addFriend(receiverUsername);

    // Remove from request lists
    receiver->removeIncomingRequest(senderUsername);
    sender->removeOutgoingRequest(receiverUsername);

    return true;
}

// Reject friend request
bool PlayerManager::rejectFriendRequest(const std::string& receiverUsername, const std::string& senderUsername) {
    // Get both players
    Player* receiver = getPlayerByUsername(receiverUsername);
    Player* sender = getPlayerByUsername(senderUsername);

    // Check if both players exist
    if (sender == nullptr || receiver == nullptr) {
        return false;
    }

    // Check if there is a pending request
    if (!receiver->hasIncomingRequest(senderUsername)) {
        return false; // No request to reject
    }

    // Remove from request lists
    receiver->removeIncomingRequest(senderUsername);
    sender->removeOutgoingRequest(receiverUsername);

    return true;
}

// Get friend list for a player
void PlayerManager::getFriendList(const std::string& username, std::string* result, int& count) {
    Player* player = getPlayerByUsername(username);
    if (player == nullptr) {
        count = 0;
        return;
    }

    player->getFriendList(result, count);
}

// Get incoming friend requests for a player
void PlayerManager::getIncomingRequests(const std::string& username, std::string* result, int& count) {
    Player* player = getPlayerByUsername(username);
    if (player == nullptr) {
        count = 0;
        return;
    }

    player->getIncomingRequests(result, count);
}

// Get outgoing friend requests for a player
void PlayerManager::getOutgoingRequests(const std::string& username, std::string* result, int& count) {
    Player* player = getPlayerByUsername(username);
    if (player == nullptr) {
        count = 0;
        return;
    }

    player->getOutgoingRequests(result, count);
}