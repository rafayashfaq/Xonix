/*
    main.cpp
    Author: [Your Name]
    Roll Number: [Your Roll Number]
    Project Title: Xonix Game - DSA Project
*/

#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Menu.h"
#include "Auth.h"
#include "MinHeap.h"
#include "PriorityQueue.h"

using namespace sf;

int main() {
    // Create the main window
    RenderWindow window(VideoMode(800, 600), "Xonix Game");
    window.setFramerateLimit(60);

    // Initialize authentication system
    Auth auth;

    // Create menu system
    Menu menu(&window, &auth);

    // Game loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            // Handle menu input
            menu.processInput(event);
        }

        // Update menu
        menu.update(1.0f / 60.0f);  // Assuming 60 FPS

        // Display menu
        menu.display();
    }

    return 0;
}