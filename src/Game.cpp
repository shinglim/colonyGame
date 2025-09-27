#include "Game.h"
#include <string>      // std::string, std::to_string
#include <fstream>     // std::ofstream
#include <iostream>    // std::cerr, std::cout
#include <cstdlib>     // rand(), srand()
#include <ctime>       // time()
#include <filesystem>  // std::filesystem::absolute

// C++17 for root path handling

Game::~Game() {
    clean();
}

bool Game::init() {
    if (!window.create("Mini RimWorld", 0, 0, true)) {
        return false;
    }

    // Allocate heap memory for the map
    world.resize(GRID_SIZE, std::vector<GridZone>(GRID_SIZE));


    if (!loadMapFromFile(getMapPath())) {
        std::cerr << "Failed to load map from: " << getMapPath() << "\nGenerating new map.\n";
        generateMapFile(getMapPath());
    }

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

    int fileGridSize;
    if (!(file >> fileGridSize) || fileGridSize <= 0) {
        std::cerr << "Error: Invalid grid size in " << filename << "\n";
        return false;
    }

    if (fileGridSize != GRID_SIZE) {
        std::cerr << "Grid size mismatch: file=" << fileGridSize
                  << ", expected=" << GRID_SIZE << ". Regenerating map.\n";
        file.close();
        generateMapFile(filename);
        return loadMapFromFile(filename);
    }

    world.resize(GRID_SIZE, std::vector<GridZone>(GRID_SIZE));
    file.close();

    loadMapFromJson(filename);
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

        if (e.type == SDL_MOUSEWHEEL) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            float zoomDelta = e.wheel.y > 0 ? 0.1f : -0.1f;
            handleZoom(zoomDelta, mouseX, mouseY);
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int gridX = mouseToGridX(e.button.x);
            int gridY = mouseToGridY(e.button.y);

            if (e.button.button == SDL_BUTTON_LEFT) {
                selectedIndex = -1;
                for (int i = 0; i < (int)colonists.size(); i++) {
                    if (colonists[i]->isAt(gridX, gridY)) {
                        selectedIndex = i;
                        std::cout<<selectedIndex;
                        std::cout<<"selected c"<<"\n";
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

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_g) generateMapFile(getMapPath());
            if (e.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
            if (e.key.keysym.sym == SDLK_SPACE) {
                colonists.push_back(
                    new Character(rand() % GRID_SIZE, rand() % GRID_SIZE, {255, 0, 0, 255})
                );
            }

            // Camera movement keys
            if (e.key.keysym.sym == SDLK_w) moveUp = true;
            if (e.key.keysym.sym == SDLK_s) moveDown = true;
            if (e.key.keysym.sym == SDLK_a) moveLeft = true;
            if (e.key.keysym.sym == SDLK_d) moveRight = true;
        }

        if (e.type == SDL_KEYUP) {
            if (e.key.keysym.sym == SDLK_w) moveUp = false;
            if (e.key.keysym.sym == SDLK_s) moveDown = false;
            if (e.key.keysym.sym == SDLK_a) moveLeft = false;
            if (e.key.keysym.sym == SDLK_d) moveRight = false;
        }
    }
}


void Game::update() {
    // Smooth camera panning
    if (moveUp)    cameraY -= panSpeed;
    if (moveDown)  cameraY += panSpeed;
    if (moveLeft)  cameraX -= panSpeed;
    if (moveRight) cameraX += panSpeed;

    // Clamp camera so it doesn't go out of bounds
    int maxCamX = GRID_SIZE - (window.getWidth() / (CELL_SIZE * cameraScale));
    int maxCamY = GRID_SIZE - (window.getHeight() / (CELL_SIZE * cameraScale));
    if (cameraX < 0) cameraX = 0;
    if (cameraY < 0) cameraY = 0;
    if (cameraX > maxCamX) cameraX = maxCamX;
    if (cameraY > maxCamY) cameraY = maxCamY;

    // Update colonists
    for (auto* c : colonists) c->update();
}


void Game::render() {
    window.clear();

    SDL_Renderer* renderer = window.getRenderer();
    int scaledCellSize = static_cast<int>(CELL_SIZE * cameraScale);

    // Defines limits of rendering
    int startX = cameraX;
    int endX = cameraX + (window.getWidth() / scaledCellSize) + GRID_SIZE;
    int startY = cameraY;
    int endY = cameraY + (window.getHeight() / scaledCellSize) + GRID_SIZE;

    // Clamp to grid bounds
    startX = std::max(0, startX);
    endX = std::min(GRID_SIZE, endX);
    startY = std::max(0, startY);
    endY = std::min(GRID_SIZE, endY);

    // Draw tiles with camera offset
    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            SDL_Rect cell = {
                (x - cameraX) * scaledCellSize,
                (y - cameraY) * scaledCellSize,
                scaledCellSize,
                scaledCellSize
            };

            switch (world[y][x].getTileType()) {
                case GRASS: SDL_SetRenderDrawColor(renderer, 0, 180, 0, 255); break;
                case DIRT:  SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); break;
                case WATER: SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); break;
                default:    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); break;
            }
            SDL_RenderFillRect(renderer, &cell);
        }
    }

    // Draw colonists with camera offset
    for (auto* c : colonists) {
        c->render(renderer, scaledCellSize, cameraX, cameraY);
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
    std::cout << "Absolute path: " << std::filesystem::absolute(filename) << "\n";

    // Write the grid size first so it can be loaded dynamically
    file << GRID_SIZE << "\n";

    // Probabilities: 60% Grass(0), 30% Dirt(1), 10% Water(2)
    for (int y = 0; y < GRID_SIZE; ++y) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            int seed = rand() % 100;
            TileType tileType = (seed > 30) ? TileType::GRASS : TileType::WATER;

            GridZone nextGridZone = generateGridZone(y, x, tileType);

            file << "  " << nextGridZone.toJson();

            // Comma except after last element
            if (!(x == GRID_SIZE - 1 && y == GRID_SIZE - 1)) {
                file << ",";
            }
            file << "\n";
        }
    }

    file.close();
    std::cout << "Map generated to " << filename << " with size " << GRID_SIZE << "x" << GRID_SIZE << "\n";
}

