//
// Created by Shing Lim on 24/8/25.
//
#include "structures.h"

Button::Button(int gx, int gy, int width, int height)
    : gx(gx), gy(gy), width(width), height(height) {}

void Button::render(SDL_Renderer* renderer, SDL_Color color) {
    SDL_Rect rect = {gx, gy, width, height};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

bool Button::checkifClicked(int mouseX, int mouseY) const {
    return (mouseX >= gx && mouseX <= gx + width &&
            mouseY >= gy && mouseY <= gy + height);
}



