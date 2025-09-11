#include "Character.h"
#include <cmath>

Character::Character(int gx, int gy, SDL_Color c)
    : gx(gx), gy(gy), targetX(gx), targetY(gy),
      selected(false), color(c) {}

void Character::update() {
    // Move 1 step toward target each frame
    if (gx < targetX) gx++;
    else if (gx > targetX) gx--;
    if (gy < targetY) gy++;
    else if (gy > targetY) gy--;
}

void Character::render(SDL_Renderer* renderer, int cellSize, int cameraX, int cameraY) {
    SDL_Rect rect = {
        (gx - cameraX) * cellSize,
        (gy - cameraY) * cellSize,
        cellSize,
        cellSize
    };

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);

    // Draw selection outline if selected
    if (selected) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);
    }
}

//TODO: characters target according to the grid of the original sized window and will not target beyond that area
void Character::setTarget(int x, int y) {
    targetX = x;
    targetY = y;
}

bool Character::isAt(int x, int y) const {
    return gx == x && gy == y;
}

void Character::select(bool sel) {
    selected = sel;
}

bool Character::getSelected() const {
    return selected;
}
