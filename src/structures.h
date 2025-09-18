#pragma once
#include <SDL2/SDL.h>
class Button {
public:
    // Constructor
    Button(int gx, int gy, int width, int height);

    // Render the button
    void render(SDL_Renderer* renderer, SDL_Color color);

    // Check if the button is clicked
    bool checkifClicked(int mouseX, int mouseY) const;

private:
    int gx, gy;
    int width, height;
};

//TODO: restructure the tile system to be able to store values like arable land
//eventually make the map use this as the data type
class GridZone {
public:
    GridZone(int gx, int gy, int arableLand, int urbanSize, int ruralSize);

private:
    int gx, gy;
    int arableLand;
    int urbanSize;
    int ruralSize;
};
