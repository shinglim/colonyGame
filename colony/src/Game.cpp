#include "Game.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <filesystem> // C++17 for root path handling

Game::~Game() {
    clean();
}

bool Game::init() {
    int visibleTilesX = window.getWidth() / (CELL_SIZE * cameraScale);
    int visibleTilesY = window.getHeight() / (CELL_SIZE * cameraScale);

    if (!window.create("Mini RimWorld", 0, 0, true)) {
        return false;
    }

    // Get path to map.txt
    std::string directoryPath = std::filesystem::current_path();
    std::filesystem::path p(directoryPath);
    std::string mapPathI = p.parent_path().string();
    std::string mapPath = mapPathI + "/map.txt";

    // Load map from file instead of generating randomly
    if (!loadMapFromFile(mapPath)) {
        std::cerr << "Failed to load map from: " << mapPath << "\nUsing default grass map.\n";
        for (int y = 0; y < GRID_SIZE; ++y) {
            for (int x = 0; x < GRID_SIZE; ++x) {
                world[y][x] = GRASS;
            }
        }
    }

    // Spawn initial colonist
    colonists.push_back(new Character(GRID_SIZE / 2, GRID_SIZE / 2, {255, 255, 0, 255}));

    isRunning = true;
    return true;
}

bool Game::loadMapFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open " << filename << "\n";
        return false;
    }

    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            int tileType;
            if (!(file >> tileType)) {
                std::cerr << "Error: Invalid format in " << filename << "\n";
                return false;
            }
            world[y][x] = tileType;
        }
    }

    file.close();
    return true;
}

void Game::run() {
    while (isRunning) {
        handleEvents();
        update();
        render();
        SDL_Delay(100);
    }
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) isRunning = false;


        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int gridX = mouseToGridX(e.button.x);
            int gridY = mouseToGridY(e.button.y);

            if (e.button.button == SDL_BUTTON_LEFT) {
                selectedIndex = -1;
                for (int i = 0; i < (int)colonists.size(); i++) {
                    if (colonists[i]->isAt(gridX, gridY)) {
                        selectedIndex = i;
                    }
                    colonists[i]->select(i == selectedIndex);
                }
            }

            if (e.button.button == SDL_BUTTON_RIGHT) {
                if (selectedIndex != -1) {
                    colonists[selectedIndex]->setTarget(gridX, gridY);
                }
            }
        }

        if (e.type == SDL_MOUSEWHEEL) {
            if (e.wheel.y > 0) cameraScale *= 1.1f;
            if (e.wheel.y < 0) cameraScale *= 0.9f;
            if (cameraScale < 0.2f) cameraScale = 0.2f;
            if (cameraScale > 5.0f) cameraScale = 5.0f;
        }

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_g) {
                generateMapFile("map.txt");
            }
            if (e.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
            if (e.key.keysym.sym == SDLK_SPACE) {
                colonists.push_back(
                    new Character(rand() % GRID_SIZE, rand() % GRID_SIZE, {255, 0, 0, 255})
                );
            }
        }
    }
}

void Game::update() {
    for (auto* c : colonists) c->update();
}

void Game::render() {
    window.clear();

    SDL_Renderer* renderer = window.getRenderer();
    int scaledCellSize = static_cast<int>(CELL_SIZE * cameraScale);

    // Draw tiles
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            SDL_Rect cell = { x * scaledCellSize, y * scaledCellSize, scaledCellSize, scaledCellSize };
            switch (world[y][x]) {
                case GRASS: SDL_SetRenderDrawColor(renderer, 0, 180, 0, 255); break;
                case DIRT:  SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); break;
                case WATER: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;
                default:    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); break;
            }
            SDL_RenderFillRect(renderer, &cell);
        }
    }

    // Draw colonists
    for (auto* c : colonists) {
        c->render(renderer, scaledCellSize);
    }

    window.present();
}

void Game::clean() {
    for (auto* c : colonists) delete c;
    colonists.clear();
    window.destroy();
}

void Game::generateMapFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << " for writing\n";
        return;
    }

    // Probabilities: 60% Grass(0), 30% Dirt(1), 10% Water(2)
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            int seed = rand() % 100;
            int tileType;
            if (seed < 60) tileType = 0;
            else if (seed < 90) tileType = 1;
            else tileType = 2;

            file << tileType;
            if (x < GRID_SIZE - 1) file << " ";
        }
        file << "\n";
    }
    file.close();
    std::cout << "Map generated to " << filename << "\n";
}