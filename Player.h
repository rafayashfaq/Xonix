/*
    Player.h
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - Data Structures and Algorithms Project

    Description:
    This file contains the Player class which represents a player in the game.
*/

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <string>
#include "LinkedList.h"
using namespace sf;

class Player {
private:
    int x, y;         // Player position in grid
    int dx, dy;       // Player direction
    int playerID;     // Unique player ID
    int score;        // Player's current score
    int powerups;     // Player's powerup count
    bool isActive;    // Is player currently playing
    std::string username;

    LinkedList<std::string> friends; // List of friend usernames
    LinkedList<std::string> incomingRequests; // Friend requests received
    LinkedList<std::string> outgoingRequests; // Friend requests sent

public:
    // Constructors
    Player();
    Player(int id);

    // Movement methods
    void move();
    void setDirection(int dirX, int dirY);

    // State methods
    void reset(int startX, int startY);
    void addScore(int points);
    void addPowerup();
    void usePowerup();

    // Getters
    int getX() const;
    int getY() const;
    int getDX() const;
    int getDY() const;
    int getScore() const;
    int getPowerups() const;
    int getPlayerID() const;
    bool getIsActive() const;
    std::string getUsername() const;

    // Setters
    void setX(int newX);
    void setY(int newY);
    void setIsActive(bool active);
    void setPlayerID(int id);
    void setUsername(const std::string& name);

    // Friend system methods
    bool isFriend(const std::string& username);
    bool hasOutgoingRequest(const std::string& username);
    bool hasIncomingRequest(const std::string& username);

    void addFriend(const std::string& username);
    void addOutgoingRequest(const std::string& username);
    void addIncomingRequest(const std::string& username);

    void removeIncomingRequest(const std::string& username);
    void removeOutgoingRequest(const std::string& username);
    void removeFriend(const std::string& username);

    // Get lists as arrays with count
    void getFriendList(std::string* result, int& count);
    void getIncomingRequests(std::string* result, int& count);
    void getOutgoingRequests(std::string* result, int& count);
};

#endif // PLAYER_H