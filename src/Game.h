#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Character.h"
#include "Tile.h"
#include "Window.h"
#include <filesystem>

class Game {
public:
    ~Game();
    bool init();
    void run();

    // Camera movement state
    bool moveUp = false;
    bool moveDown = false;
    bool moveLeft = false;
    bool moveRight = false;
    int panSpeed = 5;
    int cameraX = 0;
    int cameraY = 0;

private:
    void handleZoom(float zoomDelta, int mouseX, int mouseY);
    void handleEvents();
    void update();
    void render();
    void clean();

    bool loadMapFromFile(const std::string& filename);

    float cameraScale = 1.0f;
    int selectedIndex = -1;

    // Game.h
    static constexpr int GRID_SIZE = 500;
    static const int CELL_SIZE = 8;

    // Heap-allocated 2D array
    std::vector<std::vector<int>> world;

    std::vector<Character*> colonists;

    Window window;
    bool isRunning = false;


    int mouseToGridX(int mouseX) {
        int scaledCellSize = static_cast<int>(CELL_SIZE * cameraScale);
        return (mouseX / scaledCellSize) + cameraX;
    }

    int mouseToGridY(int mouseY) {
        int scaledCellSize = static_cast<int>(CELL_SIZE * cameraScale);
        return (mouseY / scaledCellSize) + cameraY;
    }

    std::string getMapPath() {
        // Get the directory where your project root is
        std::filesystem::path exeDir = std::filesystem::current_path();
        std::filesystem::path rootDir = exeDir.parent_path(); // one folder up from build dir
        return (rootDir / "map.txt").string();
    }

    void generateMapFile(const std::string& filename);
};
