#pragma once
#include <vector>
#include <array>
#include <string>
#include <iostream>
#include "Character.h"
#include "Tile.h"
#include "Window.h"

class Game {
public:
    ~Game();
    bool init();
    void run();

    int cameraX = 0;
    int cameraY = 0;

private:
    void handleEvents();
    void update();
    void render();
    void clean();

    // --- Map Loading ---
    bool loadMapFromFile(const std::string& filename);

    // --- Camera & Selection ---
    float cameraScale = 1.0f;
    int selectedIndex = -1;


    // --- Game State ---
    static const int GRID_SIZE = 100;
    static const int CELL_SIZE = 8;
    std::array<std::array<int, GRID_SIZE>, GRID_SIZE> world{};
    std::vector<Character*> colonists;

    // --- Window Management ---
    Window window;
    bool isRunning = false;

    // --- Helper functions ---
    // Convert mouse pixel coordinates to grid coordinates considering camera position and zoom
    int mouseToGridX(int mouseX) {
        return cameraX + static_cast<int>(mouseX / (CELL_SIZE * cameraScale));
    }
    int mouseToGridY(int mouseY) {
        return cameraY + static_cast<int>(mouseY / (CELL_SIZE * cameraScale));
    }

    // Generate a new map file
    void generateMapFile(const std::string& filename);
};
