#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "enums.h"

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
    GridZone()
        : gx(0), gy(0), arableLand(0), urbanSize(0), ruralSize(0), tileType(TileType::GRASS) {}

    GridZone(int gx, int gy, int arableLand, int urbanSize, int ruralSize, TileType tileType)
        : gx(gx), gy(gy), arableLand(arableLand), urbanSize(urbanSize), ruralSize(ruralSize), tileType(tileType) {}
    std::string toJson() const {
        return "{ \"gx\": " + std::to_string(gx) +
               ", \"gy\": " + std::to_string(gy) +
               ", \"arableLand\": " + std::to_string(arableLand) +
               ", \"urbanSize\": " + std::to_string(urbanSize) +
               ", \"ruralSize\": " + std::to_string(ruralSize) +
               ", \"tileType\": " + std::to_string(static_cast<int>(tileType)) +
               " }";
    }

    TileType getTileType() const { return tileType; }
private:
    int gx, gy;
    int arableLand;
    int urbanSize;
    int ruralSize;
    TileType tileType;
};
