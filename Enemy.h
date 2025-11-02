/*
    Enemy.h
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - Data Structures and Algorithms Project

    Description:
    This file contains the Enemy class which represents enemies in the game.
*/

#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
private:
    int x, y;     // Enemy position
    int dx, dy;   // Enemy direction
    bool frozen;  // Whether enemy is frozen by powerup
    float freezeTimer; // Time remaining frozen

public:
    // Constructor
    Enemy();

    // Movement method - now returns bool to indicate trail collision
    bool move(int grid[][40], int tileSize);
    void update(float deltaTime);

    // Freeze method (for player powerup)
    void freeze(float duration);

    // Getters
    int getX() const;
    int getY() const;
    bool isFrozen() const;
};

#endif // ENEMY_H