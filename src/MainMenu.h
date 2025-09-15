#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Character.h"
#include "Tile.h"
#include "Window.h"
#include <filesystem>
#include "Game.h"
#include "structures.h"

class MainMenu {

public:
    bool init();
    void run();
    bool clean();
    bool isRunning = false;

private:

    int screenWidth, screenHeight;
    void handleEvents();
    void update();
    void render();

    bool play = false;
    Window window;


};


