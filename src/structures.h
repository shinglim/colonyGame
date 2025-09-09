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
