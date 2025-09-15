//
// Created by Shing Lim on 13/9/25.
//

#pragma once
#include <SDL2/SDL.h>


class Cities {
    Cities(int gx, int gy, SDL_Color c);
    ~Cities() = default;

    void update();
    void render(SDL_Renderer* renderer, int cellSize, int cameraX, int cameraY);

    bool isAt(int gx, int gy) const; // Check if clicked
    void select(bool sel);
    bool getSelected() const;

private:
    int gx, gy;           // Current grid position
    bool selected;        // Is this city selected?
    int populationSize;
    int UrbanSize;
    int RuralSize;
    int ArableLand;
    int huntableWildlife;

    SDL_Color color;
};


