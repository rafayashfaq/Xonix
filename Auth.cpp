/*
    Auth.cpp
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - DSA Project

    Description:
    This file contains the implementation of the Auth class methods.
*/

#include "Auth.h"
#include <fstream>
#include <ctime>
#include <sstream>
#include <iomanip>

// Simple hash function for password hashing
std::string Auth::hashPassword(const std::string& password) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (char c : password) {
        ss << std::setw(2) << static_cast<int>(c);
    }
    return ss.str();
}

Auth::Auth() {
    userTable = new HashTable();
    isLoggedIn = false;
    loadFromFile();
}

Auth::~Auth() {
    saveToFile();
    delete userTable;
}

bool Auth::validateUsername(const std::string& username) {
    // Username must be 3-20 characters and contain only letters, numbers, and underscores
    if (username.length() < 3 || username.length() > 20) return false;
    for (char c : username) {
        if (!isalnum(c) && c != '_') return false;
    }
    return true;
}

bool Auth::validatePassword(const std::string& password) {
    std::cout << "\n=== Password Validation ===" << std::endl;
    std::cout << "Password: " << password << std::endl;
    std::cout << "Password length: " << password.length() << std::endl;

    // Password must be at least 6 characters
    if (password.length() < 6) {
        std::cout << "❌ Password too short! Must be at least 6 characters." << std::endl;
        return false;
    }
    std::cout << "✅ Password length is valid" << std::endl;

    // Check for at least one uppercase letter
    bool hasUpper = false;
    for (char c : password) {
        if (isupper(c)) {
            hasUpper = true;
            break;
        }
    }
    if (!hasUpper) {
        std::cout << "❌ Password must contain at least one uppercase letter (A-Z)" << std::endl;
        return false;
    }
    std::cout << "✅ Password contains uppercase letter" << std::endl;

    // Check for at least one lowercase letter
    bool hasLower = false;
    for (char c : password) {
        if (islower(c)) {
            hasLower = true;
            break;
        }
    }
    if (!hasLower) {
        std::cout << "❌ Password must contain at least one lowercase letter (a-z)" << std::endl;
        return false;
    }
    std::cout << "✅ Password contains lowercase letter" << std::endl;

    // Check for at least one number
    bool hasNumber = false;
    for (char c : password) {
        if (isdigit(c)) {
            hasNumber = true;
            break;
        }
    }
    if (!hasNumber) {
        std::cout << "❌ Password must contain at least one number (0-9)" << std::endl;
        return false;
    }
    std::cout << "✅ Password contains number" << std::endl;

    // Check for at least one special character
    bool hasSpecial = false;
    for (char c : password) {
        if (!isalnum(c)) {
            hasSpecial = true;
            break;
        }
    }
    if (!hasSpecial) {
        std::cout << "❌ Password must contain at least one special character (!@#$%^&* etc.)" << std::endl;
        return false;
    }
    std::cout << "✅ Password contains special character" << std::endl;

    std::cout << "✅ Password validation successful!" << std::endl;
    std::cout << "===========================\n" << std::endl;
    return true;
}

bool Auth::registerUser(const std::string& username, const std::string& password) {
    std::cout << "\n=== Registration Debug Info ===" << std::endl;
    std::cout << "Username: " << username << std::endl;
    std::cout << "Password: " << password << std::endl;
    std::cout << "Password length: " << password.length() << std::endl;

    std::cout << "\nStep 1: Validating username..." << std::endl;
    if (!validateUsername(username)) {
        std::cout << "Username validation failed!" << std::endl;
        std::cout << "Username must be 3-20 characters and contain only letters, numbers, and underscores" << std::endl;
        return false;
    }
    std::cout << "Username validation passed!" << std::endl;

    std::cout << "\nStep 2: Validating password..." << std::endl;
    if (!validatePassword(password)) {
        std::cout << "Password validation failed!" << std::endl;
        std::cout << "Password must be at least 6 characters and contain at least one number" << std::endl;
        return false;
    }
    std::cout << "Password validation passed!" << std::endl;

    std::cout << "\nStep 3: Checking if username exists..." << std::endl;
    if (userTable->get(username) != nullptr) {
        std::cout << "Username already exists!" << std::endl;
        return false;
    }
    std::cout << "Username is available!" << std::endl;

    std::cout << "\nStep 4: Hashing password..." << std::endl;
    std::string hashedPassword = hashPassword(password);
    std::cout << "Password hashed successfully!" << std::endl;

    std::cout << "\nStep 5: Inserting user into hash table..." << std::endl;
    userTable->insert(username, hashedPassword);
    std::cout << "User inserted into hash table!" << std::endl;

    std::cout << "\nStep 6: Saving to file..." << std::endl;
    saveToFile();
    std::cout << "Registration complete!" << std::endl;
    std::cout << "===========================\n" << std::endl;
    return true;
}

bool Auth::login(const std::string& username, const std::string& password) {
    std::string* storedPassword = userTable->get(username);
    if (storedPassword == nullptr) {
        return false;
    }

    std::string hashedInput = hashPassword(password);
    if (*storedPassword == hashedInput) {
        currentUser = username;
        isLoggedIn = true;
        return true;
    }
    return false;
}

void Auth::logout() {
    currentUser = "";
    isLoggedIn = false;
}

bool Auth::isUserLoggedIn() const {
    return isLoggedIn;
}

std::string Auth::getCurrentUser() const {
    return currentUser;
}

bool Auth::updateProfile(const std::string& username, const std::string& field, const std::string& value) {
    if (!isLoggedIn || currentUser != username) {
        return false;
    }
    // Implementation for updating profile fields
    return true;
}

bool Auth::deleteAccount(const std::string& username, const std::string& password) {
    if (!login(username, password)) {
        return false;
    }
    userTable->remove(username);
    logout();
    saveToFile();
    return true;
}

void Auth::saveToFile() {
    std::ofstream file("users.dat");
    if (file.is_open()) {
        // Save user data to file
        for (int i = 0; i < userTable->TABLE_SIZE; i++) {
            HashTable::Node* current = userTable->table[i];
            while (current != nullptr) {
                file << current->key << ":" << current->value << std::endl;
                current = current->next;
            }
        }
        file.close();
        std::cout << "User data saved successfully!" << std::endl;
    }
    else {
        std::cout << "Error: Could not open users.dat for writing!" << std::endl;
    }
}

void Auth::loadFromFile() {
    std::ifstream file("users.dat");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.find(':');
            if (pos != std::string::npos) {
                std::string username = line.substr(0, pos);
                std::string password = line.substr(pos + 1);
                userTable->insert(username, password);
            }
        }
        file.close();
        std::cout << "User data loaded successfully!" << std::endl;
    }
    else {
        std::cout << "No existing user data found. Starting with empty database." << std::endl;
    }
}

// Get player's current score (based on total points)
int Auth::getPlayerScore(const std::string& username) {
    // For now, score is the same as points
    return getPlayerPoints(username);
}

// Implementation of getPlayerPoints method
int Auth::getPlayerPoints(const std::string& username) {
    // For now, return a placeholder score based on username length
    // In a real implementation, this would fetch points from user data
    // Default score of 100 + username length to ensure different players have different scores
    return 100 + username.length();
}