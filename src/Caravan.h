#pragma once
#include <SDL2/SDL.h>
#include "enums.h"
#include <vector>

class Caravan {
public:
    Caravan(int gx, int gy, SDL_Color c);
    ~Caravan() = default;

    void update();
    void render(SDL_Renderer* renderer, int cellSize, int cameraX, int cameraY);

    void setTarget(int gx, int gy);  // Move command
    bool isAt(int gx, int gy) const; // Check if clicked
    void select(bool sel);
    bool getSelected() const;

private:
    int gx, gy;           // Current grid position
    int targetX, targetY; // Target grid position
    bool selected;        // Is this character selected?
    int weightLimit;
    std::vector<Items> goods;
    SDL_Color color;
};
