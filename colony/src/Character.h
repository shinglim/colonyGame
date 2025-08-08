#pragma once
#include <SDL2/SDL.h>

class Character {
public:
    Character(int gridX, int gridY, SDL_Color c);
    ~Character() = default;

    void update();
    void render(SDL_Renderer* renderer, int cellSize);

    void setTarget(int gx, int gy);  // Move command
    bool isAt(int gx, int gy) const; // Check if clicked
    void select(bool sel);
    bool getSelected() const;

private:
    int gx, gy;           // Current grid position
    int targetX, targetY; // Target grid position
    bool selected;        // Is this character selected?
    SDL_Color color;
};
