//
// Created by Shing Lim on 8/8/25.
//

#include "MainMenu.h"
bool MainMenu::init() {
    if (!window.create("Mini RimWorld", 0, 0, true)) {
        return false;
    }
//TODO: write a function to get the width and height of the window and fill in the parameters of the button below to have it centred
    Button ContinueGameButton();
    isRunning = true;
    return true;
}

void MainMenu::run() {
    handleEvents();
    update();
    render();
    SDL_Delay(100);
}

void MainMenu::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) isRunning = false;

        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_g) {
                play = true;
                isRunning = false;
                std::cout << "pressed";

            }
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                clean();
            }
        }
        }
}

void MainMenu::update() {}

void MainMenu::render() {}

bool MainMenu::clean() {
    window.destroy();
    return play;
}