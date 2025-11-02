/*
    Enemy.cpp
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - Data Structures and Algorithms Project
*/

#include "Enemy.h"
#include <stdlib.h>
#include <time.h>

// Constructor
Enemy::Enemy() {
    x = y = 300;
    dx = 4 - (rand() % 8);
    dy = 4 - (rand() % 8);
    frozen = false;
    freezeTimer = 0;
}

// Move the enemy
bool Enemy::move(int grid[][40], int tileSize) {
    if (frozen) return false;

    x += dx;

    // Check if hit a wall
    if (grid[y / tileSize][x / tileSize] == 1) {
        dx = -dx;
        x += dx;
    }

    // Check if hit a trail
    if (grid[y / tileSize][x / tileSize] == 2 || grid[y / tileSize][x / tileSize] == 3) {
        return true; // Hit a trail
    }

    y += dy;

    // Check if hit a wall
    if (grid[y / tileSize][x / tileSize] == 1) {
        dy = -dy;
        y += dy;
    }

    // Check if hit a trail
    if (grid[y / tileSize][x / tileSize] == 2 || grid[y / tileSize][x / tileSize] == 3) {
        return true; // Hit a trail
    }

    return false; // No collision with trail
}

// Update enemy state
void Enemy::update(float deltaTime) {
    if (frozen) {
        freezeTimer -= deltaTime;
        if (freezeTimer <= 0) {
            frozen = false;
        }
    }
}

// Freeze the enemy for a duration
void Enemy::freeze(float duration) {
    frozen = true;
    freezeTimer = duration;
}

// Getters
int Enemy::getX() const {
    return x;
}

int Enemy::getY() const {
    return y;
}

bool Enemy::isFrozen() const {
    return frozen;
}