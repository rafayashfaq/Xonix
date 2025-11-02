/*
    Auth.h
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - DSA Project

    Description:
    This file contains the Auth class which handles user authentication and registration.
*/

#ifndef AUTH_H
#define AUTH_H

#include <string>
#include "HashTable.h"

struct MatchHistory {
    char opponent[32];
    bool win;
    int score;
    long timestamp;
};

class Auth {
private:
    HashTable* userTable;  // Hash table to store user credentials
    std::string currentUser;  // Currently logged in user
    bool isLoggedIn;

    // Helper functions
    bool validatePassword(const std::string& password);
    bool validateUsername(const std::string& username);
    std::string hashPassword(const std::string& password);
    void saveToFile();
    void loadFromFile();

public:
    // Constructor and Destructor
    Auth();
    ~Auth();

    // Authentication methods
    bool registerUser(const std::string& username, const std::string& password);
    bool login(const std::string& username, const std::string& password);
    void logout();
    bool isUserLoggedIn() const;
    std::string getCurrentUser() const;

    // Player stats and friends methods
    int getPlayerWins(const std::string& username);
    int getPlayerLosses(const std::string& username);
    int getPlayerPoints(const std::string& username);
    int getPlayerScore(const std::string& username);  // Get player's current score
    int getRecentMatches(const std::string& username, MatchHistory* matches, int maxCount);
    int getFriendsList(const std::string& username, char friendsList[][32], int maxCount);

    // Profile management
    bool updateProfile(const std::string& username, const std::string& field, const std::string& value);
    bool deleteAccount(const std::string& username, const std::string& password);
};

#endif // AUTH_H