void Game::handleZoom(float zoomDelta, int mouseX, int mouseY) {
    float oldScale = cameraScale;

    // Update zoom level
    cameraScale += zoomDelta;
    if (cameraScale < 0.2f) cameraScale = 0.2f;
    if (cameraScale > 5.0f) cameraScale = 5.0f;

    // Calculate the world coordinate that the mouse was pointing to before zoom
    float worldMouseX = cameraX + (mouseX / (CELL_SIZE * oldScale));
    float worldMouseY = cameraY + (mouseY / (CELL_SIZE * oldScale));

    // Calculate where the camera should be positioned to keep that world coordinate under the mouse
    cameraX = static_cast<int>(worldMouseX - (mouseX / (CELL_SIZE * cameraScale)));
    cameraY = static_cast<int>(worldMouseY - (mouseY / (CELL_SIZE * cameraScale)));

    // Apply camera bounds (same as in your update method)
    int maxCamX = GRID_SIZE - static_cast<int>(window.getWidth() / (CELL_SIZE * cameraScale));
    int maxCamY = GRID_SIZE - static_cast<int>(window.getHeight() / (CELL_SIZE * cameraScale));
    if (cameraX < 0) cameraX = 0;
    if (cameraY < 0) cameraY = 0;
    if (cameraX > maxCamX) cameraX = maxCamX;
    if (cameraY > maxCamY) cameraY = maxCamY;
}

GridZone Game::generateGridZone(int gx,int gy, TileType tileType) {
    int arableLand = rand() % 300;
    int urbanSize = rand() % 100;
    int ruralSize = rand() % 100;


    return GridZone (gx, gy, arableLand, urbanSize, ruralSize, tileType);
}

bool Game::loadMapFromJson(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open " << filename << "\n";
        return false;
    }

    int fileGridSize;
    file >> fileGridSize;  // first line is grid size
    if (fileGridSize != GRID_SIZE) {
        std::cerr << "Grid size mismatch: file=" << fileGridSize
                  << ", expected=" << GRID_SIZE << "\n";
        return false;
    }

    // prepare the world grid
    world.clear();
    world.resize(GRID_SIZE, std::vector<GridZone>(GRID_SIZE));

    std::string line;
    while (std::getline(file, line)) {
        if (line.find('{') != std::string::npos) {
            int gx, gy, arableLand, urbanSize, ruralSize, tileType;

            // Parse exactly the format we saved in toJson()
            if (sscanf(line.c_str(),
                       " { \"gx\": %d, \"gy\": %d, \"arableLand\": %d, \"urbanSize\": %d, \"ruralSize\": %d, \"tileType\": %d }",
                       &gx, &gy, &arableLand, &urbanSize, &ruralSize, &tileType) == 6) {
                if (gx >= 0 && gx < GRID_SIZE && gy >= 0 && gy < GRID_SIZE) {
                    world[gy][gx] = GridZone(
                        gx, gy, arableLand, urbanSize, ruralSize,
                        static_cast<TileType>(tileType)
                    );
                }
                       }
        }
    }

    file.close();
    return true;
}

//TODO: create a function that generates and returns each gridzone individually as well as modifying the map read/write functions to handle the new data type